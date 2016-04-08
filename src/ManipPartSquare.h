/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_SQUARE_H
#define _GLEEM_MANIP_PART_SQUARE_H

#include <Namespace.h>
#include <GleemDLL.h>
#include <Util.h>
#include <ManipPartTriBased.h>

GLEEM_ENTER_NAMESPACE

/** A 2x2 square in the X-Y plane, centered about the origin, with the
    normal pointing in the +Z direction. */

GLEEM_INTERNAL class GLEEMDLL ManipPartSquare : public ManipPartTriBased
{
public:
  ManipPartSquare(Manip *parent);
  virtual ~ManipPartSquare();

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

#endif  // #defined _GLEEM_MANIP_PART_SQUARE_H
