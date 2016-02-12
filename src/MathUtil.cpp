/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include "../include/MathUtil.h"

GLEEM_USE_NAMESPACE

bool
MathUtil::makePerpendicular(const GleemV3f &src,
			    GleemV3f &dst)
{
  if ((src[0] == 0.0f) && (src[1] == 0.0f) && (src[2] == 0.0f))
    return false;
  if (src[0] != 0.0f)
    {
      if (src[1] != 0.0f)
	{
	  dst.setValue(-src[1], src[0], 0.0f);
	}
      else
	{
	  dst.setValue(-src[2], 0.0f, src[0]);
	}
    }
  else
    {
      dst.setValue(1.0f, 0.0f, 0.0f);
    }
  return true;
}

