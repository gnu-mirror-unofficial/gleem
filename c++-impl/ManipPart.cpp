/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <gleem/ManipPart.h>

GLEEM_USE_NAMESPACE

ManipPart::ManipPart(Manip *parent)
{
  setParent(parent);
}

ManipPart::~ManipPart()
{
}

void
ManipPart::setParent(Manip *parent)
{
  this->parent = parent;
}

Manip *
ManipPart::getParent() const
{
  return parent;
}
