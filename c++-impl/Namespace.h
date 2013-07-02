/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_NAMESPACE_H
#define _GLEEM_NAMESPACE_H

#if (!defined(GLEEM_NO_NAMESPACES))
# define GLEEM_ENTER_NAMESPACE namespace gleem {
# define GLEEM_EXIT_NAMESPACE }
# define GLEEM_USE_NAMESPACE using namespace gleem;
#else
# define GLEEM_ENTER_NAMESPACE
# define GLEEM_EXIT_NAMESPACE
# define GLEEM_USE_NAMESPACE
#endif

#endif  // #defined _GLEEM_NAMESPACE_H
