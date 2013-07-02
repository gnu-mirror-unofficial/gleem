/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998, 1999, 2002 Kenneth B. Russell (kbrussel@alum.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

package gnu.gleem;

import java.awt.Dimension;
import java.awt.event.*;
import java.util.*;

import gnu.gleem.linalg.*;
import gl4java.*;
import gl4java.drawable.*;

/** <P> This is an application-level class, not part of the
    manipulator hierarchy. It is an example of how you might integrate
    gleem with another application which uses the mouse. </P>

    <P> For the given GLDrawable, the ExaminerViewer takes over the
    setting of the view position. It passes along mouse events it is
    not interested in to the ManipManager's mouse routines. </P>

    <P> The ExaminerViewer's controls are similar to those of Open
    Inventor's Examiner Viewer. Alt + Left mouse button causes
    rotation about the focal point. Alt + Right mouse button causes
    translation parallel to the image plane. Alt + both mouse buttons,
    combined with up/down mouse motion, causes zooming out and in
    along the view vector. (On platforms with a "Meta" key, that key
    can be substituted in place of the Alt key.) </P>

    <P>NOTE: the current ExaminerViewer implementation assumes a
    minimum of two mouse buttons. For the Mac OS, the code needs to be
    adjusted to use e.g., the Control key as the "right" mouse
    button. </P> */

public class ExaminerViewer {
  private GLDrawable window;
  /** Simple state machine for figuring out whether we are grabbing
      events */
  private boolean interactionUnderway;
  private boolean iOwnInteraction;

  /** Simple state machine for computing distance dragged */
  private boolean button1Down;
  private boolean button2Down;
  private int numMouseButtons;
  private int lastX;
  private int lastY;

  /** Camera parameters */
  Vec3f position;
  Rotf orientation;
  float focalDist;
  float minFocalDist;
  float rotateSpeed;
  float minRotateSpeed;
  float dollySpeed;
  float minDollySpeed;
  CameraParameters params;

  /** Our bounding sphere provider (for viewAll()) */
  BSphereProvider provider;

  private MouseMotionAdapter mouseMotionListener = new MouseMotionAdapter() {
      public void mouseDragged(MouseEvent e) {
        motionMethod(e, e.getX(), e.getY());
      }

      public void mouseMoved(MouseEvent e) {
        passiveMotionMethod(e);
      }
    };

  private MouseAdapter mouseListener = new MouseAdapter() {
      public void mousePressed(MouseEvent e) {
        mouseMethod(e, e.getModifiers(), true, e.getX(), e.getY());
      }

      public void mouseReleased(MouseEvent e) {
        mouseMethod(e, e.getModifiers(), false, e.getX(), e.getY());
      }
    };

  private GLEventListener glListener = new GLEventListener() {
      public void init(GLDrawable drawable) {}
      public void preDisplay(GLDrawable drawable) {}
      public void display(GLDrawable drawable) {}
      public void postDisplay(GLDrawable drawable) {}
      public void cleanup(GLDrawable drawable) {
        detach();
      }
      public void reshape(GLDrawable drawable, int width, int height) {
        reshapeMethod(width, height);
      }
    };

  /** The constructor takes the number of mouse buttons on this system
      (couldn't figure out how to determine this internally) */
  public ExaminerViewer(int numMouseButtons) {
    this.numMouseButtons = numMouseButtons;

    focalDist = 10.0f;
    minFocalDist = 1.0f;
    rotateSpeed = 4.0f;
    minRotateSpeed = 0.0001f;
    dollySpeed = 2.0f;
    minDollySpeed = 0.0001f;
    position = new Vec3f();
    orientation = new Rotf();
    params = new CameraParameters();
  }

  /** <P> Attaches this ExaminerViewer to the given GLDrawable. This
      causes the ManipManager's mouse routines to be removed from the
      window (using ManipManager.removeMouseListeners) and the
      ExaminerViewer's to be installed. The GLDrawable should be
      registered with the ManipManager before the ExaminerViewer is
      attached to it. </P>

      <P> In order for the viewer to do anything useful, you need to
      provide a BSphereProvider to it to allow "view all"
      functionality. </P> */
  public void attach(GLDrawable window, BSphereProvider provider) {
    this.window = window;
    this.provider = provider;
    init();
    setupListeners();
  }

  /** Detaches from the given window. This causes the ManipManager's
      mouse listeners to be reinstalled on the GLDrawable and the
      ExaminerViewer's to be removed. */
  public void detach() {
    removeListeners();
    this.window = null;
    this.provider = null;
  }
  
  /** Call this at the end of your display() method to cause the
      Modelview matrix to be recomputed for the next frame. */
  public void update(GLFunc gl, GLUFunc glu) {
    recalc(gl, glu);
  }

  /** Call this from within your display() method to cause the
      ExaminerViewer to recompute its position based on the visible
      geometry. A BSphereProvider must have already been set or this
      method has no effect. */
  public void viewAll(GLFunc gl, GLUFunc glu) {
    if (provider == null) {
      return;
    }
    // Figure out how far to move
    float vertFOV, horizFOV, minFOV;
    vertFOV = 2.0f * params.getVertFOV();
    horizFOV = 2.0f * (float) Math.atan(params.getImagePlaneAspectRatio() *
                                        Math.tan(params.getVertFOV()));
    if (vertFOV < horizFOV)
      minFOV = vertFOV;
    else
      minFOV = horizFOV;
    if (minFOV == 0.0f) {
      throw new RuntimeException("Minimum field of view was zero");
    }
    BSphere bsph = provider.getBoundingSphere();
    float dist = bsph.getRadius() / (float) Math.sin(minFOV / 2.0f);
  
    // Now position the camera this far back from the scene's center
    // along the negative of the current forward direction
    Vec3f dir = orientation.rotateVector(Vec3f.Z_AXIS);
    dir.scale(dist / dir.length());
    position.add(dir, bsph.getCenter());
    focalDist = dist;
    recalc(gl, glu);
  }

  /** Get the camera parameters out of this Examiner Viewer (for
      example, to pass to ManipManager.updateCameraParameters()) */
  public CameraParameters getCameraParameters() {
    return params;
  }

  /** These routines can be hooked into a GUI by calling them from
      ActionEvent listeners for buttons elsewhere in the application. */
  public void rotateFaster() {
    rotateSpeed *= 2.0f;
  }

  public void rotateSlower() {
    if (rotateSpeed < minRotateSpeed)
      return;
    else
      rotateSpeed /= 2.0f;
  }

  public void dollyFaster() {
    dollySpeed *= 2.0f;
  }

  public void dollySlower() {
    if (dollySpeed < minDollySpeed)
      return;
    else
      dollySpeed /= 2.0f;
  }

  //----------------------------------------------------------------------
  // Internals only below this point
  //

  private static final float EPSILON = 0.0001f;

  private void setupListeners() {
    ManipManager.getManipManager().removeMouseListeners(window);
    window.addMouseMotionListener(mouseMotionListener);
    window.addMouseListener(mouseListener);
    window.addGLEventListener(glListener);
  }

  private void removeListeners() {
    window.removeMouseMotionListener(mouseMotionListener);
    window.removeMouseListener(mouseListener);
    window.removeGLEventListener(glListener);
    ManipManager.getManipManager().setupMouseListeners(window);
  }

  private void passiveMotionMethod(MouseEvent e) {
    ManipManager.getManipManager().mouseMoved(e);
  }

  private static boolean modifiersMatch(MouseEvent e) {
    return ((e.isAltDown() || e.isMetaDown()) &&
            (!e.isControlDown() && !e.isShiftDown()));
  }

  private void init() {
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
    provider = null;

    position.set(0, 0, 0);

    Dimension size = window.getSize();
    int xSize = size.width;
    int ySize = size.height;
    params.setPosition(position);
    params.setForwardDirection(Vec3f.NEG_Z_AXIS);
    params.setUpDirection(Vec3f.Y_AXIS);
    params.setVertFOV((float) Math.PI / 8.0f);
    params.setImagePlaneAspectRatio((float) xSize / (float) ySize);
    params.setXSize(xSize);
    params.setYSize(ySize);
  }

  private void motionMethod(MouseEvent e, int x, int y) {
    if (interactionUnderway && !iOwnInteraction) {
      ManipManager.getManipManager().mouseDragged(e);
    } else {
      int dx = x - lastX;
      int dy = y - lastY;
  
      lastX = x;
      lastY = y;

      if ((button1Down && (!button2Down))) {

        // Rotation functionality
        float xRads = (float) Math.PI * -1.0f * dy * rotateSpeed / 1000.0f;
        float yRads = (float) Math.PI * -1.0f * dx * rotateSpeed / 1000.0f;
        Vec3f fw = new Vec3f(0, 0, -1);
        Vec3f diff = new Vec3f(orientation.rotateVector(fw));
        diff.normalize();
        diff.scale(focalDist);
        // Update by this rotation
        Rotf xRot = new Rotf(Vec3f.X_AXIS, xRads);
        Rotf yRot = new Rotf(Vec3f.Y_AXIS, yRads);
        Rotf newRot = yRot.times(xRot);
        orientation = orientation.times(newRot);
        // Now update the camera's position. Rotate about the focal point.
        Vec3f backOut = orientation.rotateVector(fw);
        backOut.normalize();
        backOut.scale(-1.0f * focalDist);
        position = position.plus(backOut).plus(diff);

      } else if (button2Down && (!button1Down)) {

        // Translate functionality
        // Compute the local coordinate system's difference vector
        Vec3f localDiff = new Vec3f(dollySpeed * -1.0f * dx / 100.0f,
                                    dollySpeed * dy / 100.0f,
                                    0.0f);
        // Rotate this by camera's orientation
        Vec3f worldDiff = orientation.rotateVector(localDiff);
        // Add on to position
        position.add(worldDiff);

      } else if (button1Down && button2Down) {

        // Dolly functionality
        // Compute the local coordinate system's difference vector
        Vec3f localDiff = new Vec3f(0, 0, dollySpeed * -1.0f * dy / 100.0f);
        // Rotate this by camera's orientation
        Vec3f worldDiff = orientation.rotateVector(localDiff);
        position.add(worldDiff);
        // Subtract off the dolly amount from the focal distance
        float focalDiff = (float) worldDiff.length();
        if (dy > 0)
          focalDiff *= -1.0f;
        focalDist += focalDiff;
        if (focalDist < minFocalDist)
          focalDist = minFocalDist;
      }

      // Force redraw if window will not do it automatically
      if (!(window instanceof GLRunnable)) {
        window.repaint();
      }
    }

    // FIXME: can not call this from here
    //    recalc();
  }

  private void mouseMethod(MouseEvent e, int mods, boolean press,
                           int x, int y) {
    if ((interactionUnderway && !iOwnInteraction) ||
        (!modifiersMatch(e))) {
      // Update state and pass this event along to the ManipManager
      if (press) {
        interactionUnderway = true;
        iOwnInteraction = false;
        ManipManager.getManipManager().mousePressed(e);
      } else {
        interactionUnderway = false;
        iOwnInteraction = false;
        ManipManager.getManipManager().mouseReleased(e);
      }
    } else {
      if ((mods & MouseEvent.BUTTON1_MASK) != 0) {
        if (press) {
          button1Down = true;
        } else {
          button1Down = false;
        }
      } else {
        if (numMouseButtons == 2) {
          if ((mods & MouseEvent.BUTTON2_MASK) != 0) {
            if (press) {
              button2Down = true;
            } else {
              button2Down = false;
            }
          }
        } else if (numMouseButtons == 3) {
          // FIXME: must test this on 3-button system
          if ((mods & MouseEvent.BUTTON3_MASK) != 0) {
            if (press) {
              button2Down = true;
            } else {
              button2Down = false;
            }
          }
        }
      }

      lastX = x;
      lastY = y;

      if (button1Down || button2Down) {
        interactionUnderway = true;
        iOwnInteraction = true;
      } else {
        interactionUnderway = false;
        iOwnInteraction = false;
      }

      // Force redraw if window will not do it automatically
      if (!(window instanceof GLRunnable)) {
        window.repaint();
      }
    }
  }

  private void reshapeMethod(int w, int h) {
    float aspect, theta;
    aspect = (float) w / (float) h;
    if (w >= h)
      theta = 45;
    else
      theta = (float) Math.toDegrees(Math.atan(1 / aspect));
    params.setVertFOV((float) (Math.toRadians(theta) / 2.0));
    params.setImagePlaneAspectRatio(aspect);
    params.setXSize(w);
    params.setYSize(h);
  }

  private void recalc(GLFunc gl, GLUFunc glu) {
    // Recompute position, forward and up vectors
    params.setPosition(position);
    Vec3f tmp = new Vec3f();
    orientation.rotateVector(Vec3f.NEG_Z_AXIS, tmp);
    params.setForwardDirection(tmp);
    orientation.rotateVector(Vec3f.Y_AXIS, tmp);
    params.setUpDirection(tmp);

    // Compute modelview matrix based on camera parameters, position and
    // orientation
    gl.glMatrixMode(GLEnum.GL_MODELVIEW);
    gl.glLoadIdentity();
    float ang = orientation.get(tmp);
    if (tmp.lengthSquared() > EPSILON)
      gl.glRotatef((float) Math.toDegrees(ang), -tmp.x(), -tmp.y(), -tmp.z());
    gl.glTranslatef(-position.x(), -position.y(), -position.z());
    gl.glMatrixMode(GLEnum.GL_PROJECTION);
    gl.glLoadIdentity();
    glu.gluPerspective(Math.toDegrees(params.getVertFOV() * 2.0),
                       params.getImagePlaneAspectRatio(),
                       1, 100);
  }
}
