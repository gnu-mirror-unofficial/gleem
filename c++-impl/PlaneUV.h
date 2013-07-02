/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_PLANE_UV_H
#define _GLEEM_PLANE_UV_H

#include <gleem/Namespace.h>
#include <gleem/GleemDLL.h>
#include <gleem/Util.h>
#include <gleem/Linalg.h>

GLEEM_ENTER_NAMESPACE

/** This differs from the Plane class in that it maintains an origin
    and orthonormal U, V axes in the plane so that it can project a 3D
    point to a 2D one. U cross V = normal. U and V coordinates are
    computed with respect to the origin. */

GLEEM_INTERNAL class GLEEMDLL PlaneUV
{
public:
  /** Default constructor initializes normal to (0, 1, 0), origin to
      (0, 0, 0), U axis to (1, 0, 0) and V axis to (0, 0, -1). */
  PlaneUV();

  /** Takes normal vector and a point which the plane goes through
      (which becomes the plane's "origin"). Normal does NOT have to be
      normalized, but may not be zero vector. U and V axes are
      initialized to arbitrary values. */
  PlaneUV(const GleemV3f &normal, const GleemV3f &origin);

  /** Takes normal vector, point which plane goes through, and the "u"
    axis in the plane. Computes the "v" axis by taking the cross
    product of the normal and the u axis. Axis must be perpendicular
    to normal. Normal and uAxis do NOT have to be normalized, but
    neither may be the zero vector. */
  PlaneUV(const GleemV3f &normal,
	  const GleemV3f &origin,
	  const GleemV3f &uAxis);

  /** Takes normal vector, point which plane goes through, and both
    the u and v axes. u axis cross v axis = normal. Normal, uAxis, and
    vAxis do NOT have to be normalized, but none may be the zero
    vector. */
  PlaneUV(const GleemV3f &normal,
	  const GleemV3f &origin,
	  const GleemV3f &uAxis,
	  const GleemV3f &vAxis);

  /** Set the origin, through which this plane goes and with respect
      to which U and V coordinates are computed */
  void setOrigin(const GleemV3f &origin);
  const GleemV3f &getOrigin() const;

  /** Normal, U and V axes must be orthogonal and satisfy U cross V =
      normal, do not need to be unit length but must not be the zero
      vector. */
  void setNormalAndUV(const GleemV3f &normal,
		      const GleemV3f &uAxis,
		      const GleemV3f &vAxis);
  /** This version sets the normal vector and generates new U and V
      axes. */
  void setNormal(const GleemV3f &normal);
  /** This version computes the V axis from (normal cross U). */
  void setNormalAndU(const GleemV3f &normal,
		     const GleemV3f &uAxis);
  /** Normal, U and V axes are normalized internally, so, for example,
      <b>normal</b> is not necessarily equal to
      <code>plane.setNormal(normal); plane.getNormal();</code> */
  const GleemV3f &getNormal() const;
  const GleemV3f &getUAxis() const;
  const GleemV3f &getVAxis() const;

  /** Project a point onto the plane */
  void projectPoint(const GleemV3f &point,
		    GleemV3f &projPt,
		    GleemV2f &uvCoords) const;

  /** Intersect a ray with this plane, outputting not only the 3D
      intersection point but also the U, V coordinates of the
      intersection. Returns true if intersection occurred, false
      otherwise. This is a two-sided ray cast. */
  bool intersectRay(const GleemV3f &rayStart,
		    const GleemV3f &rayDirection,
		    GleemV3f &intPt,
		    float &t,
		    GleemV2f &uvCoords) const;

private:
  /** Set everything */
  void setEverything(const GleemV3f &normal,
		     const GleemV3f &origin,
		     const GleemV3f &uAxis,
		     const GleemV3f &vAxis);

  GleemV3f origin;
  /** Normalized */
  GleemV3f normal;
  GleemV3f uAxis;
  GleemV3f vAxis;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_PLANE_UV_H
