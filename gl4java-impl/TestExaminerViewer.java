/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998, 1999, 2002 Kenneth B. Russell (kbrussel@alum.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

package gnu.gleem;

import java.awt.*;
import java.awt.event.*;
import gl4java.*;
import gl4java.awt.*;
import gl4java.drawable.*;
import gnu.gleem.linalg.*;

/** Tests the Examiner Viewer. */

public class TestExaminerViewer {
  private static final int X_SIZE = 400;
  private static final int Y_SIZE = 400;

  static class HandleBoxManipBSphereProvider implements BSphereProvider {
    private HandleBoxManip manip;

    private HandleBoxManipBSphereProvider(HandleBoxManip manip) {
      this.manip = manip;
    }

    public BSphere getBoundingSphere() {
      BSphere bsph = new BSphere();
      bsph.setCenter(manip.getTranslation());
      Vec3f scale0 = manip.getScale();
      Vec3f scale1 = manip.getGeometryScale();
      Vec3f scale = new Vec3f();
      scale.setX(2.0f * scale0.x() * scale1.x());
      scale.setY(2.0f * scale0.y() * scale1.y());
      scale.setZ(2.0f * scale0.z() * scale1.z());
      bsph.setRadius(scale.length());
      return bsph;
    }
  }

  static class Listener implements GLEventListener, GLEnum {
    private GLFunc gl;
    private GLUFunc glu;
    private CameraParameters params = new CameraParameters();
    private ExaminerViewer viewer;

    public void init(GLDrawable drawable) {
      gl = drawable.getGL();
      glu = drawable.getGLU();

      gl.glClearColor(0, 0, 0, 0);
      float[] lightPosition = new float[] {1, 1, 1, 0};
      float[] ambient       = new float[] { 0.0f, 0.0f, 0.0f, 1.0f };
      float[] diffuse       = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
      gl.glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
      gl.glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
      gl.glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

      gl.glEnable(GL_LIGHTING);
      gl.glEnable(GL_LIGHT0);
      gl.glEnable(GL_DEPTH_TEST);

      params.setPosition(new Vec3f(0, 0, 0));
      params.setForwardDirection(Vec3f.NEG_Z_AXIS);
      params.setUpDirection(Vec3f.Y_AXIS);
      params.setVertFOV((float) (Math.PI / 8.0));
      params.setImagePlaneAspectRatio(1);
      params.xSize = X_SIZE;
      params.ySize = Y_SIZE;

      gl.glMatrixMode(GL_PROJECTION);
      gl.glLoadIdentity();
      glu.gluPerspective(45, 1, 1, 100);
      gl.glMatrixMode(GL_MODELVIEW);
      gl.glLoadIdentity();

      // Register the window with the ManipManager
      ManipManager manager = ManipManager.getManipManager();
      manager.registerWindow(drawable);

      // Instantiate a HandleBoxManip
      HandleBoxManip manip = new HandleBoxManip();
      manip.setTranslation(new Vec3f(0, 0, -10));
      manager.showManipInWindow(manip, drawable);

      // Instantiate ExaminerViewer
      viewer = new ExaminerViewer(MouseButtonHelper.numMouseButtons());
      viewer.attach(drawable, new HandleBoxManipBSphereProvider(manip));
      viewer.viewAll(gl, glu);
    }

    public void display(GLDrawable drawable) {
      gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      viewer.update(gl, glu);
      ManipManager.getManipManager().updateCameraParameters(drawable, viewer.getCameraParameters());
      ManipManager.getManipManager().render(drawable, gl);
    }

    // Unused routines
    public void reshape(GLDrawable drawable, int w, int h) {}
    public void preDisplay(GLDrawable drawable) {}
    public void postDisplay(GLDrawable drawable) {}
    public void cleanup(GLDrawable drawable) {}
  }

  public static void main(String[] args) {
    Frame frame = new Frame("ExaminerViewer Test");
    frame.addWindowListener(new WindowAdapter() {
        public void windowClosing(WindowEvent e) {
          System.exit(0);
        }
      });
    frame.setLayout(new BorderLayout());
    GLCanvas canvas =
      GLDrawableFactory.getFactory().createGLCanvas(new GLCapabilities(), 400, 400);
    canvas.addGLEventListener(new Listener());
    frame.add(canvas, BorderLayout.CENTER);
    frame.pack();
    frame.show();
  }
}
