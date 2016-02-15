/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_HIT_POINT_H
#define _GLEEM_HIT_POINT_H

#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

class Manip;
class ManipPart;

/** Defines an intersection of a ray with a piece of a manipulator */

class GLEEMDLL HitPoint
{
 public:
  /** The manipulator which was intersected */
  Manip *manipulator;

  /** The sub-piece of the manipulator which was actually intersected */
  ManipPart *manipPart;
  
  /** The ray which was cast. The manipulator part must set this when
      an intersection is detected. */
  GleemV3f rayStart;
  GleemV3f rayDirection;

  // Not all pieces of manipulators supply all of these pieces of
  // information.

  /** The parameter along the ray at which the intersection was
      found. It's important that this be supplied because the
      ManipManager needs it to disambiguate between intersections with
      multiple manipulators. */
  float t;

  /** The 3D point at which the intersection occurred */
  GleemV3f intPt;

  /** If, for example, the intersected object was a plane, this would
      be the U, V coordinates within that plane at which the
      intersection occurred. */
  GleemV2f intUV;

  /** User data for extended functionality */
  void *userData;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_HIT_POINT_H
