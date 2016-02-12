/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_LINALG_H
#define _GLEEM_LINALG_H

#ifndef HAVE_VRML_API

#include <gleem/_Linalg.h>

#define GleemV3f _GleemV3f
#define GleemV2f _GleemV2f
#define GleemMat4f _GleemMat4f
#define GleemMat3f _GleemMat3f
#define GleemMat2f _GleemMat2f
#define GleemRot _GleemRot

#else

#include <VrmlVecs.h>

#define GleemV3f VrmlV3f
#define GleemV2f VrmlV2f
#define GleemMat4f VrmlMat4f
#define GleemMat3f VrmlMat3f
#define GleemMat2f VrmlMat2f
#define GleemRot VrmlRot

#endif

#endif  // #defined _GLEEM_LINALG_H
