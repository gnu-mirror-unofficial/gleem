/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <assert.h>
#include <stdio.h>
#include <cmath>
#include "../include/RayTriangleIntersection.h"
#include "../include/MathUtil.h"

GLEEM_USE_NAMESPACE

// Allow roundoff error of this amount.
// Be very careful adjusting this.
// Too big a value may cause valid triangles to be rejected.
// Too small a value may trigger an assert in the code to
// create an orthonormal basis in intersectRayWithTriangle.
static const float epsilon = 1.0e-3;

RayTriangleIntersection::ResultCode
RayTriangleIntersection::intersectRayWithTriangle(const GleemV3f &rayOrigin,
						  const GleemV3f &rayDirection,
						  const GleemV3f &v0,
						  const GleemV3f &v1,
						  const GleemV3f &v2,
						  GleemV3f &intersectionPt,
						  float &t)
{
  // Returns RTI_INTERSECTION if intersection computed,
  // RTI_NO_INTERSECTION if no intersection with triangle, RTI_ERROR
  // if triangle was degenerate or line did not intersect plane
  // containing triangle.

  // NOTE these rays are TWO-SIDED.

  // Find point on line. P = ray origin, D = ray direction.
  //   P + tD = W
  // Find point on plane. X, Y = orthonormal bases for plane; O = its origin.
  //   O + uX + vY = W
  // Set equal
  //   O + uX + vY = tD + P
  //   uX + vY - tD = P - O = "B"
  //   [X0 Y0 -D0] [u]   [B0]
  //   [X1 Y1 -D1] [v] = [B1]
  //   [X2 Y2 -D2] [t]   [B2]
  // Now we have u, v coordinates for the intersection point (if system
  // wasn't degenerate).
  // Find u, v coordinates for three points of triangle. (DON'T DUPLICATE
  // WORK.) Now easy to do 2D inside/outside test.
  // If point is inside, do some sort of interpolation to compute the
  // 3D coordinates of the intersection point (may be unnecessary --
  // can reuse X, Y bases from above) and texture coordinates of this
  // point (maybe compute "texture coordinate" bases using same algorithm
  // and just use u, v coordinates??).

  GleemV3f O = v0;
  GleemV3f p2 = v1 - O;
  GleemV3f p3 = v2 - O;

  GleemV3f X = p2;
  GleemV3f Y = p3;
  
  // Normalize X
  if (X.length() < epsilon)
    return RTI_ERROR;  // coincident points in triangle
  X.normalize();

  // Use Gramm-Schmitt to orthogonalize X and Y
  Y -= X * (X.dot(Y));
  if (Y.length() < epsilon)
    return RTI_ERROR;  // coincident points in triangle
  Y.normalize();
  
  //  assert(fabs(X.dot(Y)) < epsilon);
  assert(fabs(1.0 - X.length()) < epsilon);
  assert(fabs(1.0 - Y.length()) < epsilon);

  // X and Y are now orthonormal bases for the plane defined
  // by the triangle.

  GleemV3f Bv = rayOrigin - O;

  GleemMat3f A;
  A.setCol(0, X);
  A.setCol(1, Y);
  GleemV3f tmpRayDir = rayDirection;
  GleemV3f::scale(tmpRayDir, -1.0f, tmpRayDir);
  A.setCol(2, tmpRayDir);
  if (A.invert() == false)
    return RTI_ERROR;
  GleemV3f B;
  A.xformVec(Bv, B);

  GleemV2f W(B[0], B[1]);

  // Compute u,v coords of triangle
  GleemV2f uv[3];
  uv[0] = GleemV2f(0,0);
  uv[1][0] = p2.dot(X);
  uv[1][1] = p2.dot(Y);
  uv[2][0] = p3.dot(X);
  uv[2][1] = p3.dot(Y);

  assert(fabs(uv[1][1]) < epsilon);

  // Test. For each of the sides of the triangle, is the intersection
  // point on the same side as the third vertex of the triangle?
  // If so, intersection point is inside triangle.
  int i;
  for (i = 0; i < 3; i++)
    {
      if (approxOnSameSide(uv[i], uv[(i+1)%3],
			   uv[(i+2)%3], W) == false)
	return RTI_NO_INTERSECTION;
    }

  // Blend coordinates and texture coordinates according to
  // distances from 3 points
  // To do: find u,v coordinates of intersection point in coordinate
  // system of axes defined by uv[1] and uv[2].
  // Blending coords == a, b. 0 <= a,b <= 1.
  assert(fabs(uv[2][1]) > epsilon);
  assert(fabs(uv[1][0]) > epsilon);
  float a, b;
  b = W[1] / uv[2][1];
  a = (W[0] - b * uv[2][0]) / uv[1][0];

  intersectionPt = O + (a * p2) + (b * p3);
  t = B[2];
  return RTI_INTERSECTION;
}

bool
RayTriangleIntersection::approxOnSameSide(GleemV2f &linePt1, GleemV2f &linePt2,
					  GleemV2f &testPt1, GleemV2f &testPt2)
{
  // Evaluate line equation for testPt1 and testPt2

  // ((y2 - y1) / (x2 - x1)) - ((y1 - y) / (x1 - x))
  // y - (mx + b)
  float num0 = linePt2[1] - linePt1[1];
  float den0 = linePt2[0] - linePt1[0];
  float num1 = linePt1[1] - testPt1[1];
  float den1 = linePt1[0] - testPt1[0];
  float num2 = linePt1[1] - testPt2[1];
  float den2 = linePt1[0] - testPt2[0];

  if (fabs(den0) < epsilon)
    {
      // line goes vertically.
      if ((fabs(den1) < epsilon) ||
	  (fabs(den2) < epsilon))
	return true;
      
      if (GLEEM_SGN(den1) == GLEEM_SGN(den2))
	return true;
      
      return false;
    }

  float m = num0 / den0;
  // (y - y1) - m(x - x1)
  float val1 = testPt1[1] - linePt1[1] - m * (testPt1[0] - linePt1[0]);
  float val2 = testPt2[1] - linePt1[1] - m * (testPt2[0] - linePt1[0]);
  if ((fabs(val1) < epsilon) ||
      (fabs(val2) < epsilon))
    return true;

  if (GLEEM_SGN(val1) == GLEEM_SGN(val2))
    return true;

  return false;
}
