/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <algo.h>
#include <gleem/ManipPartGroup.h>

GLEEM_USE_NAMESPACE

ManipPartGroup::ManipPartGroup(Manip *parent) :
  ManipPart(parent)
{
  visible = true;
  pickable = true;
}

ManipPartGroup::~ManipPartGroup()
{
  for (int i = 0; i < parts.size(); i++)
    delete parts[i];
}

void
ManipPartGroup::render() const
{
  if (visible)
    for (int i = 0; i < parts.size(); i++)
      parts[i]->render();
}

void
ManipPartGroup::intersectRay(const GleemV3f &rayStart,
			     const GleemV3f &rayDirection,
			     vector<HitPoint> &results)
{
  if (!pickable)
    return;
  int topIdx = results.size();
  int i;
  for (i = 0; i < parts.size(); i++)
    parts[i]->intersectRay(rayStart, rayDirection, results);
  // Fix up all HitPoints so we appear to be the manipulator part
  // which caused the intersection
  for (i = topIdx; i < results.size(); i++)
    results[i].manipPart = this;
}

void
ManipPartGroup::setTransform(const GleemMat4f &xform)
{
  for (int i = 0; i < parts.size(); i++)
    parts[i]->setTransform(xform);
}

void
ManipPartGroup::highlight()
{
  for (int i = 0; i < parts.size(); i++)
    parts[i]->highlight();
}

void
ManipPartGroup::clearHighlight()
{
  for (int i = 0; i < parts.size(); i++)
    parts[i]->clearHighlight();
}

void
ManipPartGroup::setPickable(bool pickable)
{
  this->pickable = pickable;
}

bool
ManipPartGroup::getPickable() const
{
  return pickable;
}

void
ManipPartGroup::setVisible(bool visible)
{
  this->visible = visible;
}

bool
ManipPartGroup::getVisible() const
{
  return visible;
}

int
ManipPartGroup::addPart(ManipPart *part)
{
  if (part == NULL)
    return -1;
  parts.push_back(part);
  return parts.size() - 1;
}

bool
ManipPartGroup::removePart(ManipPart *part)
{
  vector<ManipPart *>::iterator iter =
    find(parts.begin(), parts.end(), part);
  if (iter == parts.end())
    return false;
  parts.erase(iter);
  return true;
}

ManipPart *
ManipPartGroup::removePart(int i)
{
  if ((i < 0) || (i >= parts.size()))
    return NULL;
  ManipPart *part = parts[i];
  parts.erase(parts.begin() + i);
  return part;
}

int
ManipPartGroup::getNumParts()
{
  return parts.size();
}

ManipPart *
ManipPartGroup::getPart(int i)
{
  if ((i < 0) || (i >= parts.size()))
    return NULL;
  return parts[i];
}

int
ManipPartGroup::findPart(ManipPart *part)
{
  vector<ManipPart *>::iterator iter =
    find(parts.begin(), parts.end(), part);
  if (iter == parts.end())
    return -1;
  return (iter - parts.begin());
}
