/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MATH_UTIL_H
#define _GLEEM_MATH_UTIL_H


#include <math.h>
#include <gleem/Namespace.h>
#include <gleem/GleemDLL.h>
#include <gleem/Util.h>
#include <gleem/Linalg.h>

GLEEM_ENTER_NAMESPACE

/** A class which wraps up a few mathematical utilities. */

GLEEM_INTERNAL class GLEEMDLL MathUtil
{
public:
  // Finds a vector perpendicular to the one passed in and returns it
  // in DST. Does not work if source vector is (0, 0, 0); returns
  // false in this case.
  static bool makePerpendicular(const GleemV3f &src, GleemV3f &dst);
};

#define GLEEM_MIN2(x, y) (((x) < (y)) ? (x) : (y))
#define GLEEM_MAX2(x, y) (((x) > (y)) ? (x) : (y))
#define GLEEM_MIN3(x, y, z) GLEEM_MIN2(GLEEM_MIN2((x), (y)), (z))
#define GLEEM_MAX3(x, y, z) GLEEM_MAX2(GLEEM_MAX2((x), (y)), (z))
#define GLEEM_MIN4(x, y, z, w) GLEEM_MIN2(GLEEM_MIN3((x), (y), (z)), (w))
#define GLEEM_MAX4(x, y, z, w) GLEEM_MAX2(GLEEM_MAX3((x), (y), (z)), (w))

#define GLEEM_CLAMP(x, minVal, maxVal) GLEEM_MIN2((maxVal), GLEEM_MAX2((x), (minVal)))

#ifdef WIN32
// Can you believe Microsoft?
# ifndef M_PI
#  define M_PI		3.14159265358979323846
# endif
#endif

#define GLEEM_RAD_TO_DEG(x) ((x) * 180.0f / M_PI)
#define GLEEM_DEG_TO_RAD(x) ((x) * M_PI / 180.0f)

#define GLEEM_SGN(x) (((x) > 0) ? 1 : (((x) < 0) ? -1 : 0))

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MATH_UTIL_H
