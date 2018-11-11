/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <gleem/ManipPartSquare.h>
#include <gleem/NormalCalc.h>

GLEEM_USE_NAMESPACE

float
ManipPartSquare::verticesAsFloats[][3] = {
  // Counter-clockwise, starting from upper left
  {-1, 1, 0},
  {-1, -1, 0},
  {1, -1, 0},
  {1, 1, 0}
};
GleemV3f *ManipPartSquare::vertices = NULL;
int ManipPartSquare::numVertices = 4;

int
ManipPartSquare::vertexIndices[] = {
  // Just the front face
  0, 1, 2, -1,
  0, 2, 3, -1
};
int ManipPartSquare::numVertexIndices = 8;

GleemV3f *
ManipPartSquare::normals = NULL;
int ManipPartSquare::numNormals = 0;

int *ManipPartSquare::normalIndices = NULL;
int ManipPartSquare::numNormalIndices = 0;

ManipPartSquare::ManipPartSquare(Manip *parent) :
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

ManipPartSquare::~ManipPartSquare()
{
}
