/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <assert.h>
#include <BSphere.h>
#include <MathUtil.h>

GLEEM_USE_NAMESPACE

BSphere::BSphere()
{
  makeEmpty();
}

BSphere::BSphere(const GleemV3f &center, float radius)
{
  setValue(center, radius);
}

void
BSphere::makeEmpty()
{
  center.setValue(0, 0, 0);
  radius = radSq = 0;
}

void
BSphere::setCenter(const GleemV3f &center)
{
  this->center.setValue(center);
}

const GleemV3f &
BSphere::getCenter() const
{
  return center;
}

void
BSphere::setRadius(float radius)
{
  this->radius = radius;
  radSq = radius * radius;
}

float
BSphere::getRadius() const
{
  return radius;
}

void
BSphere::setValue(const GleemV3f &center, float radius)
{
  setCenter(center);
  setRadius(radius);
}

float
BSphere::getValue(GleemV3f &center) const
{
  center.setValue(this->center);
  return radius;
}

void
BSphere::extendBy(const BSphere &arg)
{
  if ((radius == 0.0f) || (arg.radius == 0.0f))
    return;
  // FIXME: This algorithm is a quick hack -- minimum bounding
  // sphere of a set of other spheres is a well studied problem, but
  // not by me
  GleemV3f diff = arg.center - center;
  if (diff.lengthSquared() == 0.0f)
  {
    setRadius(GLEEM_MAX2(radius, arg.radius));
    return;
  }
  GleemV3f intPt[4];
  float t[4];
  int numIntersections;
  bool result = intersectRay(center, diff, numIntersections,
			     intPt[0], t[0], intPt[1], t[1]);
  assert(result && (numIntersections == 2));
  result = intersectRay(center, diff, numIntersections,
			intPt[2], t[2], intPt[3], t[3]);
  assert(result && (numIntersections == 2));
  GleemV3f minIntPt = intPt[0], maxIntPt = intPt[0];
  float minT = t[0], maxT = t[0];
  // Find minimum and maximum t values, take associated intersection
  // points, find midpoint and half length of line segment -->
  // center and radius.
  for (int i = 0; i < 4; i++)
  {
    if (t[i] < minT)
    {
      minIntPt = intPt[i];
      minT = t[i];
    }
    else if (t[i] > maxT)
    {
      maxIntPt = intPt[i];
      maxT = t[i];
    }
  }
  // Compute the average -- this is the new center
  GleemV3f::add(minIntPt, maxIntPt, center);
  GleemV3f::scale(center, 0.5f, center);
  // Compute half the length -- this is the radius
  GleemV3f newDiff = minIntPt - maxIntPt;
  setRadius(0.5f * newDiff.length());
}

bool
BSphere::intersectRay(const GleemV3f &rayStart,
		      const GleemV3f &rayDirection,
		      int &numIntersections,
		      GleemV3f &intPt0,
		      float &t0,
		      GleemV3f &intPt1,
		      float &t1) const
{
  // Solve quadratic equation
  float a = rayDirection.lengthSquared();
  if (a == 0.0)
    return false;
  float b = 2.0f * (rayStart.dot(rayDirection) - rayDirection.dot(center));
  GleemV3f tempDiff = center - rayStart;
  float c = tempDiff.lengthSquared() - radSq;
  float disc = b * b - 4 * a * c;
  if (disc < 0.0f)
    return false;
  if (disc == 0.0f)
    numIntersections = 1;
  else
    numIntersections = 2;
  t0 = (0.5f * (-1.0f * b + sqrtf(disc))) / a;
  if (numIntersections == 2)
    t1 = (0.5f * (-1.0f * b - sqrtf(disc))) / a;
  intPt0.setValue(rayDirection);
  GleemV3f::scale(intPt0, t0, intPt0);
  GleemV3f::add(intPt0, rayStart, intPt0);
  if (numIntersections == 2)
  {
    intPt1.setValue(rayDirection);
    GleemV3f::scale(intPt1, t1, intPt1);
    GleemV3f::add(intPt1, rayStart, intPt1);
  }
  return true;
}
