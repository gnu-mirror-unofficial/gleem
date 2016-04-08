/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <ManipPartHollowCubeFace.h>
#include <NormalCalc.h>

GLEEM_USE_NAMESPACE

float
ManipPartHollowCubeFace::verticesAsFloats[][3] = {
  // Outer square, counter-clockwise, starting from upper left
  {-1, 1, 0},
  {-1, -1, 0},
  {1, -1, 0},
  {1, 1, 0},

  // Inner square, counter-clockwise, starting from upper left
  {-0.9f, 0.9f, 0},
  {-0.9f, -0.9f, 0},
  {0.9f, -0.9f, 0},
  {0.9f, 0.9f, 0}
};
GleemV3f *ManipPartHollowCubeFace::vertices = NULL;
int ManipPartHollowCubeFace::numVertices = 8;

int
ManipPartHollowCubeFace::vertexIndices[] = {
  // Need both front and back faces.
  // Front face
  4, 0, 1, -1,
  4, 1, 5, -1,
  5, 1, 2, -1,
  5, 2, 6, -1,
  6, 2, 3, -1,
  6, 3, 7, -1,
  7, 3, 0, -1,
  7, 0, 4, -1
};
int ManipPartHollowCubeFace::numVertexIndices = 32;

GleemV3f *
ManipPartHollowCubeFace::normals = NULL;
int ManipPartHollowCubeFace::numNormals = 0;

int *ManipPartHollowCubeFace::normalIndices = NULL;
int ManipPartHollowCubeFace::numNormalIndices = 0;

ManipPartHollowCubeFace::ManipPartHollowCubeFace(Manip *parent) :
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

ManipPartHollowCubeFace::~ManipPartHollowCubeFace()
{
}
