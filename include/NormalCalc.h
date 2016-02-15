/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_NORMAL_CALC_H
#define _GLEEM_NORMAL_CALC_H


#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

/** Helper class for computing per-vertex normals for sets of
    triangles. */

GLEEM_INTERNAL class GLEEMDLL NormalCalc
{
 public:
  /** Returns newly allocated buffer of GleemV3f's which represent
      faceted (non-averaged) normals, but per-vertex, and newly
      allocated output normal index list. Both are allocated with
      operator new. Performs bounds checking on indices with respect
      to vertex list. Index list must represent triangles; -1 indices
      indicate end of triangle. If index list doesn't represent
      triangles or other error occurred then returns false and all
      outgoing variables are set to NULL. ccw flag indicates whether
      triangles are specified counterclockwise when viewed from top or
      not. */
  static bool computeFacetedNormals(const GleemV3f *vertices,
				    int numVertices,
				    const int *indices,
				    int numIndices,
				    bool ccw,
				    GleemV3f *&outputNormals,
				    int &numNormals,
				    int *&outputNormalIndices,
				    int &numNormalIndices);
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_NORMAL_CALC_H
