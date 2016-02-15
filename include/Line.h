/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_LINE_H
#define _GLEEM_LINE_H


#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

GLEEM_INTERNAL class GLEEMDLL Line
{
public:
  /** Default constructor initializes line to point (0, 0, 0) and
      direction (1, 0, 0) */
  Line();

  /** Line goes in direction <b>direction</b> through the point
      <b>point</b>. <b>direction</b> does not need to be normalized but must
      not be the zero vector. */
  Line(const GleemV3f &direction,
       const GleemV3f &point);

  /** Setter does some work to maintain internal caches.
      <b>direction</b> does not need to be normalized but must not be
      the zero vector. */
  void setDirection(const GleemV3f &direction);
  /** Direction is normalized internally, so <b>direction</b> is not
      necessarily equal to <code>plane.setDirection(direction);
      plane.getDirection();</code> */
  const GleemV3f &getDirection() const;

  /** Setter does some work to maintain internal caches. */
  void setPoint(const GleemV3f &point);
  const GleemV3f &getPoint() const;

  /** Project a point onto the line */
  void projectPoint(const GleemV3f &point,
		    GleemV3f &projPt) const;

  /** Find closest point on this line to the given ray, specified by
      start point and direction. If ray is parallel to this line,
      returns false and closestPoint is not modified. */
  bool closestPointToRay(const GleemV3f &rayStart,
			 const GleemV3f &rayDirection,
			 GleemV3f &closestPoint);

private:
  GleemV3f point;
  /** Normalized */
  GleemV3f direction;
  /** For computing projections along line */
  GleemV3f alongVec;
  void recalc();
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_LINE_H
