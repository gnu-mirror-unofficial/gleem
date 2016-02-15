/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_SCREEN_TO_RAY_MAPPING_H
#define _GLEEM_SCREEN_TO_RAY_MAPPING_H

#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "CameraParameters.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

/** This class defines the mapping from normalized screen coordinates
    to a 3D ray based on the given camera parameters. You could
    subclass it to allow for more types of camera frusta, but would
    also have to modify the CameraParameters structure.

    The "normalized" screen coordinates must have the following
    properties:
    
    upper left corner = (-1, 1)
    lower left corner = (-1, -1)
    lower right corner = (1, -1)
    upper right corner = (1, 1)
    center = (0, 0)

    The application is responsible for specifying the window size to
    allow the ManipManager to compute these coordinates. */

class GLEEMDLL ScreenToRayMapping
{
 public:
  virtual void mapScreenToRay(const GleemV2f &screenCoords,
			      const CameraParameters &params,
			      GleemV3f &raySource,
			      GleemV3f &rayDirection) = 0;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_SCREEN_TO_RAY_MAPPING_H
