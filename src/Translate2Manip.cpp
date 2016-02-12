/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include "../include/Translate2Manip.h"
#include "../include/ManipPartTwoWayArrow.h"
#include "../include/ManipPartTransform.h"
#include "../include/MathUtil.h"

GLEEM_USE_NAMESPACE

Translate2Manip::Translate2Manip()
{
  translation.setValue(0, 0, 0);
  normal.setValue(0, 1, 0);
  scale.setValue(1, 1, 1);
  createGeometry();
  recalc();
}

Translate2Manip::~Translate2Manip()
{
}

void
Translate2Manip::setTranslation(const GleemV3f &translation)
{
  this->translation = translation;
  recalc();
}

const GleemV3f &
Translate2Manip::getTranslation() const
{
  return translation;
}

void
Translate2Manip::setNormal(const GleemV3f &normal)
{
  this->normal = normal;
  this->normal.normalize();
  recalc();
}

const GleemV3f &
Translate2Manip::getNormal() const
{
  return normal;
}

void
Translate2Manip::setScale(const GleemV3f &scale)
{
  this->scale = scale;
  recalc();
}

const GleemV3f &
Translate2Manip::getScale() const
{
  return scale;
}

void
Translate2Manip::replaceGeometry(ManipPart *newGeom)
{
  if (iOwnGeometry)
    {
      delete geometry;
      iOwnGeometry = false;
    }
  geometry = newGeom;
  if (geometry == NULL)
    createGeometry();
  recalc();
}

void
Translate2Manip::render()
{
  geometry->render();
}

void
Translate2Manip::intersectRay(const GleemV3f &rayStart,
			      const GleemV3f &rayDirection,
			      vector<HitPoint> &results)
{
  geometry->intersectRay(rayStart, rayDirection, results);
}

void
Translate2Manip::highlight(const HitPoint &hit)
{
  if (hit.manipPart != geometry)
    {
      cerr << "gleem::Translate2Manip::highlight: ERROR: my old geometry "
	   << "disappeared; how did this happen?" << endl;
      return;
    }
  geometry->highlight();
}

void
Translate2Manip::clearHighlight()
{
  geometry->clearHighlight();
}

void
Translate2Manip::makeActive(const HitPoint &hit)
{
  geometry->highlight();
  dragPlane.setNormal(normal);
  dragPlane.setPoint(hit.intPt);
  GleemV3f::sub(translation, hit.intPt, dragOffset);
}

void
Translate2Manip::drag(const GleemV3f &rayStart,
		      const GleemV3f &rayDirection)
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
  Manip::drag(rayStart, rayDirection);
}

void
Translate2Manip::makeInactive()
{
  geometry->clearHighlight();
}

void
Translate2Manip::createGeometry()
{
  ManipPartGroup *group = new ManipPartGroup(this);
  ManipPartTwoWayArrow *arrow1 = new ManipPartTwoWayArrow(this);
  group->addPart(arrow1);
  ManipPartTransform *xform = new ManipPartTransform(this);
  GleemMat4f rotMat;
  rotMat.makeIdent();
  rotMat[0][0] = 0;
  rotMat[1][0] = 0;
  rotMat[2][0] = -1;
  rotMat[0][2] = 1;
  rotMat[1][2] = 0;
  rotMat[2][2] = 0;
  xform->setOffsetTransform(rotMat);
  ManipPartTwoWayArrow *arrow2 = new ManipPartTwoWayArrow(this);
  xform->addPart(arrow2);
  group->addPart(xform);
  geometry = group;
  iOwnGeometry = true;
}

void
Translate2Manip::recalc()
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
  MathUtil::makePerpendicular(normal, p0);
  GleemV3f::cross(normal, p0, p1);
  // p1, normal, p0 correspond to x, y, z
  p0.normalize();
  p1.normalize();
  rotMat.makeIdent();
  rotMat[0][0] = p1[0];
  rotMat[1][0] = p1[1];
  rotMat[2][0] = p1[2];
  rotMat[0][1] = normal[0];
  rotMat[1][1] = normal[1];
  rotMat[2][1] = normal[2];
  rotMat[0][2] = p0[0];
  rotMat[1][2] = p0[1];
  rotMat[2][2] = p0[2];
  xlateMat.makeIdent();
  xlateMat[0][3] = translation[0];
  xlateMat[1][3] = translation[1];
  xlateMat[2][3] = translation[2];
  GleemMat4f::mult(xlateMat, rotMat, tmpMat);
  GleemMat4f::mult(tmpMat, scaleMat, xform);
  geometry->setTransform(xform);
}
