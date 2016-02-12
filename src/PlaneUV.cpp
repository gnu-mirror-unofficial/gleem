/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <cmath>
#include "../include/PlaneUV.h"
#include "../include/MathUtil.h"

GLEEM_USE_NAMESPACE

PlaneUV::PlaneUV()
{
  setEverything(GleemV3f(0, 1, 0),
		GleemV3f(0, 0, 0),
		GleemV3f(1, 0, 0),
		GleemV3f(0, 0, -1));
}

PlaneUV::PlaneUV(const GleemV3f &normal,
		 const GleemV3f &origin)
{
  setOrigin(origin);
  setNormal(normal);
}

PlaneUV::PlaneUV(const GleemV3f &normal,
		 const GleemV3f &origin,
		 const GleemV3f &uAxis)
{
  setOrigin(origin);
  setNormalAndU(normal, uAxis);
}

PlaneUV::PlaneUV(const GleemV3f &normal,
		 const GleemV3f &origin,
		 const GleemV3f &uAxis,
		 const GleemV3f &vAxis)
{
  setEverything(normal, origin, uAxis, vAxis);
}

void
PlaneUV::setOrigin(const GleemV3f &origin)
{
  this->origin = origin;
}

const GleemV3f &
PlaneUV::getOrigin() const
{
  return origin;
}

void
PlaneUV::setNormalAndUV(const GleemV3f &normal,
			const GleemV3f &uAxis,
			const GleemV3f &vAxis)
{
  setEverything(normal, origin, uAxis, vAxis);
}

void
PlaneUV::setNormal(const GleemV3f &normal)
{
  GleemV3f uAxis, vAxis;
  MathUtil::makePerpendicular(normal, uAxis);
  GleemV3f::cross(normal, uAxis, vAxis);
  setEverything(normal, origin, uAxis, vAxis);
}

void
PlaneUV::setNormalAndU(const GleemV3f &normal,
		       const GleemV3f &uAxis)
{
  GleemV3f vAxis;
  GleemV3f::cross(normal, uAxis, vAxis);
  setEverything(normal, origin, uAxis, vAxis);
}

const GleemV3f &
PlaneUV::getNormal() const
{
  return normal;
}

const GleemV3f &
PlaneUV::getUAxis() const
{
  return uAxis;
}

const GleemV3f &
PlaneUV::getVAxis() const
{
  return vAxis;
}

void
PlaneUV::projectPoint(const GleemV3f &point,
		      GleemV3f &projPt,
		      GleemV2f &uvCoords) const
{
  // Using projPt as a temporary
  GleemV3f::sub(point, origin, projPt);
  float dotp = normal.dot(projPt);
  // Component perpendicular to plane
  GleemV3f tmpDir;
  tmpDir.setValue(normal);
  GleemV3f::scale(tmpDir, dotp, tmpDir);
  GleemV3f::sub(projPt, tmpDir, projPt);
  // Take dot products with basis vectors
  uvCoords.setValue(projPt.dot(uAxis),
		    projPt.dot(vAxis));
  // Add on center to intersection point
  GleemV3f::add(origin, projPt, projPt);
}

bool
PlaneUV::intersectRay(const GleemV3f &rayStart,
		      const GleemV3f &rayDirection,
		      GleemV3f &intPt,
		      float &t,
		      GleemV2f &uvCoords) const
{
  float denom = rayDirection.dot(normal);
  if (denom == 0.0f)
    return false;
  GleemV3f tmpDir;
  GleemV3f::sub(origin, rayStart, tmpDir);
  t = tmpDir.dot(normal) / denom;
  // Find intersection point
  intPt.setValue(rayDirection);
  GleemV3f::scale(intPt, t, intPt);
  GleemV3f::add(intPt, rayStart, intPt);
  // Find UV coords
  GleemV3f::sub(intPt, origin, tmpDir);
  uvCoords.setValue(tmpDir.dot(uAxis), tmpDir.dot(vAxis));
  return true;
}

void
PlaneUV::setEverything(const GleemV3f &normal,
		       const GleemV3f &origin,
		       const GleemV3f &uAxis,
		       const GleemV3f &vAxis)
{
  this->normal.setValue(normal);
  this->origin.setValue(origin);
  this->uAxis.setValue(uAxis);
  this->vAxis.setValue(vAxis);
  this->normal.normalize();
  this->uAxis.normalize();
  this->vAxis.normalize();
}
