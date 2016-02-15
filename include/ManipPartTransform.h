/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_TRANSFORM_H
#define _GLEEM_MANIP_PART_TRANSFORM_H

#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "ManipPartGroup.h"

GLEEM_ENTER_NAMESPACE

/** A Group which contains an offset transformation which is performed
    before the one handed down in setTransform. */

GLEEM_INTERNAL class GLEEMDLL ManipPartTransform : public ManipPartGroup
{
 public:
  ManipPartTransform(Manip *parent);
  virtual ~ManipPartTransform();

  /** Inherit everything but setTransform from ManipPartGroup */
  virtual void setTransform(const GleemMat4f &xform);

  // Transform-specific functions

  void setOffsetTransform(const GleemMat4f &offsetTransform);
  const GleemMat4f &getOffsetTransform() const;

 private:
  GleemMat4f offsetTransform;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MANIP_PART_TRANSFORM_H
