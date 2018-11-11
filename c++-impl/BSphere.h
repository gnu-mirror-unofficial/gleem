/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_BSPHERE_H
#define _GLEEM_BSPHERE_H

#include <gleem/Namespace.h>
#include <gleem/GleemDLL.h>
#include <gleem/Util.h>
#include <gleem/Linalg.h>

GLEEM_ENTER_NAMESPACE

/** Class which represents a bounding sphere. This is a public class
    because the ExaminerViewer needs clients to supply one. */

class GLEEMDLL BSphere
{
public:
  /** Default constructor creates a sphere with center (0, 0, 0) and
      radius 0 */
  BSphere();
  BSphere(const GleemV3f &center, float radius);

  /** Re-initialize this sphere to center (0, 0, 0) and radius 0 */
  void makeEmpty();

  void setCenter(const GleemV3f &center);
  const GleemV3f &getCenter() const;

  void setRadius(float radius);
  float getRadius() const;

  void setValue(const GleemV3f &center, float radius);
  /** Returns radius and mutates passed "center" vector */
  float getValue(GleemV3f &center) const;

  /** Mutate this sphere to encompass both itself and the
      argument. Ignores zero-size arguments. */
  void extendBy(const BSphere &arg);

  /** Intersect a ray with the sphere. Returns true if intersection
      occurred, false otherwise. This is a one-sided ray cast. Returns
      number of intersections which occurred. If one occurred then
      only intPt0 and t0 will be mutated. */
  bool intersectRay(const GleemV3f &rayStart,
		    const GleemV3f &rayDirection,
		    int &numIntersections,
		    GleemV3f &intPt0,
		    float &t0,
		    GleemV3f &intPt1,
		    float &t1) const;

private:
  GleemV3f center;
  float radius;
  float radSq;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_BSPHERE_H
