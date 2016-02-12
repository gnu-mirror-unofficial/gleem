/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <assert.h>
#ifdef WIN32
# include <windows.h>
#endif
#include <GL/gl.h>
#include "../include/ManipPartTriBased.h"
#include "../include/RayTriangleIntersection.h"

GLEEM_USE_NAMESPACE

ManipPartTriBased::ManipPartTriBased(Manip *parent) :
  ManipPart(parent)
{
  vertices = NULL;
  numVertices = 0;
  normals = NULL;
  numNormals = 0;
  vertexIndices = NULL;
  numVertexIndices = 0;
  normalIndices = NULL;
  numNormalIndices = 0;
  color.setValue(0.8f, 0.8f, 0.8f);
  highlightColor.setValue(0.8f, 0.8f, 0);
  highlighted = false;
  visible = true;
  pickable = true;
  xform.makeIdent();
}

ManipPartTriBased::~ManipPartTriBased()
{
}

void
ManipPartTriBased::setColor(const GleemV3f &color)
{
  this->color = color;
}

const GleemV3f &
ManipPartTriBased::getColor() const
{
  return color;
}

void
ManipPartTriBased::setHighlightColor(const GleemV3f &highlightColor)
{
  this->highlightColor = highlightColor;
}

void
ManipPartTriBased::render() const
{
  assert(numVertexIndices == numNormalIndices);
  assert((numVertexIndices % 4) == 0);
  assert(numVertices == curVertices.size());
  assert(numNormals == curNormals.size());
  if (!visible)
    return;
  bool lightingOn = true;
  // FIXME: this is too expensive; figure out another way
  //  if (glIsEnabled(GL_LIGHTING))
  //    lightingOn = true;
  GleemV3f *tmpNormals = (GleemV3f *) curNormals.begin();
  GleemV3f *tmpVertices = (GleemV3f *) curVertices.begin();

  if (lightingOn)
    {
      glEnable(GL_COLOR_MATERIAL);
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    }
  glBegin(GL_TRIANGLES);
  if (highlighted)
    glColor3f(highlightColor[0], highlightColor[1], highlightColor[2]);
  else
    glColor3f(color[0], color[1], color[2]);
  int i = 0;
  while (i < numVertexIndices)
    {
      const GleemV3f &n0 = tmpNormals[normalIndices[i]];
      const GleemV3f &v0 = tmpVertices[vertexIndices[i]];
      glNormal3f(n0[0], n0[1], n0[2]);
      glVertex3f(v0[0], v0[1], v0[2]);
      i++;

      const GleemV3f &n1 = tmpNormals[normalIndices[i]];
      const GleemV3f &v1 = tmpVertices[vertexIndices[i]];
      glNormal3f(n1[0], n1[1], n1[2]);
      glVertex3f(v1[0], v1[1], v1[2]);
      i++;

      const GleemV3f &n2 = tmpNormals[normalIndices[i]];
      const GleemV3f &v2 = tmpVertices[vertexIndices[i]];
      glNormal3f(n2[0], n2[1], n2[2]);
      glVertex3f(v2[0], v2[1], v2[2]);
      i+=2;

      //      assert(normalIndices[i] == -1);
      //      assert(vertexIndices[i] == -1);
      //      i++;
    }
  glEnd();
  if (lightingOn)
    glDisable(GL_COLOR_MATERIAL);
}

void
ManipPartTriBased::intersectRay(const GleemV3f &rayStart,
				const GleemV3f &rayDirection,
				vector<HitPoint> &results)
{
  assert(numVertexIndices == numNormalIndices);
  assert((numVertexIndices % 4) == 0);
  assert(numVertices == curVertices.size());
  assert(numNormals == curNormals.size());
  if (!pickable)
    return;
  GleemV3f intPt;
  float t;
  HitPoint hitPt;
  hitPt.manipulator = getParent();
  hitPt.manipPart = this;
  assert(hitPt.manipPart != NULL);
  for (int i = 0; i < numVertexIndices; i+=4)
    {
      int i0 = vertexIndices[i];
      int i1 = vertexIndices[i+1];
      int i2 = vertexIndices[i+2];
      int i3 = vertexIndices[i+3];
      assert(i3 == -1);
      if (RayTriangleIntersection::intersectRayWithTriangle(rayStart,
							    rayDirection,
							    curVertices[i0],
							    curVertices[i1],
							    curVertices[i2],
							    intPt,
							    t)
	  == RayTriangleIntersection::RTI_INTERSECTION)
	{
	  // Check for intersections behind the ray
	  if (t >= 0)
	    {
	      hitPt.rayStart = rayStart;
	      hitPt.rayDirection = rayDirection;
	      hitPt.intPt = intPt;
	      hitPt.t = t;
	      /*
	      cerr << "ManipPartTriBased::intersectRay:" << endl;
	      cerr << "hitPt.manipulator = "
		   << (void *) hitPt.manipulator << endl;
	      cerr << "hitPt.manipPart = "
		   << (void *) hitPt.manipPart << endl;
	      */
	      results.push_back(hitPt);
	    }
	}
    }
}

void
ManipPartTriBased::setTransform(const GleemMat4f &xform)
{
  this->xform = xform;
  recalcVertices();
}

void
ManipPartTriBased::highlight()
{
  highlighted = true;
}

void
ManipPartTriBased::clearHighlight()
{
  highlighted = false;
}

void
ManipPartTriBased::setPickable(bool pickable)
{
  this->pickable = pickable;
}

bool
ManipPartTriBased::getPickable() const
{
  return pickable;
}

void
ManipPartTriBased::setVisible(bool visible)
{
  this->visible = visible;
}

bool
ManipPartTriBased::getVisible() const
{
  return visible;
}

void
ManipPartTriBased::setVertices(GleemV3f *vertices, int numVertices)
{
  this->vertices = vertices;
  this->numVertices = numVertices;
}

GleemV3f *
ManipPartTriBased::getVertices() const
{
  return vertices;
}

int
ManipPartTriBased::getNumVertices() const
{
  return numVertices;
}

void
ManipPartTriBased::setNormals(GleemV3f *normals, int numNormals)
{
  this->normals = normals;
  this->numNormals = numNormals;
}

int
ManipPartTriBased::getNumNormals() const
{
  return numNormals;
}

void
ManipPartTriBased::setVertexIndices(int *vertexIndices, int numVertexIndices)
{
  this->vertexIndices = vertexIndices;
  this->numVertexIndices = numVertexIndices;
}

int *
ManipPartTriBased::getVertexIndices() const
{
  return vertexIndices;
}

int
ManipPartTriBased::getNumVertexIndices() const
{
  return numVertexIndices;

}

void
ManipPartTriBased::setNormalIndices(int *normalIndices, int numNormalIndices)
{
  this->normalIndices = normalIndices;
  this->numNormalIndices = numNormalIndices;
}

int *
ManipPartTriBased::getNormalIndices() const
{
  return normalIndices;
}

int
ManipPartTriBased::getNumNormalIndices() const
{
  return numNormalIndices;

}

void
ManipPartTriBased::recalcVertices()
{
  curVertices.erase(curVertices.begin(), curVertices.end());
  curNormals.erase(curNormals.begin(), curNormals.end());
  GleemV3f v, n;
  GleemV3f vNew, nNew;
  int i;
  for (i = 0; i < numVertices; i++)
    {
      v = vertices[i];
      xform.xformPt(v, vNew);
      curVertices.push_back(vNew);
    }
  for (i = 0; i < numNormals; i++)
    {
      n = normals[i];
      xform.xformDir(n, nNew);
      nNew.normalize();
      curNormals.push_back(nNew);
    }
  assert(numVertexIndices == numNormalIndices);
  assert((numVertexIndices % 4) == 0);
  assert(numVertices == curVertices.size());
  assert(numNormals == curNormals.size());
}
