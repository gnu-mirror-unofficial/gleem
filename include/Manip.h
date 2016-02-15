/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_H
#define _GLEEM_MANIP_H

#include <vector>
#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "HitPoint.h"
#include "ManipPart.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

/** The base class for all manipulators. Fundamentally a manipulator
    must support a ray cast operation with itself and logic to
    understand what to do when that ray cast actually made the
    manipulator active. */

typedef void ManipCB(void *userData, Manip *manip);

class GLEEMDLL Manip
{
public:
  /** These get called each time the manipulator is dragged. Adding a
      callback twice (even with the same userData) will cause it to
      get called twice. */
  void    addMotionCallback(ManipCB *f, void *userData = NULL);
  /** Removes first instance of (callback, userData) pair, if any. */
  void removeMotionCallback(ManipCB *f, void *userData = NULL);

GLEEM_INTERNAL public:
  Manip();
  virtual ~Manip();

  /** Render this manipulator using OpenGL */
  virtual void render() = 0;

  /** Cast a ray in 3-space from the camera start position in the
      specified direction and test for intersections against all live
      portions of this manipulator. Add all hits, in arbitrary order,
      to the end of the given vector. Must not modify the results
      vector in any other way (i.e., must not remove any existing
      HitPoints from the results vector). */
  virtual void intersectRay(const GleemV3f &rayStart,
			    const GleemV3f &rayDirection,
			    vector<HitPoint> &results) = 0;

  /** Tell the manipulator to highlight the current portion of itself.
      This is merely visual feedback to the user. */
  virtual void highlight(const HitPoint &hit) = 0;

  /** Tell the manipulator to clear the current highlight */
  virtual void clearHighlight() = 0;

  /** If the ManipManager decides that this manipulator is to become
      active, it will pass back the HitPoint which made it make its
      decision. The manipulator can then change its state to look for
      drags of this portion of the manipulator. */
  virtual void makeActive(const HitPoint &hit) = 0;

  /** When a manipulator is active, drags of the live portion cause
      motion of the manipulator. The ManipManager keeps track of which
      manipulator (if any) is active and takes care of calling the
      drag() method with the current ray start and direction. The
      manipulator must keep enough state to understand how it should
      position and/or rotate itself. NOTE that the base class provides
      an implementation for this method which you must call at the end
      of your overriding method. */
  virtual void drag(const GleemV3f &rayStart,
		    const GleemV3f &rayDirection);
  
  /** When the mouse button is released, makeInactive() is called. The
      manipulator should reset its state in preparation for the next
      drag. */
  virtual void makeInactive() = 0;

private:
  typedef pair<ManipCB *, void *> CallbackInfo;
  vector<CallbackInfo> motionCallbacks;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MANIP_H
