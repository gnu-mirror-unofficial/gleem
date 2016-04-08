/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_RAY_TRIANGLE_INTERSECTION_H
#define _GLEEM_RAY_TRIANGLE_INTERSECTION_H

#include <Namespace.h>
#include <GleemDLL.h>
#include <Util.h>
#include <Linalg.h>

GLEEM_ENTER_NAMESPACE

GLEEM_INTERNAL class GLEEMDLL RayTriangleIntersection
{
public:
  // Apparently Windows defines ERROR as a preprocessor macro...sigh...
  typedef enum
  {
    RTI_ERROR,
    RTI_NO_INTERSECTION,
    RTI_INTERSECTION
  } ResultCode;

  // Cast a ray starting at rayOrigin with rayDirection into the
  // triangle defined by vertices v0, v1, and v2. If intersection
  // occurred returns INTERSECTION and sets intersectionPt
  // appropriately, as well as t parameter (scale factor for
  // rayDirection to reach intersection plane starting from
  // rayOrigin). Returns NO_INTERSECTION if no intersection, or ERROR
  // if triangle was degenerate or line was parallel to plane of
  // triangle.
  static ResultCode intersectRayWithTriangle(const GleemV3f &rayOrigin,
					     const GleemV3f &rayDirection,
					     const GleemV3f &v0,
					     const GleemV3f &v1,
					     const GleemV3f &v2,
					     GleemV3f &intersectionPt,
					     float &t);

private:
  static bool approxOnSameSide(GleemV2f &linePt1, GleemV2f &linePt2,
			       GleemV2f &testPt1, GleemV2f &testPt2);
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_RAY_TRIANGLE_INTERSECTION_H
