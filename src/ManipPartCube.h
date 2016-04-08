/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_CUBE_H
#define _GLEEM_MANIP_PART_CUBE_H

#include <Namespace.h>
#include <GleemDLL.h>
#include <Util.h>
#include <ManipPartTriBased.h>

GLEEM_ENTER_NAMESPACE

/** A cube of width, height, and depth 2, centered about the origin
    and aligned with the X, Y, and Z axes. */

GLEEM_INTERNAL class GLEEMDLL ManipPartCube : public ManipPartTriBased
{
public:
  ManipPartCube(Manip *parent);
  virtual ~ManipPartCube();

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

#endif  // #defined _GLEEM_MANIP_PART_CUBE_H
