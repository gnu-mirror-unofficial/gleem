/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <gleem/ManipPartLineSeg.h>
#ifdef WIN32
# include <windows.h>
#endif
#include <GL/gl.h>

GLEEM_USE_NAMESPACE

float
ManipPartLineSeg::verticesAsFloats[][3] = {
  {-1, 0, 0},
  {1, 0, 0}
};
GleemV3f *ManipPartLineSeg::vertices = NULL;
int ManipPartLineSeg::numVertices = 2;

ManipPartLineSeg::ManipPartLineSeg(Manip *parent) :
  ManipPart(parent)
{
  // FIXME: not thread safe
  if (vertices == NULL)
    {
      vertices = new GleemV3f[numVertices];
      for (int i = 0; i < numVertices; i++)
	vertices[i].setValue(verticesAsFloats[i][0],
			     verticesAsFloats[i][1],
			     verticesAsFloats[i][2]);
    }
  color.setValue(0.8f, 0.8f, 0.8f);
  highlightColor.setValue(0.8f, 0.8f, 0);
  highlighted = false;
  visible = true;
  xform.makeIdent();
  recalcVertices();
}

ManipPartLineSeg::~ManipPartLineSeg()
{
}

void
ManipPartLineSeg::setColor(const GleemV3f &color)
{
  this->color = color;
}

const GleemV3f &
ManipPartLineSeg::getColor() const
{
  return color;
}

void
ManipPartLineSeg::setHighlightColor(const GleemV3f &highlightColor)
{
  this->highlightColor = highlightColor;
}

void
ManipPartLineSeg::render() const
{
  if (!visible)
    return;
  // FIXME: probably too slow
  bool reenable = glIsEnabled(GL_LIGHTING);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  if (highlighted)
    glColor3f(highlightColor[0], highlightColor[1], highlightColor[2]);
  else
    glColor3f(color[0], color[1], color[2]);
  GleemV3f *tmpVertices = (GleemV3f *) curVertices.begin();
  assert(numVertices == curVertices.size());
  for (int i = 0; i < numVertices; i++)
    {
      const GleemV3f &v = tmpVertices[i];
      glVertex3f(v[0], v[1], v[2]);
    }
  glEnd();
  if (reenable)
    glEnable(GL_LIGHTING);
}

void
ManipPartLineSeg::intersectRay(const GleemV3f &rayStart,
			       const GleemV3f &rayDirection,
			       vector<HitPoint> &results)
{
}

void
ManipPartLineSeg::setTransform(const GleemMat4f &xform)
{
  this->xform = xform;
  recalcVertices();
}

void
ManipPartLineSeg::highlight()
{
  highlighted = true;
}

void
ManipPartLineSeg::clearHighlight()
{
  highlighted = false;
}

void
ManipPartLineSeg::setPickable(bool pickable)
{
}

bool
ManipPartLineSeg::getPickable() const
{
  return false;
}

void
ManipPartLineSeg::setVisible(bool visible)
{
  this->visible = visible;
}

bool
ManipPartLineSeg::getVisible() const
{
  return visible;
}

void
ManipPartLineSeg::recalcVertices()
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
  assert(numVertices == curVertices.size());
}
