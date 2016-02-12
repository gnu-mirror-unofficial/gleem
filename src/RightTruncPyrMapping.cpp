/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <math.h>
#include <iostream>
#include <gleem/RightTruncPyrMapping.h>

GLEEM_USE_NAMESPACE

void
RightTruncPyrMapping::mapScreenToRay(const GleemV2f &screenCoords,
				     const CameraParameters &params,
				     GleemV3f &raySource,
				     GleemV3f &rayDirection)
{
  GleemV3f fwd, up, right;
  fwd = params.forwardDirection;
  up = params.upDirection;
  GleemV3f::cross(fwd, up, right);
  fwd.normalize();
  up.normalize();
  right.normalize();
  float horizFOV = atan(params.imagePlaneAspectRatio * tan(params.vertFOV));
  right *= (tan(horizFOV) * screenCoords[0]);
  up *= (tan(params.vertFOV) * screenCoords[1]);
  raySource = params.position;
  rayDirection = fwd + up + right;
}
