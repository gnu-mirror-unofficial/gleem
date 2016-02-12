/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_PLANE_H
#define _GLEEM_PLANE_H

#include <gleem/Namespace.h>
#include <gleem/GleemDLL.h>
#include <gleem/Util.h>
#include <gleem/Linalg.h>

GLEEM_ENTER_NAMESPACE

GLEEM_INTERNAL class GLEEMDLL Plane
{
public:
  /** Default constructor initializes normal to (0, 1, 0) and point to
      (0, 0, 0) */
  Plane();

  /** Sets all parameters of plane. Plane has normal <b>normal</b> and
      goes through the point <b>point</b>. Normal does not need to be
      unit length but must not be the zero vector. */
  Plane(const GleemV3f &normal, const GleemV3f &point);

  /** Setter does some work to maintain internal caches. Normal does
      not need to be unit length but must not be the zero vector. */
  void setNormal(const GleemV3f &normal);
  /** Normal is normalized internally, so <b>normal</b> is not
      necessarily equal to <code>plane.setNormal(normal);
      plane.getNormal();</code> */
  const GleemV3f &getNormal() const;

  /** Setter does some work to maintain internal caches */
  void setPoint(const GleemV3f &point);
  const GleemV3f &getPoint() const;

  /** Project a point onto the plane */
  void projectPoint(const GleemV3f &point,
		    GleemV3f &projPt) const;

  /** Intersect a ray with the plane. Returns true if intersection occurred, false
      otherwise. This is a two-sided ray cast. */
  bool intersectRay(const GleemV3f &rayStart,
		    const GleemV3f &rayDirection,
		    GleemV3f &intPt,
		    float &t) const;

private:
  /** Normalized */
  GleemV3f normal;
  GleemV3f point;
  /** Constant for faster projection and intersection */
  float c;
  void recalc();
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_PLANE_H
