/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <ManipPartCube.h>
#include <NormalCalc.h>

GLEEM_USE_NAMESPACE

float
ManipPartCube::verticesAsFloats[][3] = {
  // Front side
  {-1, 1, 1},
  {-1, -1, 1},
  {1, -1, 1},
  {1, 1, 1},
  // Back side
  {-1, 1, -1},
  {-1, -1, -1},
  {1, -1, -1},
  {1, 1, -1}
};
GleemV3f *ManipPartCube::vertices = NULL;
int ManipPartCube::numVertices = 8;

int
ManipPartCube::vertexIndices[] = {
  // Front face
  0, 1, 2, -1,
  0, 2, 3, -1,
  // Right face
  3, 2, 6, -1,
  3, 6, 7, -1,
  // Back face
  7, 6, 5, -1,
  7, 5, 4, -1,
  // Left face
  4, 5, 1, -1,
  4, 1, 0, -1,
  // Top face
  4, 0, 3, -1,
  4, 3, 7, -1,
  // Bottom face
  1, 5, 6, -1,
  1, 6, 2, -1
};
int ManipPartCube::numVertexIndices = 48;

GleemV3f *
ManipPartCube::normals = NULL;
int ManipPartCube::numNormals = 0;

int *ManipPartCube::normalIndices = NULL;
int ManipPartCube::numNormalIndices = 0;

ManipPartCube::ManipPartCube(Manip *parent) :
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

ManipPartCube::~ManipPartCube()
{
}
