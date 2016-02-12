/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifdef WIN32
# include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "../include/ManipManager.h"
#include "../include/MathUtil.h"
#include "../include/Translate1Manip.h"

GLEEM_USE_NAMESPACE

Translate1Manip *manip = NULL;
CameraParameters params;

void
display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ManipManager::getManipManager()->updateCameraParameters(glutGetWindow(),
							  params);
  ManipManager::getManipManager()->render();
  const GleemV3f &trans = manip->getTranslation();
  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();
}

void
reshape(int w, int h)
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
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(theta, aspect, 1, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(256, 256);
  glutCreateWindow("Translate1Manip Test");

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
  glutReshapeFunc(reshape);
  ManipManager::init();

  params.position.setValue(0, 0, 0);
  params.forwardDirection.setValue(0, 0, -1);
  params.upDirection.setValue(0, 1, 0);
  params.vertFOV = M_PI / 8.0;
  params.imagePlaneAspectRatio = 1;
  params.xSize = 256;
  params.ySize = 256;

  manip = new Translate1Manip();
  manip->setTranslation(GleemV3f(0, 0, -5));
  manip->setAxis(GleemV3f(-1, 0.5, 1));

  glViewport(0, 0, 256, 256);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1, 1, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glutMainLoop();
  return 0;
}
