/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <gleem/Plane.h>

GLEEM_USE_NAMESPACE

Plane::Plane()
{
  normal.setValue(0, 1, 0);
  point.setValue(0, 0, 0);
}

Plane::Plane(const GleemV3f &normal, const GleemV3f &point)
{
  this->normal = normal;
  this->normal.normalize();
  this->point = point;
  recalc();
}

void
Plane::setNormal(const GleemV3f &normal)
{
  this->normal = normal;
  this->normal.normalize();
  recalc();
}

const GleemV3f &
Plane::getNormal() const
{
  return normal;
}

void
Plane::setPoint(const GleemV3f &point)
{
  this->point = point;
  recalc();
}

const GleemV3f &
Plane::getPoint() const
{
  return point;
}

void
Plane::projectPoint(const GleemV3f &pt,
		    GleemV3f &projPt) const
{
  projPt = pt - (normal.dot(point) - c) * normal;
}

bool
Plane::intersectRay(const GleemV3f &rayStart,
		    const GleemV3f &rayDirection,
		    GleemV3f &intPt,
		    float &t) const
{
  float denom = normal.dot(rayDirection);
  if (denom == 0)
    return false;
  t = (c - normal.dot(rayStart)) / denom;
  intPt = rayStart + t * rayDirection;
  return true;
}

void
Plane::recalc()
{
  c = normal.dot(point);
}
