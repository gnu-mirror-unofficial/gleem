/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_EXAMINER_VIEWER_H
#define _GLEEM_EXAMINER_VIEWER_H


#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "CameraParameters.h"
#include "BSphere.h"
#include "BasicHashtable.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

/** This is an application-level class, not part of the manipulator
    hierarchy. It is an example of how you might integrate gleem with
    another application which uses the mouse.

    For the given rendering context, the ExaminerViewer takes over the
    setting of the Modelview matrix and GLUT's mouse, motion, and
    reshape functions. It passes along events it is not interested in
    to the given mouse and motion delegates. It does not require any
    other callbacks. A typical GLUT application might set idle and
    display callbacks, call ExaminerViewer::update() at the end of its
    idle callback, and post a redraw there, then do its drawing in the
    display callback.

    The ExaminerViewer's controls are similar to those of Open
    Inventor's Examiner Viewer. Alt + Left mouse button causes
    rotation about the focal point. Alt + Right mouse button causes
    translation parallel to the image plane. Alt + both mouse buttons,
    combined with up/down mouse motion, causes zooming out and in
    along the view vector.
*/

class GLEEMDLL ExaminerViewer
{
 public:
  /** Defaults to position (0, 0, 0), up vector (0, 1, 0), and forward
      vector (0, 0, -1). */
  ExaminerViewer();
  ~ExaminerViewer();
  ExaminerViewer(const ExaminerViewer &);
  ExaminerViewer &operator=(const ExaminerViewer &);

  /** Call this at the end of your idle function to cause the
      Modelview matrix to be recomputed. */
  void update();

  /** Get the camera parameters out of this Examiner Viewer (for
      example, to pass to ManipManager::updateCameraParameters()) */
  const CameraParameters &getCameraParameters() const;

  /** FIXME: A hack to get the orientation directly (not contained in
      CameraParameters structure) */
  const GleemRot &getOrientation() const;

  /** Set mouse and motion delegates. When the Alt key is not pressed,
      these events will be passed through to the delegates. */
  void setMouseDelegate(void (*)(int button, int state, int x, int y));
  void setMotionDelegate(void (*)(int x, int y));

  /** In order for the viewer to do anything useful, you need to
      attach a BSphereProvider to it. This class gives a bounding
      sphere to the viewer to allow "view all" functionality. */
  class GLEEMDLL BSphereProvider
  {
  public:
    virtual const BSphere &getBoundingSphere() = 0;
  };

  /** Attach a BSphereProvider to this viewer. Caller maintains
      responsibility for freeing provider's memory. */
  void setBSphereProvider(BSphereProvider *provider);

  /** Call this to view the entire scene (bounding sphere provided by
      the BSphereProvider specified above) */
  void viewAll();

  void rotateFaster();
  void rotateSlower();
  void dollyFaster();
  void dollySlower();

  /** Change the title of this viewer. */
  void setTitle(const char *title);

  /** Make this Examiner Viewer's context current (i.e.,
      glutSetWindow(windowID)) */
  void makeCurrent();

  /** Get the GLUT window ID associated with this viewer */
  int getWindowID() const;

 protected:
  void init(int windowID);

  static void mouseFunc(int button, int state, int x, int y);
  static void motionFunc(int x, int y);
  static void reshapeFunc(int w, int h);

  void mouseMethod(int button, int state, int x, int y);
  void motionMethod(int x, int y);
  void reshapeMethod(int w, int h);

  /** Delegate functions (NULL by default) */
  void (*mouseDelegate)(int button, int state, int x, int y);
  void (*motionDelegate)(int x, int y);

  /** Window ID (passed in or obtained via glutGetWindow() */
  int windowID;

  /** Camera parameters */
  GleemV3f position;
  GleemRot orientation;
  float focalDist;
  float minFocalDist;
  float rotateSpeed;
  float minRotateSpeed;
  float dollySpeed;
  float minDollySpeed;
  CameraParameters params;

  /** Simple state machine for figuring out whether we are grabbing
      events */
  bool interactionUnderway;
  bool iOwnInteraction;

  /** Simple state machine for computing distance dragged */
  int lastX, lastY;
  bool button1Down, button2Down;
  int numMouseButtons;

  /** Our bounding sphere provider */
  BSphereProvider *provider;

  void recalc();

  /** Hash table mapping window IDs to ExaminerViewer objects */
  typedef BasicHashtable<ExaminerViewer *, int, hash<int> >
    WindowToExaminerViewerTable;
  static WindowToExaminerViewerTable windowViewerTable;
  static ExaminerViewer *getExaminerViewer(int windowID);
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_EXAMINER_VIEWER_H
