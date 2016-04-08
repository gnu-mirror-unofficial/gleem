/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_HOLLOW_CUBE_FACE_H
#define _GLEEM_MANIP_PART_HOLLOW_CUBE_FACE_H

#include <Namespace.h>
#include <GleemDLL.h>
#include <Util.h>
#include <ManipPartTriBased.h>

GLEEM_ENTER_NAMESPACE

/** This class describes a manipulator part that, if replicated six
    times and assembled, can be used to create a hollow cube of width,
    height, and depth 2 units. By default the part is oriented in the
    X-Y plane with outward normal pointing in the +Z direction. */

GLEEM_INTERNAL class GLEEMDLL ManipPartHollowCubeFace : public ManipPartTriBased
{
public:
  ManipPartHollowCubeFace(Manip *parent);
  virtual ~ManipPartHollowCubeFace();

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

#endif  // #defined _GLEEM_MANIP_PART_HOLLOW_CUBE_FACE_H
