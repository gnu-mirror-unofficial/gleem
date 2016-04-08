/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#ifdef WIN32
# include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <ExaminerViewer.h>
#include <MathUtil.h>

GLEEM_USE_NAMESPACE

#define EPSILON 0.0001f

// How many examiner viewers do we expect to instantiate?
static const int NUM_EXAMINER_VIEWERS = 16;
ExaminerViewer::WindowToExaminerViewerTable
ExaminerViewer::windowViewerTable(NUM_EXAMINER_VIEWERS, hash<int>());

ExaminerViewer::ExaminerViewer()
{
  glutInitWindowSize(256, 256);
  init(glutCreateWindow("Examiner Viewer"));
  pair<WindowToExaminerViewerTable::iterator, bool> result =
    windowViewerTable.insert_unique(windowID, this);
  assert(result.second == true);
}

ExaminerViewer::~ExaminerViewer()
{
  windowViewerTable.erase(windowViewerTable.find(windowID));
  glutDestroyWindow(windowID);
}

ExaminerViewer::ExaminerViewer(const ExaminerViewer &viewer)
{
  glutInitWindowSize(256, 256);
  init(glutCreateWindow("Examiner Viewer"));
  *this = viewer;
}

ExaminerViewer &
ExaminerViewer::operator=(const ExaminerViewer &viewer)
{
  mouseDelegate = viewer.mouseDelegate;
  motionDelegate = viewer.motionDelegate;
  position = viewer.position;
  orientation = viewer.orientation;
  focalDist = viewer.focalDist;
  minFocalDist = viewer.minFocalDist;
  rotateSpeed = viewer.rotateSpeed;
  minRotateSpeed = viewer.minRotateSpeed;
  dollySpeed = viewer.dollySpeed;
  minDollySpeed = viewer.minDollySpeed;
  params = viewer.params;
  interactionUnderway = false;
  iOwnInteraction = false;
  provider = viewer.provider;
  return *this;
}

void
ExaminerViewer::update()
{
  recalc();
}

const CameraParameters &
ExaminerViewer::getCameraParameters() const
{
  return params;
}

const GleemRot &
ExaminerViewer::getOrientation() const
{
  return orientation;
}

void
ExaminerViewer::setMouseDelegate(void (*mouseDelegate)(int button, int state,
						       int x, int y))
{
  this->mouseDelegate = mouseDelegate;
}

void
ExaminerViewer::setMotionDelegate(void (*motionDelegate)(int x, int y))
{
  this->motionDelegate = motionDelegate;
}

void
ExaminerViewer::setBSphereProvider(BSphereProvider *provider)
{
  this->provider = provider;
}

void
ExaminerViewer::viewAll()
{
  if (provider == NULL)
    {
      cerr << "gleem::ExaminerViewer::viewAll: ERROR: "
	   << "bounding sphere provider was NULL, so I don't know how "
	   << "big the scene is" << endl;
      return;
    }
  // Figure out how far to move
  float vertFOV, horizFOV, minFOV;
  vertFOV = 2.0f * params.vertFOV;
  horizFOV = 2.0f * atan(params.imagePlaneAspectRatio * tan(params.vertFOV));
  if (vertFOV < horizFOV)
    minFOV = vertFOV;
  else
    minFOV = horizFOV;
  if (minFOV == 0.0f)
    {
      cerr << "gleem::ExaminerViewer::viewAll: ERROR: "
	   << "minimum field of view was zero!" << endl;
      return;
    }
  const BSphere &bsph = provider->getBoundingSphere();
  float dist = bsph.getRadius() / (float) sin(minFOV / 2.0f);
  
  // Now position the camera this far back from the scene's center
  // along the negative of the current forward direction
  GleemV3f bw(0.0f, 0.0f, 1.0f);
  GleemV3f dir = orientation.rotateVector(bw);
  GleemV3f::scale(dir, dist / dir.length(), dir);
  GleemV3f::add(dir, bsph.getCenter(), position);
  focalDist = dist;
  recalc();
}

void
ExaminerViewer::rotateFaster()
{
  rotateSpeed *= 2.0f;
}

void
ExaminerViewer::rotateSlower()
{
  if (rotateSpeed < minRotateSpeed)
    return;
  else
    rotateSpeed /= 2.0f;
}

void
ExaminerViewer::dollyFaster()
{
  dollySpeed *= 2.0f;
}

void
ExaminerViewer::dollySlower()
{
  if (dollySpeed < minDollySpeed)
    return;
  else
    dollySpeed /= 2.0f;
}

void
ExaminerViewer::setTitle(const char *title)
{
  int curWindow = glutGetWindow();
  if (curWindow != windowID)
    {
      glutSetWindow(windowID);
      glutSetWindowTitle((char *) title);
      glutSetWindow(curWindow);
    }
  else
    glutSetWindowTitle((char *) title);
}

void
ExaminerViewer::makeCurrent()
{
  glutSetWindow(windowID);
}

int
ExaminerViewer::getWindowID() const
{
  return windowID;
}

void
ExaminerViewer::init(int windowID)
{
  this->windowID = windowID;
  glutSetWindow(windowID);

  mouseDelegate = NULL;
  motionDelegate = NULL;

  focalDist = 10.0f;
  minFocalDist = 1.0f;
  rotateSpeed = 4.0f;
  minRotateSpeed = 0.0001f;
  dollySpeed = 2.0f;
  minDollySpeed = 0.0001f;
  interactionUnderway = false;
  iOwnInteraction = false;
  button1Down = false;
  button2Down = false;
  provider = NULL;

  position.setValue(0, 0, 0);

  params.position.setValue(0, 0, 0);
  params.forwardDirection.setValue(0, 0, -1);
  params.upDirection.setValue(0, 1, 0);
  params.vertFOV = M_PI / 8.0;
  params.imagePlaneAspectRatio = 1;
  params.xSize = 256;
  params.ySize = 256;

  glutMouseFunc(mouseFunc);
  glutMotionFunc(motionFunc);
  glutReshapeFunc(reshapeFunc);
  numMouseButtons = glutDeviceGet(GLUT_NUM_MOUSE_BUTTONS);
}

void
ExaminerViewer::mouseFunc(int button, int state, int x, int y)
{
  getExaminerViewer(glutGetWindow())->mouseMethod(button, state, x, y);
}

void
ExaminerViewer::motionFunc(int x, int y)
{
  getExaminerViewer(glutGetWindow())->motionMethod(x, y);
}

void
ExaminerViewer::reshapeFunc(int w, int h)
{
  getExaminerViewer(glutGetWindow())->reshapeMethod(w, h);
}

void
ExaminerViewer::mouseMethod(int button, int state, int x, int y)
{
  int mods = glutGetModifiers();
  if ((interactionUnderway && !iOwnInteraction) ||
      (!(mods & GLUT_ACTIVE_ALT)))
    {
      if (state == GLUT_DOWN)
	{
	  interactionUnderway = true;
	  iOwnInteraction = false;
	}
      else
	{
	  interactionUnderway = false;
	  iOwnInteraction = false;
	}
      // Pass this event on to the mouse delegate
      if (mouseDelegate != NULL)
	(*mouseDelegate)(button, state, x, y);
    }
  else
    {
      if (button == GLUT_LEFT_BUTTON)
	if (state == GLUT_DOWN)
	  button1Down = true;
	else
	  button1Down = false;

      if (numMouseButtons == 2)
	{
	  if (button == GLUT_RIGHT_BUTTON)
	    if (state == GLUT_DOWN)
	      button2Down = true;
	    else
	      button2Down = false;
	}
      else if (numMouseButtons == 3)
	{
	  if (button == GLUT_MIDDLE_BUTTON)
	    if (state == GLUT_DOWN)
	      button2Down = true;
	    else
	      button2Down = false;
	}
      lastX = x;
      lastY = y;
      
      if (button1Down || button2Down)
	{
	  interactionUnderway = true;
	  iOwnInteraction = true;
	}
      else
	{
	  interactionUnderway = false;
	  iOwnInteraction = false;
	}
    }
}

void
ExaminerViewer::motionMethod(int x, int y)
{
  if (interactionUnderway && !iOwnInteraction)
    {
      // Pass this event on to the motion delegate
      if (motionDelegate != NULL)
	(*motionDelegate)(x, y);
    }
  else 
    {
      int dx = x - lastX;
      int dy = y - lastY;
  
      lastX = x;
      lastY = y;

      if ((button1Down && (!button2Down)))
	{
	  // Rotation functionality
	  float xRads = M_PI * -1.0f * dy * rotateSpeed / 1000.0f;
	  float yRads = M_PI * -1.0f * dx * rotateSpeed / 1000.0f;
	  GleemV3f fw(0, 0, -1);
	  GleemV3f diff = orientation.rotateVector(fw);
	  diff.normalize();
	  GleemV3f::scale(diff, focalDist, diff);
	  // Update by this rotation
	  GleemRot xRot(GleemV3f(1, 0, 0), xRads);
	  GleemRot yRot(GleemV3f(0, 1, 0), yRads);
	  GleemRot newRot = yRot * xRot;
	  orientation = orientation * newRot;
	  // Now update the camera's position. Rotate about the focal point.
	  GleemV3f backOut = orientation.rotateVector(fw);
	  backOut.normalize();
	  GleemV3f::scale(backOut, -1.0f * focalDist, backOut);
	  position = position + backOut + diff;
	}
      else if (button2Down && (!button1Down))
	{
	  // Translate functionality
	  // Compute the local coordinate system's difference vector
	  GleemV3f localDiff(dollySpeed * -1.0f * dx / 100.0f,
			    dollySpeed * dy / 100.0f,
			    0.0f);
	  // Rotate this by camera's orientation
	  GleemV3f worldDiff = orientation.rotateVector(localDiff);
	  // Add on to position
	  GleemV3f::add(worldDiff, position, position);
	}
      else if (button1Down && button2Down)
	{
	  // Dolly functionality
	  // Compute the local coordinate system's difference vector
	  GleemV3f localDiff(0, 0, dollySpeed * -1.0f * dy / 100.0f);
	  // Rotate this by camera's orientation
	  GleemV3f worldDiff = orientation.rotateVector(localDiff);
	  GleemV3f::add(worldDiff, position, position);
	  // Subtract off the dolly amount from the focal distance
	  float focalDiff = (float) worldDiff.length();
	  if (dy > 0)
	    focalDiff *= -1.0f;
	  focalDist += focalDiff;
	  if (focalDist < minFocalDist)
	    focalDist = minFocalDist;
	}
    }
}

void
ExaminerViewer::reshapeMethod(int w, int h)
{
  glViewport(0, 0, w, h);
  float aspect, theta;
  aspect = (float) w / (float) h;
  if (w >= h)
    theta = 45;
  else
    theta = GLEEM_RAD_TO_DEG(atan(1 / aspect));
  params.vertFOV = GLEEM_DEG_TO_RAD(theta) / 2.0f;
  params.imagePlaneAspectRatio = aspect;
  params.xSize = w;
  params.ySize = h;
}

void
ExaminerViewer::recalc()
{
  makeCurrent();
  // Recompute position, forward and up vectors
  params.position = position;
  orientation.rotateVector(GleemV3f(0, 0, -1), params.forwardDirection);
  orientation.rotateVector(GleemV3f(0, 1, 0), params.upDirection);

  //  cerr << "params.position: " << params.position << endl;
  //  cerr << "params.forwardDirection: " << params.forwardDirection << endl;
  //  cerr << "params.upDirection: " << params.upDirection << endl;
  //  cerr << "focalDist: " << focalDist << endl;

  // Compute modelview matrix based on camera parameters, position and
  // orientation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  GleemV3f axis;
  float ang = orientation.getValue(axis);
  if (axis.lengthSquared() > EPSILON)
    glRotatef(GLEEM_RAD_TO_DEG(ang), -axis[0], -axis[1], -axis[2]);
  glTranslatef(-position[0], -position[1], -position[2]);
  //  glTranslatef(0, 0, 10);
  //  glRotatef(15, 0, 1, 0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(GLEEM_RAD_TO_DEG(params.vertFOV * 2.0f),
		 params.imagePlaneAspectRatio,
		 1, 100);
}

ExaminerViewer *
ExaminerViewer::getExaminerViewer(int windowID)
{
  WindowToExaminerViewerTable::iterator iter =
    windowViewerTable.find(windowID);
  if (iter == windowViewerTable.end())
    return NULL;
  return *iter;
}
