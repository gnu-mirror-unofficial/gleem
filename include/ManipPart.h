/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_H
#define _GLEEM_MANIP_PART_H


#include <vector>
#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "HitPoint.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

class Manip;

/** A ManipPart is a visible or invisible sub-part of a manipulator.
    There are only a few necessary methods: render(), intersectRay(),
    and setTransform(). highlight() and clearHighlight() are merely
    for visual feedback and do not need to be implemented. */

GLEEM_INTERNAL class GLEEMDLL ManipPart
{
 public:
  ManipPart(Manip *parent);
  virtual ~ManipPart();

  /** Render this part using OpenGL */
  virtual void render() const = 0;

  /** Intersect a ray with this part, returning all intersected points
      in the results vector. The same rules as Manip::intersectRay()
      apply. */
  virtual void intersectRay(const GleemV3f &rayStart,
			    const GleemV3f &rayDirection,
			    vector<HitPoint> &results) = 0;

  /** Set the transform of this part. */
  virtual void setTransform(const GleemMat4f &xform) = 0;

  /** Highlight this part */
  virtual void highlight() = 0;

  /** Unhighlight this part */
  virtual void clearHighlight() = 0;

  /** Is this part pickable, or just decorative? Not pickable implies
      that intersectRay() will return immediately. */
  virtual void setPickable(bool pickable) = 0;
  virtual bool getPickable() const = 0;

  /** Is this part visible right now? Not visible implies that
      render() will return immediately. */
  virtual void setVisible(bool visible) = 0;
  virtual bool getVisible() const = 0;

  GLEEM_INTERNAL public:
    /** Set the parent of this ManipPart */
    void setParent(Manip *parent);

    /** Get the containing Manip for constructing HitPoints */
    Manip *getParent() const;

 private:
    Manip *parent;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MANIP_PART_H
