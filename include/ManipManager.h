/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIPMANAGER_H
#define _GLEEM_MANIPMANAGER_H

#include <vector>
#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "ScreenToRayMapping.h"
#include "BasicHashtable.h"

GLEEM_ENTER_NAMESPACE

class Manip;

/** This class is a singleton and keeps track of all instantiated manips */

class GLEEMDLL ManipManager
{
 public:

  /** init() must be called before instantiating any manips. By
      default it overrides the GLUT mouse, passive motion, and motion
      callbacks for the current window (i.e., that returned from
      glutGetWindow()). If your application uses these then you can
      pass a value of false to the initialization function and call
      them manually at the end of your callbacks. */
  static void init(bool installGLUTCallbacks = true);
  static ManipManager *getManipManager();

  /** updateCameraParameters() must be called each tick for each
      window in which you are using manipulators. The window ID is a
      GLUT window ID obtained from, i.e., glutCreateWindow(). You must
      coerce your application's camera parameters into the
      gleem::CameraParameters data structure. */
  void updateCameraParameters(int windowID, const CameraParameters &params);

  /** This must be called each clock tick, and renders all
      manipulators. NOTE that this changes the graphics context (i.e.,
      makes multiple calls to glutSetWindow). FIXME: what about state
      changes? Guarantees on that? */
  void render();
  
  /** Support for multiple windows. Call this to notify the
      ManipManager of the creation of a new window (glutCreateWindow,
      glutCreateSubWindow). This is optional because it will
      automatically be called if you instantiate a Manip into a
      previously unseen window. Returns false if this windowID was a
      duplicate of one it had already seen. */
  bool windowCreated(int windowID);

  /** Support for multiple windows. Call this to notify the
      ManipManager of the closing/destroying of a window (via
      glutDestroyWindow). This is NOT OPTIONAL and if you forget to
      call it before destroying the window then your program will
      CRASH the next time you call update(). NOTE that this does not
      call glutDestroyWindow itself. Returns false if the ManipManager
      had not seen this window ID before. */
  bool windowDestroyed(int windowID);

  /** Support for multiple windows. Call this to make a manipulator
      show up in a window it was not created into. windowID must be a
      valid GLUT window identifier. If windowID had not been seen
      before (via a call to windowCreated), calls this automatically.
      Returns false if this manip was already being viewed by this
      window. (Note: this is used internally by manipulators to get
      themselves displayed in the window they are created into as
      well.) */
  bool addManipToWindow(Manip *manip, int windowID);
  
  /** Support for multiple windows. Call this to remove a manipulator
      from a particular window. Returns false if manip was not being
      viewed in this window. */
  bool removeManipFromWindow(Manip *manip, int windowID);

  GLEEM_INTERNAL public:

    /** This installs the mouse, motion and passive motion callbacks
	which the ManipManager needs for the given window. NOTE that
	this switches rendering contexts (via glutSetWindow) and leaves
	windowID's context the current. You must not change this
	behavior (for example, ExaminerViewer.cpp relies on it.) */
    void installGLUTCallbacks(int windowID);

    /** Installed by installGLUTCallbacks, but you can call it manually
	if your application needs to override it */
    static void mouseFunc(int button, int state, int x, int y);

    /** Installed by installGLUTCallbacks, but you can call it manually
	if your application needs to override it */
    static void motionFunc(int x, int y);

    /** Installed by installGLUTCallbacks, but you can call it manually
	if your application needs to override it. This one only provides
	highlighting, which is merely a convenience, so you can skip
	calling it if you don't need it or it's too expensive */
    static void passiveMotionFunc(int x, int y);

    /** Okay, okay. Here's the mapping from normalized screen
	coordinates to a 3D ray based on the camera parameters. You can
	feel free to replace this mapping with something more
	sophisticated; you'll have to modify the CameraParameters data
	structure, though. FIXME: update this to handle multiple
	windows. */
    ScreenToRayMapping *getScreenToRayMapping();
    void setScreenToRayMapping(ScreenToRayMapping *mapping);

    /** Remove a manipulator from the manager completely. */
    void removeManip(Manip *manip);

    /** Get the camera parameters associated with a particular
	window. NOTE: since this is an internal method, asserts if the
	windowID was unknown. */
    const CameraParameters &getCameraParameters(int windowID);

 private:
    ManipManager();

    void mouseMethod(int windowID, int button, int state, int x, int y);
    void motionMethod(int windowID, int x, int y);
    void passiveMotionMethod(int windowID, int x, int y);

    static ManipManager *manipManager;
    ScreenToRayMapping *mapping;

    // To handle multiple windows, we keep a list of attached
    // manipulators to each window ID.
    typedef vector<Manip *> ManipList;
    typedef BasicHashtable<ManipList, int, hash<int> > WindowToManipListTable;
    WindowToManipListTable windowManipTable;

    // Hash table mapping window IDs to lists of Manips which are
    // contained in that window
    typedef vector<int> IntList;
    typedef size_t ManipHashFunc(const Manip * const &arg);
    static size_t hashManip(const Manip * const &arg);
    typedef BasicHashtable<IntList, Manip *, ManipHashFunc *> ManipToWindowListTable;
    ManipToWindowListTable manipWindowTable;

    // Hash table mapping window ID to CameraParameters structure
    typedef BasicHashtable<CameraParameters, int, hash<int> > WindowToCameraParametersTable;
    WindowToCameraParametersTable windowCameraTable;

    // Convenience routines

    /** Ensure that an entry exists for manip. Does not create a new one
	if one already exists. */
    void createEntryForManip(Manip *manip);

    /** Remove the entry for the given manipulator completely. This also
	cleans up the reverse mappings from window ID to manipulator. */
    void removeEntryForManip(Manip *manip);

    /** Returns true if the ManipManager knows about the passed
	manipulator, false if not. */
    bool findEntryForManip(Manip *manip);

    /** Ensure that an entry exists for windowID. Does not create a new
	one if one already exists. */
    void createEntryForWindow(int windowID);

    /** Remove the entry for the given window ID completely. This also
	cleans up the reverse mappings from manipulator to window ID. */
    void removeEntryForWindow(int windowID);

    /** Returns true if the ManipManager knows about the passed window
	ID, false if not. */
    bool findEntryForWindow(int windowID);

    bool dragging;
    Manip *curManip;
    Manip *curHighlightedManip;

    GleemV2f screenToNormalizedCoordinates(const CameraParameters &params,
					   int x, int y);
    bool computeRay(const CameraParameters &params,
		    int x, int y,
		    GleemV3f &raySource,
		    GleemV3f &rayDirection);
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MANIPMANAGER_H
