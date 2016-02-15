/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <iostream>
#include "../include/NormalCalc.h"

GLEEM_USE_NAMESPACE

bool
NormalCalc::computeFacetedNormals(const GleemV3f *vertices,
				  int numVertices,
				  const int *indices,
				  int numIndices,
				  bool ccw,
				  GleemV3f *&outputNormals,
				  int &numNormals,
				  int *&outputNormalIndices,
				  int &numNormalIndices)
{
  if ((numIndices % 4) != 0)
    {
      std::cerr << "NormalCalc::computeFacetedNormals: ERROR: "
	   << "numIndices wasn't divisible by 4, so it can't possibly "
	   << "represent a set of triangles" << std::endl;
      outputNormals = NULL;
      numNormals = 0;
      outputNormalIndices = NULL;
      numNormalIndices = 0;
      return false;
    }
  numNormals = numIndices / 4;
  numNormalIndices = numIndices;
  outputNormals = new GleemV3f[numNormals];
  outputNormalIndices = new int[numNormalIndices];
  int curNormalIdx = 0;
  for (int i = 0; i < numIndices; i += 4)
    {
      int i0 = indices[i];
      int i1 = indices[i+1];
      int i2 = indices[i+2];
      int i3 = indices[i+3];
      if ((i0 < 0) || (i0 >= numVertices) ||
	  (i1 < 0) || (i1 >= numVertices) ||
	  (i2 < 0) || (i2 >= numVertices) ||
	  (i3 != -1))
	{
	  std::cerr << "NormalCalc::computeFacetedNormals: ERROR: "
	       << "vertex index out of bounds or no end of triangle "
	       << "index found" << std::endl;
	  delete[] outputNormals;
	  delete[] outputNormalIndices;
	  outputNormals = NULL;
	  numNormals = 0;
	  outputNormalIndices = NULL;
	  numNormalIndices = 0;
	  return false;
	}
      const GleemV3f &v0 = vertices[i0];
      const GleemV3f &v1 = vertices[i1];
      const GleemV3f &v2 = vertices[i2];
      GleemV3f d1 = v1 - v0;
      GleemV3f d2 = v2 - v0;
      GleemV3f n;
      if (ccw)
	GleemV3f::cross(d1, d2, n);
      else
	GleemV3f::cross(d2, d1, n);
      n.normalize();
      outputNormals[curNormalIdx] = n;
      outputNormalIndices[i] = curNormalIdx;
      outputNormalIndices[i+1] = curNormalIdx;
      outputNormalIndices[i+2] = curNormalIdx;
      outputNormalIndices[i+3] = -1;
      curNormalIdx++;
    }
  return true;
}
