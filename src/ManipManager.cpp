/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifdef WIN32
# include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <algo.h>
#include "../include/ManipManager.h"
#include "../include/RightTruncPyrMapping.h"
#include "../include/Manip.h"

GLEEM_USE_NAMESPACE

static const int MANIP_MANAGER_NUM_WINDOWS = 16; // How many windows do we
						 // expect to be created?
static const int MANIP_MANAGER_NUM_MANIPS = 32; // How many manipulators do we
						// expect to be created?
size_t
ManipManager::hashManip(const Manip * const &arg)
{
  return (int) ((void *) arg);
}

ManipManager *ManipManager::manipManager = NULL;

void
ManipManager::init(bool install)
{
  if (manipManager != NULL)
    {
      cerr << "gleem::ManipManager::init(): (warning/usage) init() called "
	   << "more than once (error, but continuing)" << endl;
    }
  else
    {
      manipManager = new ManipManager();
      if (install)
	manipManager->installGLUTCallbacks(glutGetWindow());
    }
}

ManipManager *
ManipManager::getManipManager()
{
  if (manipManager == NULL)
    {
      cerr << "gleem::ManipManager::getManipManager: WARNING: you probably "
	   << "forgot to call ManipManager::init(), so I'm doing it for you"
	   << endl;
      init();
    }
  return manipManager;
}

void
ManipManager::updateCameraParameters(int windowID,
				     const CameraParameters &params)
{
  WindowToCameraParametersTable::iterator iter =
    windowCameraTable.find(windowID);
  if (iter == windowCameraTable.end())
    {
      cerr << "gleem::ManipManager::updateCameraParameters: ERROR: "
	   << "I got called with a window I had never heard of ("
	   << windowID << ")." << endl
	   << "You need to call windowCreated() when you open a new window."
	   << endl;
    }
  *iter = params;
}

void
ManipManager::render()
{
  for (WindowToManipListTable::iterator windowTableIter =
	 windowManipTable.begin();
       windowTableIter != windowManipTable.end();
       windowTableIter++)
    {
      int windowID = windowTableIter.getKey();
      ManipList &manips = *windowTableIter;
      if (glutGetWindow() != windowID)
	glutSetWindow(windowID);
      for (int i = 0; i < manips.size(); i++)
	manips[i]->render();
    }
}

bool
ManipManager::windowCreated(int windowID)
{
  if (findEntryForWindow(windowID) == true)
    return false;
  createEntryForWindow(windowID);
  return true;
}

bool
ManipManager::windowDestroyed(int windowID)
{
  if (findEntryForWindow(windowID) == false)
    return false;
  removeEntryForWindow(windowID);
  return true;
}

bool
ManipManager::addManipToWindow(Manip *manip, int windowID)
{
  // Ensure no problems later
  createEntryForManip(manip);
  createEntryForWindow(windowID);
  WindowToManipListTable::iterator windowTableIter =
    windowManipTable.find(windowID);
  assert(windowTableIter != windowManipTable.end());
  ManipList &manipList = *windowTableIter;
  if (find(manipList.begin(), manipList.end(), manip) !=
      manipList.end())
    {
      // Ensure invariants
      IntList &windowList = *(manipWindowTable.find(manip));
      assert(find(windowList.begin(), windowList.end(), windowID) !=
	     windowList.end());
      return false;
    }
  // Insert
  manipList.push_back(manip);
  IntList &windowList = *(manipWindowTable.find(manip));
  assert(find(windowList.begin(), windowList.end(), windowID) ==
	 windowList.end());
  windowList.push_back(windowID);
  return true;
}

bool
ManipManager::removeManipFromWindow(Manip *manip, int windowID)
{
  WindowToManipListTable::iterator windowTableIter =
    windowManipTable.find(windowID);
  if (windowTableIter == windowManipTable.end())
    return false;
  ManipList &manipList = *windowTableIter;
  ManipList::iterator manipListIter =
    find(manipList.begin(), manipList.end(), manip);
  if (manipListIter == manipList.end())
    return false;
  manipList.erase(manipListIter);
  // Okay, now remove window from manip's window list
  ManipToWindowListTable::iterator manipTableIter =
    manipWindowTable.find(manip);
  assert(manipTableIter != manipWindowTable.end());
  IntList &windowList = *manipTableIter;
  IntList::iterator windowListIter =
    find(windowList.begin(), windowList.end(), windowID);
  assert(windowListIter != windowList.end());
  windowList.erase(windowListIter);
  return true;
}

void
ManipManager::installGLUTCallbacks(int windowID)
{
  glutSetWindow(windowID);
  glutMouseFunc(mouseFunc);
  glutMotionFunc(motionFunc);
  glutPassiveMotionFunc(passiveMotionFunc);
}

void
ManipManager::mouseFunc(int button, int state, int x, int y)
{
  getManipManager()->mouseMethod(glutGetWindow(), button, state, x, y);
}

void
ManipManager::motionFunc(int x, int y)
{
  getManipManager()->motionMethod(glutGetWindow(), x, y);
}

void
ManipManager::passiveMotionFunc(int x, int y)
{
  getManipManager()->passiveMotionMethod(glutGetWindow(), x, y);
}

ScreenToRayMapping *
ManipManager::getScreenToRayMapping()
{
  return mapping;
}

void
ManipManager::setScreenToRayMapping(ScreenToRayMapping *map)
{
  mapping = map;
}

void
ManipManager::removeManip(Manip *manip)
{
  if (curManip == manip)
    {
      cerr << "ManipManager::removeManip: WARNING: it's a bad idea to delete "
	   << "a manipulator while you're dragging it" << endl;
      curManip = NULL;
    }
  removeEntryForManip(manip);
}

const CameraParameters &
ManipManager::getCameraParameters(int windowID)
{
  WindowToCameraParametersTable::iterator iter =
    windowCameraTable.find(windowID);
  assert(iter != windowCameraTable.end());
  return *iter;
}

ManipManager::ManipManager() :
  windowManipTable(MANIP_MANAGER_NUM_WINDOWS, hash<int>()),
  manipWindowTable(MANIP_MANAGER_NUM_MANIPS, &ManipManager::hashManip),
  windowCameraTable(MANIP_MANAGER_NUM_WINDOWS, hash<int>())
{
  mapping = new RightTruncPyrMapping();
  dragging = false;
  curManip = NULL;
  curHighlightedManip = NULL;
}

void
ManipManager::mouseMethod(int windowID, int button, int state, int x, int y)
{
  WindowToManipListTable::iterator windowTableIter =
    windowManipTable.find(windowID);
  if (windowTableIter == windowManipTable.end())
    {
      cerr << "gleem::ManipManager::mouseMethod: ERROR: "
	   << "I got called from a window I had never heard of ("
	   << windowID << "). " << endl
	   << "You need to call windowCreated() when you open a new window."
	   << endl;
      return;
    }
  ManipList &manips = *windowTableIter;
  const CameraParameters &params = getCameraParameters(windowID);
  if (button == GLUT_LEFT_BUTTON)
    {
      if (state == GLUT_DOWN)
	{
	  // Compute ray in 3D
	  GleemV3f raySource, rayDirection;
	  if (computeRay(params, x, y, raySource, rayDirection) == false)
	    {
	      cerr << "gleem::ManipManager::mouseFunc: ERROR: "
		   << "screen to ray mapping was unspecified" << endl;
	      return;
	    }
	
	  vector<HitPoint> results;
	  // Determine hits
	  int i;
	  for (i = 0; i < manips.size(); i++)
	    manips[i]->intersectRay(raySource, rayDirection, results);
	  // Now find closest one
	  int closestIdx = -1;
	  float closest = 0.0f;
	  for (i = 0; i < results.size(); i++)
	    {
	      if ((closestIdx == -1) ||
		  (results[i].t < closest))
		{
		  closestIdx = i;
		  closest = results[i].t;
		}
	    }
	  if (closestIdx != -1)
	    {
	      if (curHighlightedManip != NULL)
		{
		  curHighlightedManip->clearHighlight();
		  curHighlightedManip = NULL;
		}
	      const HitPoint &hp = results[closestIdx];
	      assert(hp.manipulator != NULL);
	      hp.manipulator->makeActive(hp);
	      curManip = hp.manipulator;
	      dragging = true;
	    }
	}
      else
	{
	  //	  cerr << "Button up" << endl;
	  if (curManip != NULL)
	    {
	      curManip->makeInactive();
	      dragging = false;
	      curManip = NULL;
	      // Check to see where mouse is
	      passiveMotionMethod(windowID, x, y);
	    }
	}
    }
}

void
ManipManager::motionMethod(int windowID, int x, int y)
{
  WindowToManipListTable::iterator windowTableIter =
    windowManipTable.find(windowID);
  if (windowTableIter == windowManipTable.end())
    {
      cerr << "gleem::ManipManager::mouseMethod: ERROR: "
	   << "I got called from a window I had never heard of ("
	   << windowID << "). " << endl
	   << "You need to call windowCreated() when you open a new window."
	   << endl;
      return;
    }
  ManipList &manips = *windowTableIter;
  const CameraParameters &params = getCameraParameters(windowID);
  //  cerr << "motionFunc" << endl;
  if (dragging)
    {
      // Compute ray in 3D
      GleemV3f raySource, rayDirection;
      if (computeRay(params, x, y, raySource, rayDirection) == false)
	{
	  cerr << "gleem::ManipManager::motionFunc: ERROR: "
	       << "screen to ray mapping was unspecified" << endl;
	  return;
	}
      if (curManip == NULL)
	{
	  cerr << "gleem::ManipManager::motionFunc: INTERNAL ERROR: "
	       << "curManip == NULL during drag (did you delete the "
	       << "current manipulator?)" << endl;
	  dragging = false;
	  return;
	}
      curManip->drag(raySource, rayDirection);
    }
}

void
ManipManager::passiveMotionMethod(int windowID, int x, int y)
{
  // FIXME: workaround for minor GLUT bug where passiveMotionFunc is
  // called upon window entry/exit
  if (dragging)
    return;

  WindowToManipListTable::iterator windowTableIter =
    windowManipTable.find(windowID);
  if (windowTableIter == windowManipTable.end())
    {
      cerr << "gleem::ManipManager::mouseMethod: ERROR: "
	   << "I got called from a window I had never heard of ("
	   << windowID << "). " << endl
	   << "You need to call windowCreated() when you open a new window."
	   << endl;
      return;
    }
  ManipList &manips = *windowTableIter;
  const CameraParameters &params = getCameraParameters(windowID);
  //  cerr << "passiveMotionFunc" << endl;
  // Compute ray in 3D
  GleemV3f raySource, rayDirection;
  if (computeRay(params, x, y, raySource, rayDirection) == false)
    {
      cerr << "gleem::ManipManager::passiveMotionFunc: ERROR: "
	   << "screen to ray mapping was unspecified" << endl;
      return;
    }

  vector<HitPoint> results;
  // Determine hits
  int i;
  for (i = 0; i < manips.size(); i++)
    manips[i]->intersectRay(raySource, rayDirection, results);
  // Now find closest one
  int closestIdx = -1;
  float closest = 0.0f;
  for (i = 0; i < results.size(); i++)
    {
      if ((closestIdx == -1) ||
	  (results[i].t < closest))
	{
	  closestIdx = i;
	  closest = results[i].t;
	}
    }
  if (curHighlightedManip != NULL)
    {
      curHighlightedManip->clearHighlight();
    }
  if (closestIdx != -1)
    {
      const HitPoint &hp = results[closestIdx];
      assert(hp.manipulator != NULL);
      assert(hp.manipPart != NULL);
      curHighlightedManip = hp.manipulator;
      curHighlightedManip->highlight(hp);
      //      cerr << "Highlighted manip" << endl;
    }
  else
    curHighlightedManip = NULL;
}

void
ManipManager::createEntryForManip(Manip *manip)
{
  if (manipWindowTable.find(manip) == manipWindowTable.end())
    {
      pair<ManipToWindowListTable::iterator, bool> result =
	manipWindowTable.insert_unique(manip, IntList());
      assert(result.second == true);
    }
}

void
ManipManager::removeEntryForManip(Manip *manip)
{
  ManipToWindowListTable::iterator iter =
    manipWindowTable.find(manip);
  if (iter == manipWindowTable.end())
    return;
  IntList &windows = *iter;
  for (int i = 0; i < windows.size(); i++)
    {
      int windowID = windows[i];
      WindowToManipListTable::iterator windowTableIter =
	windowManipTable.find(windowID);
      assert(windowTableIter != windowManipTable.end());
      ManipList &manipList = *windowTableIter;
      ManipList::iterator manipListIter =
	find(manipList.begin(), manipList.end(), manip);
      assert(manipListIter != manipList.end());
      manipList.erase(manipListIter);
    }
  manipWindowTable.erase(iter);
}

bool
ManipManager::findEntryForManip(Manip *manip)
{
  ManipToWindowListTable::iterator iter =
    manipWindowTable.find(manip);
  if (iter == manipWindowTable.end())
    return false;
  return true;
}

void
ManipManager::createEntryForWindow(int windowID)
{
  if (windowManipTable.find(windowID) == windowManipTable.end())
    {
      pair<WindowToManipListTable::iterator, bool> result =
	windowManipTable.insert_unique(windowID, ManipList());
      assert(result.second == true);
      pair<WindowToCameraParametersTable::iterator, bool> result2 =
	windowCameraTable.insert_unique(windowID, CameraParameters());
      assert(result2.second == true);
    }
}

void
ManipManager::removeEntryForWindow(int windowID)
{
  WindowToManipListTable::iterator iter =
    windowManipTable.find(windowID);
  if (iter == windowManipTable.end())
    return;
  WindowToCameraParametersTable::iterator camIter =
    windowCameraTable.find(windowID);
  assert(camIter != windowCameraTable.end());
  windowCameraTable.erase(camIter);
  ManipList &manips = *iter;
  for (int i = 0; i < manips.size(); i++)
    {
      Manip *manip = manips[i];
      ManipToWindowListTable::iterator manipTableIter =
	manipWindowTable.find(manip);
      assert(manipTableIter != manipWindowTable.end());
      IntList &windowList = *manipTableIter;
      IntList::iterator windowListIter =
	find(windowList.begin(), windowList.end(), windowID);
      assert(windowListIter != windowList.end());
      windowList.erase(windowListIter);
    }
  windowManipTable.erase(iter);
}

bool
ManipManager::findEntryForWindow(int windowID)
{
  WindowToManipListTable::iterator iter =
    windowManipTable.find(windowID);
  if (iter == windowManipTable.end())
    return false;
  return true;
}

GleemV2f
ManipManager::screenToNormalizedCoordinates(const CameraParameters &params,
					    int x, int y)
{
  GleemV2f retval;

  // GLUT's origin is upper left
  retval[0] = (float) x / (float) (params.xSize - 1);
  retval[1] = (float) y / (float) (params.ySize - 1);
  retval[0] = (retval[0] - 0.5f) * 2.0f;
  retval[1] = (0.5f - retval[1]) * 2.0f;
  return retval;
}

bool
ManipManager::computeRay(const CameraParameters &params,
			 int x, int y,
			 GleemV3f &raySource,
			 GleemV3f &rayDirection)
{
  if (mapping == NULL)
    return false;
  mapping->mapScreenToRay(screenToNormalizedCoordinates(params, x, y),
			  params,
			  raySource,
			  rayDirection);
  return true;
}
