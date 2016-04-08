/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_TRANSLATE_2_MANIP_H
#define _GLEEM_TRANSLATE_2_MANIP_H

#include <Namespace.h>
#include <GleemDLL.h>
#include <Util.h>
#include <Manip.h>
#include <Plane.h>
#include <Linalg.h>

GLEEM_ENTER_NAMESPACE

class GLEEMDLL Translate2Manip : public Manip
{
public:
  /** Default Translate2Manip has translation (0, 0, 0) and normal (0,
      1, 0) */
  Translate2Manip();

  virtual ~Translate2Manip();

  /** Set the translation of this Translate2Manip. This moves its
      on-screen representation. Manipulations cause the translation to
      be modified, not overwritten, so if you want the default
      Translate2Manip to go through the point (0, 1, 0) but still
      translate in the X-Z plane, then setTranslation(0, 1, 0). */
  void setTranslation(const GleemV3f &translation);

  /** Get the translation of this Translate2Manip. This corresponds to
      the center of its body. */
  const GleemV3f &getTranslation() const;

  /** Set the normal vector of this Translate2Manip. The
      Translate2Manip travels in the plane defined by this normal and
      the current translation (which is a point in the plane). Does
      not need to be normalized, but must not be the zero vector. */
  void setNormal(const GleemV3f &normal);
  const GleemV3f &getNormal() const;

  /** Set the scale of the Translate2Manip. This only affects the
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
  void createGeometry();
  void recalc();
  ManipPart *geometry;
  bool iOwnGeometry;
  GleemV3f translation;
  /** Normalized */
  GleemV3f normal;
  GleemV3f scale;
  /** Local-to-world transform for geometry */
  GleemMat4f xform;

  // Dragging state.
  Plane dragPlane;
  GleemV3f dragOffset;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_TRANSLATE_2_MANIP_H
