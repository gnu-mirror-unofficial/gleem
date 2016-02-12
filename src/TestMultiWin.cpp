/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <cmath>
#ifdef WIN32
# include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "../include/ManipManager.h"
#include "../include/HandleBoxManip.h"
#include "../include/MathUtil.h"
#include "../include/ExaminerViewer.h"

GLEEM_USE_NAMESPACE

HandleBoxManip *manip = NULL;
ExaminerViewer *viewer0 = NULL;
ExaminerViewer *viewer1 = NULL;

class HandleBoxManipBSphereProvider : 
  public ExaminerViewer::BSphereProvider
{
public:
  HandleBoxManipBSphereProvider(HandleBoxManip *manip)
  {
    this->manip = manip;
  }

  virtual const BSphere &getBoundingSphere()
  {
    bsph.setCenter(manip->getTranslation());
    GleemV3f scale0 = manip->getScale();
    GleemV3f scale1 = manip->getGeometryScale();
    GleemV3f scale;
    scale[0] = 2.0f * scale0[0] * scale1[0];
    scale[1] = 2.0f * scale0[1] * scale1[1];
    scale[2] = 2.0f * scale0[2] * scale1[2];
    bsph.setRadius(sqrtf(scale[0] * scale[0] +
			 scale[1] * scale[1] +
			 scale[2] * scale[2]));
    return bsph;
  }

private:
  HandleBoxManip *manip;
  BSphere bsph;
};

void
display()
{
  viewer0->makeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  viewer1->makeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  viewer0->update();
  viewer1->update();
  ManipManager::getManipManager()->
    updateCameraParameters(viewer0->getWindowID(),
			   viewer0->getCameraParameters());
  ManipManager::getManipManager()->
    updateCameraParameters(viewer1->getWindowID(),
			   viewer1->getCameraParameters());
  ManipManager::getManipManager()->render();
  glFlush();
  viewer0->makeCurrent();
  glutSwapBuffers();
  glutPostRedisplay();
  viewer1->makeCurrent();
  glutSwapBuffers();
  glutPostRedisplay();
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  ManipManager::init(false);
  viewer0 = new ExaminerViewer();
  viewer0->makeCurrent();
  viewer0->setMouseDelegate(ManipManager::mouseFunc);
  viewer0->setMotionDelegate(ManipManager::motionFunc);
  glutPassiveMotionFunc(ManipManager::passiveMotionFunc);

  //GLfloat matSpecular[] = {1, 1, 1, 1};
  //  GLfloat matShininess[] = {50};
  GLfloat lightPosition[] = {1, 1, 1, 0};
  glClearColor(0, 0, 0, 0);
  //  glShadeModel(GL_SMOOTH);
  //  glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
  //  glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
  GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(display);
  manip = new HandleBoxManip();
  //  manip->setTranslation(GleemV3f(0, 0, -10));
  viewer0->setBSphereProvider(new HandleBoxManipBSphereProvider(manip));
  viewer0->viewAll();

  viewer1 = new ExaminerViewer();
  viewer1->makeCurrent();
  viewer1->setMouseDelegate(ManipManager::mouseFunc);
  viewer1->setMotionDelegate(ManipManager::motionFunc);
  glutPassiveMotionFunc(ManipManager::passiveMotionFunc);

  glClearColor(0, 0, 0, 0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(display);
  viewer1->setBSphereProvider(new HandleBoxManipBSphereProvider(manip));
  viewer1->viewAll();
  ManipManager::getManipManager()->addManipToWindow(manip,
						    viewer1->getWindowID());

  glutMainLoop();
  return 0;
}
