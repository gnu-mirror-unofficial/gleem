/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_CAMERA_PARAMETERS_H
#define _GLEEM_CAMERA_PARAMETERS_H

#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

/** The information gleem needs to construct a 3D ray from the glut
    mouse callback. Note that gleem currently does not support
    asymmetrical or orthogonal viewing frusta; your viewing frustum
    must be a right truncated pyramid. The parameters vertFOV and
    imagePlaneAspectRatio correspond roughly to the parameters to
    gluPerspective(). */

class GLEEMDLL CameraParameters
{
public:
  /** 3-space origin of camera */
  GleemV3f position;

  /** 3-space forward direction of camera. Does not need to be
      normalized. */
  GleemV3f forwardDirection;

  /** 3-space upward direction of camera. This must be orthogonal to
      the viewing direction, but does not need to be normalized. */
  GleemV3f upDirection;
  
  /** This is HALF of the vertical angular span of the frustum,
      specified in radians. For example, if your <b>fovy</b> argument
      to gluPerspective() is 90, then this would be M_PI / 4. */
  float vertFOV;

  /** The aspect ratio of the image plane. Note that this does not
      necessarily have to correspond to the aspect ratio of the
      window. */
  float imagePlaneAspectRatio;

  /** The horizontal size of the window, in pixels */
  int xSize;

  /** The vertical size of the window, in pixels */
  int ySize;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_CAMERA_PARAMETERS_H
