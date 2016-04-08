/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_GROUP_H
#define _GLEEM_MANIP_PART_GROUP_H

#include <vector.h>
#include <Namespace.h>
#include <GleemDLL.h>
#include <Util.h>
#include <ManipPart.h>

GLEEM_ENTER_NAMESPACE

/** This class groups a set of ManipParts. Makes a set of ManipParts
    look like one. */

GLEEM_INTERNAL class GLEEMDLL ManipPartGroup : public ManipPart
{
public:
  ManipPartGroup(Manip *parent);
  virtual ~ManipPartGroup();

  /** Implementation of ManipPart interface */
  virtual void render() const;
  virtual void intersectRay(const GleemV3f &rayStart,
			    const GleemV3f &rayDirection,
			    vector<HitPoint> &results);
  virtual void setTransform(const GleemMat4f &xform);
  virtual void highlight();
  virtual void clearHighlight();
  /** Default is pickable */
  virtual void setPickable(bool pickable);
  virtual bool getPickable() const;
  /** Default is visible */
  virtual void setVisible(bool visible);
  virtual bool getVisible() const;

  // Group-specific functions

  /** Add this part to the group, returning its index. Returns -1 if
      part is NULL. NOTE: Group obtains ownership of ManipPart's
      memory and calls delete on it when the group is destroyed, so
      ManipPart must be allocated with operator new. Multiple
      references are not allowed. */
  int addPart(ManipPart *part);
  /** Remove the first instance (NOTE: you should not be adding
      multiple instances of any ManipPart to this group in the first
      place) of this part from the group. Returns false if part was
      not found, true otherwise. Caller obtains ownership of part's
      memory. */
  bool removePart(ManipPart *part);
  /** Remove the part at index i from the group and returns
      it. Returns NULL if i was out of bounds. Caller obtains
      ownership of the returned ManipPart's memory. */
  ManipPart *removePart(int i);
  int getNumParts();
  /** Returns NULL if i is not within 0..getNumParts() - 1 */
  ManipPart *getPart(int i);
  /** Returns index 0..getNumParts() - 1, or -1 if not found */
  int findPart(ManipPart *part);

private:
  vector<ManipPart *> parts;
  bool visible;
  bool pickable;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MANIP_PART_GROUP_H
