/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <iostream.h>
#include <gleem/Line.h>

GLEEM_USE_NAMESPACE

Line::Line()
{
  point.setValue(0, 0, 0);
  direction.setValue(1, 0, 0);
  recalc();
}

Line::Line(const GleemV3f &direction,
	   const GleemV3f &point)
{
  this->direction = direction;
  this->direction.normalize();
  this->point = point;
  recalc();
}

void
Line::setDirection(const GleemV3f &direction)
{
  this->direction = direction;
  this->direction.normalize();
  recalc();
}

const GleemV3f &
Line::getDirection() const
{
  return direction;
}

void
Line::setPoint(const GleemV3f &point)
{
  this->point = point;
  recalc();
}

const GleemV3f &
Line::getPoint() const
{
  return point;
}

void
Line::projectPoint(const GleemV3f &pt,
		   GleemV3f &projPt) const
{
  projPt = (direction.dot(pt) * direction) + alongVec;
}
		   
bool
Line::closestPointToRay(const GleemV3f &rayStart,
			const GleemV3f &rayDirection,
			GleemV3f &closestPoint)
{
  // Line 1 is this one. Line 2 is the incoming one.
  GleemMat2f A;
  A[0][0] = -direction.lengthSquared();
  A[1][1] = -rayDirection.lengthSquared();
  A[0][1] = direction.dot(rayDirection);
  A[1][0] = A[0][1];
  if (A.invert() == false)
    return false;
  GleemV2f x, b;
  b[0] = point.dot(direction) - rayStart.dot(direction);
  b[1] = rayStart.dot(rayDirection) - point.dot(rayDirection);
  A.xformVec(b, x);
  if (x[1] < 0)
    // Means that ray start is closest point to this line
    closestPoint = rayStart;
  else
    closestPoint = point + x[0] * direction;
  return true;
}

void
Line::recalc()
{
  float denom = direction.lengthSquared();
  if (denom == 0.0f)
    {
      cerr << "gleem::Line::recalc: ERROR: direction was the zero vector "
	   << "(not allowed)" << endl;
      return;
    }
  alongVec = point - (point.dot(direction) * direction);
}
