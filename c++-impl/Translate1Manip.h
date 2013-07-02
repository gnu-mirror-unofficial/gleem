/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_TRANSLATE_1_MANIP_H
#define _GLEEM_TRANSLATE_1_MANIP_H

#include <gleem/Namespace.h>
#include <gleem/GleemDLL.h>
#include <gleem/Util.h>
#include <gleem/Manip.h>
#include <gleem/Line.h>
#include <gleem/Linalg.h>

GLEEM_ENTER_NAMESPACE

class GLEEMDLL Translate1Manip : public Manip
{
public:
  /** Default Translate1Manip has translation (0, 0, 0) and axis (1,
      0, 0) */
  Translate1Manip();

  virtual ~Translate1Manip();

  /** Set the translation of this Translate1Manip. This moves its
      on-screen representation. Manipulations cause the translation to
      be modified, not overwritten, so if you want the default
      Translate1Manip to go through the point (0, 1, 0) but still
      translate along the X axis, then setTranslation(0, 1, 0). */
  void setTranslation(const GleemV3f &translation);

  /** Get the translation of this Translate1Manip. This corresponds to
      the center of its body. */
  const GleemV3f &getTranslation() const;

  /** Set the axis of this Translate1Manip. This is the direction
      along which it will travel. Does not need to be normalized, but
      must not be the zero vector. */
  void setAxis(const GleemV3f &axis);
  const GleemV3f &getAxis() const;

  /** Set the scale of the Translate1Manip. This only affects the
      on-screen geometry. */
  void setScale(const GleemV3f &scale);
  const GleemV3f &getScale() const;

  /** Change the geometry of this manipulator to be the user-defined
      piece. User retains ownership of memory of newGeom. */
  void replaceGeometry(ManipPart *newGeom);

GLEEM_INTERNAL public:
  /** Implementation of Manip interface */
  virtual void render();
  virtual void intersectRay(const GleemV3f &rayStart,
			    const GleemV3f &rayDirection,
			    vector<HitPoint> &results);
  virtual void highlight(const HitPoint &hit);
  virtual void clearHighlight();
  virtual void makeActive(const HitPoint &hit);
  virtual void drag(const GleemV3f &rayStart,
		    const GleemV3f &rayDirection);
  virtual void makeInactive();

private:
  void recalc();
  ManipPart *geometry;
  bool iOwnGeometry;
  GleemV3f translation;
  /** Normalized */
  GleemV3f axis;
  GleemV3f scale;
  /** Local-to-world transform for geometry */
  GleemMat4f xform;

  // Dragging state.
  Line dragLine;
  GleemV3f dragOffset;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_TRANSLATE_1_MANIP_H
