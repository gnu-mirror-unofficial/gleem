/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <math.h>
#ifdef WIN32
# include <windows.h>
#endif
#include <GL/glut.h>
#include <gleem/HandleBoxManip.h>
#include <gleem/ManipPartTransform.h>
#include <gleem/ManipPartCube.h>
#include <gleem/ManipPartLineSeg.h>
#include <gleem/ManipPartSquare.h>
#include <gleem/MathUtil.h>

GLEEM_USE_NAMESPACE

HandleBoxManip::HandleBoxManip()
{
  translation.setValue(0, 0, 0);
  scale.setValue(1, 1, 1);
  geometryScale.setValue(1, 1, 1);
  // FIXME
  dragState = INACTIVE;
  for (int i = 0; i < 12; i++)
    lineSegs[i] = NULL;
  createGeometry();
  recalc();
}

HandleBoxManip::~HandleBoxManip()
{
  deleteGeometry();
}

void
HandleBoxManip::setTranslation(const GleemV3f &translation)
{
  this->translation = translation;
  recalc();
}

const GleemV3f &
HandleBoxManip::getTranslation() const
{
  return translation;
}

void
HandleBoxManip::setRotation(const GleemRot &rotation)
{
  this->rotation = rotation;
  recalc();
}

const GleemRot &
HandleBoxManip::getRotation() const
{
  return rotation;
}

void
HandleBoxManip::setScale(const GleemV3f &scale)
{
  this->scale = scale;
  recalc();
}

const GleemV3f &
HandleBoxManip::getScale() const
{
  return scale;
}

void
HandleBoxManip::setGeometryScale(const GleemV3f &geometryScale)
{
  this->geometryScale = geometryScale;
  recalc();
}

const GleemV3f &
HandleBoxManip::getGeometryScale() const
{
  return geometryScale;
}

void
HandleBoxManip::render()
{
  // FIXME: Add scale handles
  int i;
  for (i = 0; i < 12; i++)
    lineSegs[i]->render();

  for (i = 0; i < rotateHandles.size(); i++)
    rotateHandles[i].geometry->render();
  for (i = 0; i < scaleHandles.size(); i++)
    scaleHandles[i].geometry->render();
}

void
HandleBoxManip::intersectRay(const GleemV3f &rayStart,
			     const GleemV3f &rayDirection,
			     vector<HitPoint> &results)
{
  int i;
  for (i = 0; i < faces.size(); i++)
    faces[i].centerSquare->intersectRay(rayStart, rayDirection, results);
  for (i = 0; i < rotateHandles.size(); i++)
    rotateHandles[i].geometry->intersectRay(rayStart, rayDirection, results);
  for (i = 0; i < scaleHandles.size(); i++)
    scaleHandles[i].geometry->intersectRay(rayStart, rayDirection, results);
}

void
HandleBoxManip::highlight(const HitPoint &hit)
{
  ManipPart *part = hit.manipPart;
  if (part == NULL)
    {
      assert(0);
      return;
    }
  // Search for this part in the FaceInfo array
  for (int i = 0; i < faces.size(); i++)
    {
      FaceInfo &info = faces[i];
      if (info.centerSquare == part)
	{
	  for (int j = 0; j < 4; j++)
	    {
	      info.lineSegs[j]->highlight();
	      highlightedGeometry.push_back(info.lineSegs[j]);
	    }
	  return;
	}
    }
  // Otherwise, was a rotation or scale handle
  part->highlight();
  highlightedGeometry.push_back(part);
}

void
HandleBoxManip::clearHighlight()
{
  for (int i = 0; i < highlightedGeometry.size(); i++)
    highlightedGeometry[i]->clearHighlight();
  highlightedGeometry.erase(highlightedGeometry.begin(),
			    highlightedGeometry.end());
}

void
HandleBoxManip::makeActive(const HitPoint &hit)
{
  // Find which piece of geometry it is
  int i;
  for (i = 0; i < faces.size(); i++)
    {
      if (faces[i].centerSquare == hit.manipPart)
	{
	  dragState = TRANSLATE;
	  dragPlane.setPoint(hit.intPt);
	  dragPlane.setNormal(faces[i].normal);
	  dragOffset = translation - hit.intPt;
	  for (int j = 0; j < 4; j++)
	    {
	      faces[i].lineSegs[j]->highlight();
	      draggedGeometry.push_back(faces[i].lineSegs[j]);
	    }
	  return;
	}      
    }

  for (i = 0; i < rotateHandles.size(); i++)
    {
      RotateHandleInfo &rotInfo = rotateHandles[i];
      if (rotInfo.geometry == hit.manipPart)
	{
	  dragState = ROTATE;
	  // Determine which direction we're rotating by taking dot
	  // products of the ray direction with the rotating planes'
	  // normals
	  float dotp0 =
	    fabs(hit.rayDirection.dot(faces[rotInfo.faceIdx0].normal));
	  float dotp1 =
	    fabs(hit.rayDirection.dot(faces[rotInfo.faceIdx1].normal));
	  int faceIdx;
	  if (dotp0 > dotp1)
	    faceIdx = rotInfo.faceIdx0;
	  else
	    faceIdx = rotInfo.faceIdx1;
	  FaceInfo &face = faces[faceIdx];
	  // Set up the rotation plane
	  rotatePlane.setOrigin(translation);
	  rotatePlane.setNormal(face.normal);
	  GleemV3f dummy;
	  GleemV2f startUV;
	  rotatePlane.projectPoint(hit.intPt, dummy, startUV);
	  startAngle = atan2f(startUV[1], startUV[0]);
	  startRot = rotation;
	  rotInfo.geometry->highlight();
	  draggedGeometry.push_back(rotInfo.geometry);
	  return;
	}
    }

  for (i = 0; i < scaleHandles.size(); i++)
    {
      ScaleHandleInfo &info = scaleHandles[i];
      if (info.geometry == hit.manipPart)
	{
	  int mods = glutGetModifiers();
	  if (mods & GLUT_ACTIVE_SHIFT)
	    {
	      dragState = SCALE_SINGLE_AXIS;
	      // Figure out which are the two axes along which we're
	      // going to allow scaling by taking dot products of the
	      // ray direction with the normals of the adjacent faces
	      // to the scale handle.
	      float dotp = 0.0f;
	      float tmpDotp;
	      int faceIdx;
	      for (int i = 0; i < 3; i++)
		{
		  FaceInfo &faceInfo = faces[info.faceIndices[i]];
		  tmpDotp = faceInfo.normal.dot(hit.rayDirection);
		  // cerr << "i: " << i << " tmpDotp: " << tmpDotp << endl;
		  if ((i == 0) || (tmpDotp < dotp))
		    {
		      dotp = tmpDotp;
		      faceIdx = info.faceIndices[i];
		    }
		}
	      // cerr << "faceIdx: " << faceIdx << " dotp: " << dotp << endl;
	      scaleAxes = faces[faceIdx].scaleAxes;
	      // cerr << "scaleAxes: ";
	      GleemV3f uAxisOrig, vAxisOrig;
	      if (scaleAxes == SCALE_XY)
		{
		  uAxisOrig.setValue(1, 0, 0);
		  vAxisOrig.setValue(0, 1, 0);
		  // cerr << "SCALE_XY";
		}
	      else if (scaleAxes == SCALE_YZ)
		{
		  uAxisOrig.setValue(0, 1, 0);
		  vAxisOrig.setValue(0, 0, 1);
		  // cerr << "SCALE_YZ";
		}
	      else
		{
		  uAxisOrig.setValue(0, 0, 1);
		  vAxisOrig.setValue(1, 0, 0);
		  // cerr << "SCALE_ZX";
		}
	      // cerr << endl;
	      GleemV3f uAxis, vAxis;
	      rotation.rotateVector(uAxisOrig, uAxis);
	      rotation.rotateVector(vAxisOrig, vAxis);
	      GleemV3f normal;
	      GleemV3f::cross(uAxis, vAxis, normal);
	      scaleAxisPlane.setNormalAndUV(normal, uAxis, vAxis);
	      // We need to be able to constrain the scaling to be
	      // nonnegative.
	      GleemV3f newOrigin;
	      GleemV2f foo;
	      scaleAxisPlane.projectPoint(translation, newOrigin, foo);
	      scaleAxisPlane.setOrigin(newOrigin);
	      scaleAxisOffset = hit.intPt - newOrigin;
	      // Now project intersection point onto plane
	      GleemV3f bar;
	      scaleAxisPlane.projectPoint(hit.intPt, bar, scaleAxisOrigUV);
	      // Put the plane back where it was
	      scaleAxisPlane.setOrigin(hit.intPt);
	      origScale = scale;
	    }
	  else
	    {
	      dragState = SCALE_XYZ;
	      scaleXYZLine.setPoint(hit.intPt);
	      GleemV3f scaleDiffVec = hit.intPt - translation;
	      scaleXYZLine.setDirection(scaleDiffVec);
	      origScale = scale;
	      origScaleLen = scaleDiffVec.length();
	    }
	  info.geometry->highlight();
	  draggedGeometry.push_back(info.geometry);
	  return;
	}
    }

  assert(0);
}

void
HandleBoxManip::drag(const GleemV3f &rayStart,
		     const GleemV3f &rayDirection)
{
  if (dragState == TRANSLATE)
    {
      // Algorithm: Find intersection of ray with dragPlane. Add
      // dragOffset to this point to get new translation.
      GleemV3f intPt;
      float t;
      if (dragPlane.intersectRay(rayStart,
				 rayDirection,
				 intPt,
				 t) == false)
	// Ray is parallel to plane. Punt.
	return;
      translation = intPt + dragOffset;
      recalc();
    }
  else if (dragState == ROTATE)
    {
      GleemV3f intPt;
      float t;
      GleemV2f uvCoords;
      if (rotatePlane.intersectRay(rayStart,
				   rayDirection,
				   intPt,
				   t,
				   uvCoords) == false)
	// Ray is parallel to plane. Punt.
	return;
      // Compute offset rotation angle
      GleemRot offsetRot(rotatePlane.getNormal(),
			atan2f(uvCoords[1], uvCoords[0]) - startAngle);
      GleemRot::mult(offsetRot, startRot, rotation);
      recalc();
    }
  else if (dragState == SCALE_XYZ)
    {
      GleemV3f closestPt;
      bool gotPt = scaleXYZLine.closestPointToRay(rayStart,
						  rayDirection,
						  closestPt);
      if (gotPt)
	{
	  // How far have we moved?
	  // Clamp scale to be positive.
	  GleemV3f newDiffVec = closestPt - translation;
	  if (newDiffVec.dot(scaleXYZLine.getDirection()) < 0)
	    scale.setValue(0, 0, 0);
	  else
	    {
	      float scaleChange = newDiffVec.length() / origScaleLen;
	      scale = origScale * scaleChange;
	    }
	  recalc();	  
	}
    }
  else if (dragState == SCALE_SINGLE_AXIS)
    {
      GleemV3f intPt;
      float t;
      GleemV2f uvCoords;
      if (scaleAxisPlane.intersectRay(rayStart, rayDirection,
				      intPt, t, uvCoords))
	{
	  GleemV2f faceCenteredUVCoords;
	  GleemV3f foo;
	  scaleAxisPlane.projectPoint(intPt + scaleAxisOffset,
				      foo, faceCenteredUVCoords);
	  if ((GLEEM_SGN(faceCenteredUVCoords[0]) ==
	       GLEEM_SGN(scaleAxisOrigUV[0])) &&
	      (GLEEM_SGN(faceCenteredUVCoords[1]) ==
	       GLEEM_SGN(scaleAxisOrigUV[1])))
	    {
	      if (faceCenteredUVCoords[0] < 0)
		uvCoords[0] *= -1;
	      if (faceCenteredUVCoords[1] < 0)
		uvCoords[1] *= -1;
	      GleemV3f scaleVec(0, 0, 0);
	      if (fabs(uvCoords[0]) > fabs(uvCoords[1]))
		{
		  if (scaleAxes == SCALE_XY)
		    scaleVec[0] = uvCoords[0];
		  else if (scaleAxes == SCALE_YZ)
		    scaleVec[1] = uvCoords[0];
		  else
		    scaleVec[2] = uvCoords[0];
		}
	      else
		{
		  if (scaleAxes == SCALE_XY)
		    scaleVec[1] = uvCoords[1];
		  else if (scaleAxes == SCALE_YZ)
		    scaleVec[2] = uvCoords[1];
		  else
		    scaleVec[0] = uvCoords[1];
		}
	      scaleVec[0] /= geometryScale[0];
	      scaleVec[1] /= geometryScale[1];
	      scaleVec[2] /= geometryScale[2];
	      scale = origScale + scaleVec;
	      // This shouldn't be necessary anymore
	      /*
	      if (scale[0] < 0)
		scale[0] = 0;
	      if (scale[1] < 0)
		scale[1] = 0;
	      if (scale[2] < 0)
		scale[2] = 0;
	      */
	    }
	  else
	    {
	      if (fabs(uvCoords[0]) > fabs(uvCoords[1]))
		{
		  if (scaleAxes == SCALE_XY)
		    scale[0] = 0;
		  else if (scaleAxes == SCALE_YZ)
		    scale[1] = 0;
		  else
		    scale[2] = 0;
		}
	      else
		{
		  if (scaleAxes == SCALE_XY)
		    scale[1] = 0;
		  else if (scaleAxes == SCALE_YZ)
		    scale[2] = 0;
		  else
		    scale[0] = 0;
		}
	      
	    }
	  recalc();
	}
    }
  else
    {
      cerr << "HandleBoxManip::drag: ERROR: Unexpected drag state" << endl;
      return;
    }
  Manip::drag(rayStart, rayDirection);
}

void
HandleBoxManip::makeInactive()
{
  dragState = INACTIVE;
  for (int i = 0; i < draggedGeometry.size(); i++)
    draggedGeometry[i]->clearHighlight();
  draggedGeometry.erase(draggedGeometry.begin(),
			draggedGeometry.end());
}

void
HandleBoxManip::deleteGeometry()
{
  int i;
  for (i = 0; i < 12; i++)
    {
      if (lineSegs[i] != NULL)
	{
	  delete lineSegs[i];
	  lineSegs[i] = NULL;
	}
    }
  for (i = 0; i < faces.size(); i++)
    {
      FaceInfo &foo = faces[i];
      delete foo.centerSquare;
    }
  faces.erase(faces.begin(), faces.end());
  for (i = 0; i < rotateHandles.size(); i++)
    {
      RotateHandleInfo &foo = rotateHandles[i];
      delete foo.geometry;
    }
  rotateHandles.erase(rotateHandles.begin(), rotateHandles.end());
  for (i = 0; i < scaleHandles.size(); i++)
    {
      ScaleHandleInfo &foo = scaleHandles[i];
      delete foo.geometry;
    }
  scaleHandles.erase(scaleHandles.begin(), scaleHandles.end());
}

void
HandleBoxManip::createGeometry()
{
  deleteGeometry();

  //
  // Lines
  //

  // Top face:
  // Front line
  lineSegs[0] = createLineSeg(GleemV3f(0, 1, 1),
			      GleemV3f(1, 0, 0),
			      GleemV3f(0, 1, 0));
  // Left line
  lineSegs[1] = createLineSeg(GleemV3f(-1, 1, 0),
			      GleemV3f(0, 0, 1),
			      GleemV3f(0, 1, 0));
  // Back line
  lineSegs[2] = createLineSeg(GleemV3f(0, 1, -1),
			      GleemV3f(1, 0, 0),
			      GleemV3f(0, 1, 0));
  // Right line
  lineSegs[3] = createLineSeg(GleemV3f(1, 1, 0),
			      GleemV3f(0, 0, 1),
			      GleemV3f(0, 1, 0));
  // Middle segments:
  // Front left
  lineSegs[4] = createLineSeg(GleemV3f(-1, 0, 1),
			      GleemV3f(0, -1, 0),
			      GleemV3f(1, 0, 0));
  // Back left
  lineSegs[5] = createLineSeg(GleemV3f(-1, 0, -1),
			      GleemV3f(0, -1, 0),
			      GleemV3f(1, 0, 0));
  // Back right
  lineSegs[6] = createLineSeg(GleemV3f(1, 0, -1),
			      GleemV3f(0, -1, 0),
			      GleemV3f(1, 0, 0));
  // Front right
  lineSegs[7] = createLineSeg(GleemV3f(1, 0, 1),
			      GleemV3f(0, -1, 0),
			      GleemV3f(1, 0, 0));
  // Bottom face:
  // Front line
  lineSegs[8] = createLineSeg(GleemV3f(0, -1, 1),
			      GleemV3f(1, 0, 0),
			      GleemV3f(0, 1, 0));
  // Left line
  lineSegs[9] = createLineSeg(GleemV3f(-1, -1, 0),
			      GleemV3f(0, 0, 1),
			      GleemV3f(0, 1, 0));
  // Back line
  lineSegs[10] = createLineSeg(GleemV3f(0, -1, -1),
			       GleemV3f(1, 0, 0),
			       GleemV3f(0, 1, 0));
  // Right line
  lineSegs[11] = createLineSeg(GleemV3f(1, -1, 0),
			       GleemV3f(0, 0, 1),
			       GleemV3f(0, 1, 0));

  //
  // Faces
  //

  FaceInfo info;
  // Front face (index 0)
  info.origNormal.setValue(0, 0, 1);
  info.centerSquare =
    createFace(info.origNormal, info.origNormal, GleemV3f(0, 1, 0));
  info.lineSegs[0] = lineSegs[0];
  info.lineSegs[1] = lineSegs[4];
  info.lineSegs[2] = lineSegs[7];
  info.lineSegs[3] = lineSegs[8];
  info.scaleAxes = SCALE_XY;
  faces.push_back(info);
  // Right face (index 1)
  info.origNormal.setValue(1, 0, 0);
  info.centerSquare =
    createFace(info.origNormal, info.origNormal, GleemV3f(0, 1, 0));
  info.lineSegs[0] = lineSegs[3];
  info.lineSegs[1] = lineSegs[6];
  info.lineSegs[2] = lineSegs[7];
  info.lineSegs[3] = lineSegs[11];
  info.scaleAxes = SCALE_YZ;
  faces.push_back(info);
  // Back face (index 2)
  info.origNormal.setValue(0, 0, -1);
  info.centerSquare =
    createFace(info.origNormal, info.origNormal, GleemV3f(0, 1, 0));
  info.lineSegs[0] = lineSegs[2];
  info.lineSegs[1] = lineSegs[5];
  info.lineSegs[2] = lineSegs[6];
  info.lineSegs[3] = lineSegs[10];
  info.scaleAxes = SCALE_XY;
  faces.push_back(info);
  // Left face (index 3)
  info.origNormal.setValue(-1, 0, 0);
  info.centerSquare =
    createFace(info.origNormal, info.origNormal, GleemV3f(0, 1, 0));
  info.lineSegs[0] = lineSegs[1];
  info.lineSegs[1] = lineSegs[4];
  info.lineSegs[2] = lineSegs[5];
  info.lineSegs[3] = lineSegs[9];
  info.scaleAxes = SCALE_YZ;
  faces.push_back(info);
  // Top face (index 4)
  info.origNormal.setValue(0, 1, 0);
  info.centerSquare =
    createFace(info.origNormal, info.origNormal, GleemV3f(0, 0, -1));
  info.lineSegs[0] = lineSegs[0];
  info.lineSegs[1] = lineSegs[1];
  info.lineSegs[2] = lineSegs[2];
  info.lineSegs[3] = lineSegs[3];
  info.scaleAxes = SCALE_ZX;
  faces.push_back(info);
  // Bottom face (index 5)
  info.origNormal.setValue(0, -1, 0);
  info.centerSquare =
    createFace(info.origNormal, info.origNormal, GleemV3f(0, 0, 1));
  info.lineSegs[0] = lineSegs[8];
  info.lineSegs[1] = lineSegs[9];
  info.lineSegs[2] = lineSegs[10];
  info.lineSegs[3] = lineSegs[11];
  info.scaleAxes = SCALE_ZX;
  faces.push_back(info);

  //
  // Rotation handles
  //

  RotateHandleInfo rotInfo;
  // Front handle. Rotates about top/bottom and left/right faces.
  // Maintain references to top and right faces.
  rotInfo.faceIdx0 = 4;
  rotInfo.faceIdx1 = 1;
  rotInfo.geometry = createRotateHandle(GleemV3f(0, 0, 1));
  rotateHandles.push_back(rotInfo);
  // Right handle. Rotates about top/bottom and front/back faces.
  // Maintain references to top and front faces.
  rotInfo.faceIdx0 = 4;
  rotInfo.faceIdx1 = 0;
  rotInfo.geometry = createRotateHandle(GleemV3f(1, 0, 0));
  rotateHandles.push_back(rotInfo);
  // Back handle. Rotates about top/bottom and left/right faces.
  // Maintain references to top and right faces.
  rotInfo.faceIdx0 = 4;
  rotInfo.faceIdx1 = 1;
  rotInfo.geometry = createRotateHandle(GleemV3f(0, 0, -1));
  rotateHandles.push_back(rotInfo);
  // Left handle. Rotates about top/bottom and front/back faces.
  // Maintain references to top and front faces.
  rotInfo.faceIdx0 = 4;
  rotInfo.faceIdx1 = 0;
  rotInfo.geometry = createRotateHandle(GleemV3f(-1, 0, 0));
  rotateHandles.push_back(rotInfo);
  // Top handle. Rotates about front/back and left/right faces.
  // Maintain references to front and right faces.
  rotInfo.faceIdx0 = 0;
  rotInfo.faceIdx1 = 1;
  rotInfo.geometry = createRotateHandle(GleemV3f(0, 1, 0));
  rotateHandles.push_back(rotInfo);
  // Bottom handle. Rotates about front/back and left/right faces.
  // Maintain references to front and right faces.
  rotInfo.faceIdx0 = 0;
  rotInfo.faceIdx1 = 1;
  rotInfo.geometry = createRotateHandle(GleemV3f(0, -1, 0));
  rotateHandles.push_back(rotInfo);

  // Scale handles
  ScaleHandleInfo scaleInfo;
  // Top right front (order: front right top)
  scaleInfo.geometry = createScaleHandle(GleemV3f(1, 1, 1));
  scaleInfo.faceIndices[0] = 0;
  scaleInfo.faceIndices[1] = 1;
  scaleInfo.faceIndices[2] = 4;
  scaleHandles.push_back(scaleInfo);
  // Top right back (order: right back top)
  scaleInfo.geometry = createScaleHandle(GleemV3f(1, 1, -1));
  scaleInfo.faceIndices[0] = 1;
  scaleInfo.faceIndices[1] = 2;
  scaleInfo.faceIndices[2] = 4;
  scaleHandles.push_back(scaleInfo);
  // Bottom right front (order: front right bottom)
  scaleInfo.geometry = createScaleHandle(GleemV3f(1, -1, 1));
  scaleInfo.faceIndices[0] = 0;
  scaleInfo.faceIndices[1] = 1;
  scaleInfo.faceIndices[2] = 5;
  scaleHandles.push_back(scaleInfo);
  // Bottom right back (order: right back bottom)
  scaleInfo.geometry = createScaleHandle(GleemV3f(1, -1, -1));
  scaleInfo.faceIndices[0] = 1;
  scaleInfo.faceIndices[1] = 2;
  scaleInfo.faceIndices[2] = 5;
  scaleHandles.push_back(scaleInfo);
  // Top left front (order: front left top)
  scaleInfo.geometry = createScaleHandle(GleemV3f(-1, 1, 1));
  scaleInfo.faceIndices[0] = 0;
  scaleInfo.faceIndices[1] = 3;
  scaleInfo.faceIndices[2] = 4;
  scaleHandles.push_back(scaleInfo);
  // Top left back (order: back left top)
  scaleInfo.geometry = createScaleHandle(GleemV3f(-1, 1, -1));
  scaleInfo.faceIndices[0] = 2;
  scaleInfo.faceIndices[1] = 3;
  scaleInfo.faceIndices[2] = 4;
  scaleHandles.push_back(scaleInfo);
  // Bottom left front (order: front left bottom)
  scaleInfo.geometry = createScaleHandle(GleemV3f(-1, -1, 1));
  scaleInfo.faceIndices[0] = 0;
  scaleInfo.faceIndices[1] = 3;
  scaleInfo.faceIndices[2] = 5;
  scaleHandles.push_back(scaleInfo);
  // Bottom left back (order: back left bottom)
  scaleInfo.geometry = createScaleHandle(GleemV3f(-1, -1, -1));
  scaleInfo.faceIndices[0] = 2;
  scaleInfo.faceIndices[1] = 3;
  scaleInfo.faceIndices[2] = 5;
  scaleHandles.push_back(scaleInfo);
}

void
HandleBoxManip::recalc()
{
  // Construct local to world transform for geometry.
  // Scale, Rotation, Translation. Since we're right multiplying
  // column vectors, the actual matrix composed is TRS.
  GleemMat4f scaleMat;
  GleemMat4f rotMat;
  GleemMat4f xlateMat;
  GleemMat4f tmpMat;
  scaleMat.makeIdent();
  scaleMat[0][0] = scale[0] * geometryScale[0];
  scaleMat[1][1] = scale[1] * geometryScale[1];
  scaleMat[2][2] = scale[2] * geometryScale[2];
  rotMat.makeIdent();
  rotMat.setRotation(rotation);
  xlateMat.makeIdent();
  xlateMat[0][3] = translation[0];
  xlateMat[1][3] = translation[1];
  xlateMat[2][3] = translation[2];
  GleemMat4f::mult(xlateMat, rotMat, tmpMat);
  GleemMat4f::mult(tmpMat, scaleMat, xform);
  int i;
  for (i = 0; i < 12; i++)
    lineSegs[i]->setTransform(xform);
  for (i = 0; i < faces.size(); i++)
    {
      FaceInfo &face = faces[i];
      face.centerSquare->setTransform(xform);
      xform.xformDir(face.origNormal, face.normal);
      face.normal.normalize();
      
      RotateHandleInfo &rotInfo = rotateHandles[i];
      rotInfo.geometry->setTransform(xform);
    }
  for (i = 0; i < scaleHandles.size(); i++)
    {
      scaleHandles[i].geometry->setTransform(xform);
    }
}

ManipPart *
HandleBoxManip::createLineSeg(const GleemV3f &translation,
			      const GleemV3f &xAxis,
			      const GleemV3f &yAxis)
{
  ManipPartTransform *xform = new ManipPartTransform(this);
  ManipPartLineSeg *lineSeg = new ManipPartLineSeg(this);
  xform->addPart(lineSeg);
  GleemMat4f offset;
  offset.makeIdent();
  GleemV3f zAxis;
  GleemV3f::cross(xAxis, yAxis, zAxis);
  offset[0][0] = xAxis[0];
  offset[1][0] = xAxis[1];
  offset[2][0] = xAxis[2];
  offset[0][1] = yAxis[0];
  offset[1][1] = yAxis[1];
  offset[2][1] = yAxis[2];
  offset[0][2] = zAxis[0];
  offset[1][2] = zAxis[1];
  offset[2][2] = zAxis[2];
  offset[0][3] = translation[0];
  offset[1][3] = translation[1];
  offset[2][3] = translation[2];
  xform->setOffsetTransform(offset);
  return xform;
}

ManipPart *
HandleBoxManip::createFace(const GleemV3f &translation,
			   const GleemV3f &normal,
			   const GleemV3f &up)
{
  ManipPartTransform *xform = new ManipPartTransform(this);
  ManipPartSquare *square = new ManipPartSquare(this);
  square->setVisible(false);
  xform->addPart(square);
  GleemMat4f offset;
  offset.makeIdent();
  GleemV3f right;
  GleemV3f::cross(up, normal, right);
  offset[0][0] = right[0];
  offset[1][0] = right[1];
  offset[2][0] = right[2];
  offset[0][1] = up[0];
  offset[1][1] = up[1];
  offset[2][1] = up[2];
  offset[0][2] = normal[0];
  offset[1][2] = normal[1];
  offset[2][2] = normal[2];
  offset[0][3] = translation[0];
  offset[1][3] = translation[1];
  offset[2][3] = translation[2];
  xform->setOffsetTransform(offset);
  return xform;
}

ManipPart *
HandleBoxManip::createRotateHandle(const GleemV3f &direction)
{
  ManipPartCube *handle = new ManipPartCube(this);
  GleemMat4f offset;
  offset.makeIdent();
  offset[0][0] = offset[1][1] = offset[2][2] = 0.1f;
  offset.setTranslation(2.0f * direction);
  ManipPartTransform *xform = new ManipPartTransform(this);
  xform->addPart(handle);
  xform->setOffsetTransform(offset);
  return xform;
}

ManipPart *
HandleBoxManip::createScaleHandle(const GleemV3f &position)
{
  ManipPartCube *handle = new ManipPartCube(this);
  GleemMat4f offset;
  offset.makeIdent();
  offset[0][0] = offset[1][1] = offset[2][2] = 0.1f;
  offset.setTranslation(position);
  ManipPartTransform *xform = new ManipPartTransform(this);
  xform->addPart(handle);
  xform->setOffsetTransform(offset);
  return xform;
}
