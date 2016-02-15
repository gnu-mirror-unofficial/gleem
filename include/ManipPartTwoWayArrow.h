/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_TWO_WAY_ARROW_H
#define _GLEEM_MANIP_PART_TWO_WAY_ARROW_H

#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "ManipPartTriBased.h"

GLEEM_ENTER_NAMESPACE

/** This manipulator part, by default, is aligned with the X axis, has
    its leftmost vertex at (-1, 0, 0) and its rightmost vertex at (1,
    0, 0), and therefore has a total width of 2. */

GLEEM_INTERNAL class GLEEMDLL ManipPartTwoWayArrow : public ManipPartTriBased
{
 public:
  ManipPartTwoWayArrow(Manip *parent);
  virtual ~ManipPartTwoWayArrow();
 private:
  static float verticesAsFloats[][3];
  static GleemV3f *vertices;
  static int numVertices;
  static int vertexIndices[];
  static int numVertexIndices;
  static GleemV3f *normals;
  static int numNormals;
  static int *normalIndices;
  static int numNormalIndices;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MANIP_PART_TWO_WAY_ARROW_H
