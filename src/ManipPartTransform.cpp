/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <ManipPartTransform.h>

GLEEM_USE_NAMESPACE

ManipPartTransform::ManipPartTransform(Manip *parent) :
  ManipPartGroup(parent)
{
}

ManipPartTransform::~ManipPartTransform()
{
}

void
ManipPartTransform::setTransform(const GleemMat4f &xform)
{
  GleemMat4f totalXform;
  GleemMat4f::mult(xform, offsetTransform, totalXform);
  for (int i = 0; i < getNumParts(); i++)
    getPart(i)->setTransform(totalXform);
}

void
ManipPartTransform::setOffsetTransform(const GleemMat4f &offsetTransform)
{
  this->offsetTransform = offsetTransform;
}

const GleemMat4f &
ManipPartTransform::getOffsetTransform() const
{
  return offsetTransform;
}
