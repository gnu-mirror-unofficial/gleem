/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_RIGHT_TRUNC_PYR_H
#define _GLEEM_RIGHT_TRUNC_PYR_H

#include <gleem/Namespace.h>
#include <gleem/GleemDLL.h>
#include <gleem/Util.h>
#include <gleem/ScreenToRayMapping.h>

GLEEM_ENTER_NAMESPACE

/** The only mapping gleem supports right now -- a right truncated
    pyramid. */

GLEEM_INTERNAL class GLEEMDLL RightTruncPyrMapping : public ScreenToRayMapping
{
public:
  virtual void mapScreenToRay(const GleemV2f &screenCoords,
			      const CameraParameters &params,
			      GleemV3f &raySource,
			      GleemV3f &rayDirection);
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_RIGHT_TRUNC_PYR_H
