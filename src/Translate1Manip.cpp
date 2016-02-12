/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include "../include/Translate1Manip.h"
#include "../include/ManipPartTwoWayArrow.h"
#include "../include/MathUtil.h"

GLEEM_USE_NAMESPACE

Translate1Manip::Translate1Manip()
{
  geometry = new ManipPartTwoWayArrow(this);
  iOwnGeometry = true;
  translation.setValue(0, 0, 0);
  axis.setValue(1, 0, 0);
  scale.setValue(1, 1, 1);
}

Translate1Manip::~Translate1Manip()
{
  if (iOwnGeometry)
    delete geometry;
}

void
Translate1Manip::setTranslation(const GleemV3f &translation)
{
  this->translation = translation;
  recalc();
}

const GleemV3f &
Translate1Manip::getTranslation() const
{
  return translation;
}

void
Translate1Manip::setAxis(const GleemV3f &axis)
{
  this->axis = axis;
  this->axis.normalize();
  recalc();
}

const GleemV3f &
Translate1Manip::getAxis() const
{
  return axis;
}

void
Translate1Manip::setScale(const GleemV3f &scale)
{
  this->scale = scale;
  recalc();
}

const GleemV3f &
Translate1Manip::getScale() const
{
  return scale;
}

void
Translate1Manip::replaceGeometry(ManipPart *newGeom)
{
  if (iOwnGeometry)
    {
      delete geometry;
      iOwnGeometry = false;
    }
  geometry = newGeom;
  if (geometry == NULL)
    {
      geometry = new ManipPartTwoWayArrow(this);
      iOwnGeometry = true;
    }
  recalc();
}

void
Translate1Manip::render()
{
  geometry->render();
}

void
Translate1Manip::intersectRay(const GleemV3f &rayStart,
			      const GleemV3f &rayDirection,
			      vector<HitPoint> &results)
{
  geometry->intersectRay(rayStart, rayDirection, results);
}

void
Translate1Manip::highlight(const HitPoint &hit)
{
  if (hit.manipPart != geometry)
    {
      cerr << "gleem::Translate1Manip::highlight: ERROR: my old geometry "
	   << "disappeared; how did this happen?" << endl;
      return;
    }
  geometry->highlight();
}

void
Translate1Manip::clearHighlight()
{
  geometry->clearHighlight();
}

void
Translate1Manip::makeActive(const HitPoint &hit)
{
  //  cerr << "Translate1Manip::makeActive()" << endl;
  geometry->highlight();
  dragLine.setDirection(axis);
  dragLine.setPoint(hit.intPt);
  GleemV3f::sub(translation, hit.intPt, dragOffset);
}

void
Translate1Manip::drag(const GleemV3f &rayStart,
		      const GleemV3f &rayDirection)
{
  // Algorithm: Find closest point of ray to dragLine. Add dragOffset
  // to this point to get new translation.
  GleemV3f closestPoint;
  if (dragLine.closestPointToRay(rayStart,
				 rayDirection,
				 closestPoint) == false)
    // Drag axis is parallel to ray. Punt.
    return;
  translation = closestPoint + dragOffset;
  //  cerr << "Dragged to "
  //       << translation[0] << ", "
  //       << translation[1] << ", "
  //       << translation[2] << endl;
  recalc();
  Manip::drag(rayStart, rayDirection);
}

void
Translate1Manip::makeInactive()
{
  geometry->clearHighlight();
}

void
Translate1Manip::recalc()
{
  // Construct local to world transform for geometry.
  // Scale, Rotation, Translation. Since we're right multiplying
  // column vectors, the actual matrix composed is TRS.
  GleemMat4f scaleMat;
  GleemMat4f rotMat;
  GleemMat4f xlateMat;
  GleemMat4f tmpMat;
  scaleMat.makeIdent();
  scaleMat[0][0] = scale[0];
  scaleMat[1][1] = scale[1];
  scaleMat[2][2] = scale[2];
  // Perpendiculars
  GleemV3f p0, p1;
  MathUtil::makePerpendicular(axis, p0);
  GleemV3f::cross(axis, p0, p1);
  // axis, p0, p1 correspond to x, y, z
  p0.normalize();
  p1.normalize();
  rotMat.makeIdent();
  rotMat[0][0] = axis[0];
  rotMat[1][0] = axis[1];
  rotMat[2][0] = axis[2];
  rotMat[0][1] = p0[0];
  rotMat[1][1] = p0[1];
  rotMat[2][1] = p0[2];
  rotMat[0][2] = p1[0];
  rotMat[1][2] = p1[1];
  rotMat[2][2] = p1[2];
  xlateMat.makeIdent();
  xlateMat[0][3] = translation[0];
  xlateMat[1][3] = translation[1];
  xlateMat[2][3] = translation[2];
  GleemMat4f::mult(xlateMat, rotMat, tmpMat);
  GleemMat4f::mult(tmpMat, scaleMat, xform);
  geometry->setTransform(xform);
}
