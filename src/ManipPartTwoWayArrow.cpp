/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include "../include/ManipPartTwoWayArrow.h"
#include "../include/NormalCalc.h"

GLEEM_USE_NAMESPACE

float
ManipPartTwoWayArrow::verticesAsFloats[][3] = {
  // Left tetrahedron
  {-1, 0, 0},
  {-0.666666, 0.166666, 0.166666},
  {-0.666666, -0.166666, 0.166666},
  {-0.666666, -0.166666, -0.166666},
  {-0.666666, 0.166666, -0.166666},

  // Box at center
  {-0.666666, 0.041666, 0.0416666},
  {-0.666666, -0.041666, 0.0416666},
  {-0.666666, -0.041666, -0.0416666},
  {-0.666666, 0.041666, -0.0416666},
  {0.666666, 0.041666, 0.0416666},
  {0.666666, -0.041666, 0.0416666},
  {0.666666, -0.041666, -0.0416666},
  {0.666666, 0.041666, -0.0416666},

  // Right tetrahedron
  {0.666666, 0.166666, 0.166666},
  {0.666666, 0.166666, -0.166666},
  {0.666666, -0.166666, -0.166666},
  {0.666666, -0.166666, 0.166666},
  {1, 0, 0}
};
GleemV3f *ManipPartTwoWayArrow::vertices = NULL;
int ManipPartTwoWayArrow::numVertices = 18;

int
ManipPartTwoWayArrow::vertexIndices[] = {
  // Left tetrahedron
  1, 0, 2, -1,
  2, 0, 3, -1,
  3, 0, 4, -1,
  4, 0, 1, -1,
  1, 2, 3, -1,
  1, 3, 4, -1,

  // Box
  5, 7, 6, -1, // left face
  5, 8, 7, -1,
  5, 6, 10, -1, // front face
  5, 10, 9, -1,
  6, 7, 11, -1, // bottom face
  6, 11, 10, -1,
  7, 8, 12, -1, // back face
  7, 12, 11, -1,
  8, 5, 9, -1, // top face
  8, 9, 12, -1,
  9, 10, 11, -1, // right face
  9, 11, 12, -1,

  // Right tetrahedron
  13, 14, 15, -1,
  13, 15, 16, -1,
  17, 14, 13, -1,
  17, 15, 14, -1,
  17, 16, 15, -1,
  17, 13, 16, -1
};
int ManipPartTwoWayArrow::numVertexIndices = 96;

GleemV3f *
ManipPartTwoWayArrow::normals = NULL;
int ManipPartTwoWayArrow::numNormals = 0;

int *ManipPartTwoWayArrow::normalIndices = NULL;
int ManipPartTwoWayArrow::numNormalIndices = 0;

ManipPartTwoWayArrow::ManipPartTwoWayArrow(Manip *parent) :
  ManipPartTriBased(parent)
{
  // FIXME: not thread safe
  if (vertices == NULL)
    {
      vertices = new GleemV3f[numVertices];
      for (int i = 0; i < numVertices; i++)
	vertices[i].setValue(verticesAsFloats[i][0],
			     verticesAsFloats[i][1],
			     verticesAsFloats[i][2]);

      bool result = NormalCalc::computeFacetedNormals(vertices,
						      numVertices,
						      vertexIndices,
						      numVertexIndices,
						      true,
						      normals,
						      numNormals,
						      normalIndices,
						      numNormalIndices);
      assert(result == true);
    }
  setVertices(vertices, numVertices);
  setVertexIndices(vertexIndices, numVertexIndices);
  setNormals(normals, numNormals);
  setNormalIndices(normalIndices, numNormalIndices);
}

ManipPartTwoWayArrow::~ManipPartTwoWayArrow()
{
}
