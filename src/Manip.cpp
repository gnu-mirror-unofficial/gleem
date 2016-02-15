/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifdef WIN32
# include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "../include/Manip.h"
#include "../include/ManipManager.h"

GLEEM_USE_NAMESPACE

void
Manip::addMotionCallback(ManipCB *f, void *userData)
{
  if (f == NULL)
    {
      std::cerr << "Manip::addMotionCallback: ERROR: detected NULL callback function."
	   << std::endl;
      return;
    }
  motionCallbacks.push_back(pair<ManipCB *, void *>(f, userData));
}

void
Manip::removeMotionCallback(ManipCB *f, void *userData)
{
  vector<CallbackInfo>::iterator iter = motionCallbacks.begin();
  while (iter != motionCallbacks.end())
    {
      CallbackInfo &info = *iter;
      if ((info.first == f) && (info.second == userData))
	{
	  motionCallbacks.erase(iter);
	  return;
	}
    }
}

Manip::Manip()
{
  ManipManager::getManipManager()->addManipToWindow(this, glutGetWindow());
}

Manip::~Manip()
{
  ManipManager::getManipManager()->removeManip(this);
}

void
Manip::drag(const GleemV3f &rayStart,
	    const GleemV3f &rayDirection)
{
  for (int i = 0; i < motionCallbacks.size(); i++)
    (*motionCallbacks[i].first)(motionCallbacks[i].second, this);
}
