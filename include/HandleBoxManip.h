/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_HANDLE_BOX_MANIP_H
#define _GLEEM_HANDLE_BOX_MANIP_H

#include "Namespace.h"
#include "GleemDLL.h"
#include "Util.h"
#include "Manip.h"
#include "Line.h"
#include "Plane.h"
#include "PlaneUV.h"
#include "Linalg.h"

GLEEM_ENTER_NAMESPACE

/** Patterned after Inventor's HandleBoxManip (by Paul Isaacs and
    David Mott) and TransformerManip (by Paul Isaacs). Center box
    allows 3-D translation. Outer six handles allow rotation about the
    box's six axes. When a handle is clicked, the axis of rotation is
    immediately chosen as that which is most parallel to the viewing
    direction (note there are at most two possibilities for the axis
    of each handle's rotation). Eight corner handles allow aspect
    ratio-preserving scaling; shift-dragging these handles allows
    non-uniform scaling in one of two possible directions using a
    "snap-to-axis" paradigm. These two directions are chosen as the
    axes of the adjacent face to the handle whose normal most directly
    faces the viewing direction. */

class GLEEMDLL HandleBoxManip : public Manip
{
public:
  /** Default HandleBoxManip has translation (0, 0, 0) and the
      identity orientation */
  HandleBoxManip();
  virtual ~HandleBoxManip();

  /** Set the translation of this HandleBoxManip. This moves its
      on-screen representation. Manipulations cause the translation to
      be modified, not overwritten, so if you want the default
      HandleBoxManip to go through the point (0, 1, 0) but still
      translate in the X-Z plane, then setTranslation(0, 1, 0). */
  void setTranslation(const GleemV3f &translation);

  /** Get the translation of this HandleBoxManip. This corresponds to
      the center of the box. */
  const GleemV3f &getTranslation() const;
  
  /** Set the rotation of this HandleBoxManip. */
  void setRotation(const GleemRot &rotation);

  /** Get the rotation of this HandleBoxManip. */
  const GleemRot &getRotation() const;

  /** Set the scale of the HandleBoxManip. This corresponds to the
      scaling the user has performed. */
  void setScale(const GleemV3f &scale);
  const GleemV3f &getScale() const;

  /** Set the scale of the HandleBoxManip's geometry. This only
      affects its on-screen representation. It is probably a bad idea
      to use a non-uniform scale here, because it'd be very confusing
      to the user. None of the components of the geometryScale vector
      may be negative. */
  void setGeometryScale(const GleemV3f &geometryScale);
  const GleemV3f &getGeometryScale() const;

GLEEM_INTERNAL public:
  /** Implementation of Manip interface */
  virtual void render();
  virtual void intersectRay(const GleemV3f &rayStart,
			    const GleemV3f &rayDirection,
			    vector<HitPoint> &results);
  virtual void highlight(const HitPoint &hit);
  virtual void clearHighlight();
  virtual void makeActive(const HitPoint &hit);
  virtual void drag(const GleemV3f &rayStart,
		    const GleemV3f &rayDirection);
  virtual void makeInactive();

private:
  void deleteGeometry();
  void createGeometry();
  void recalc();

  // Member data
  GleemV3f translation;
  GleemV3f scale;
  GleemV3f geometryScale;
  GleemRot rotation;
  // Cumulative transform of this object
  GleemMat4f xform;

  typedef enum
  {
    INACTIVE,
    TRANSLATE,
    ROTATE,
    // Scaling based on all three axes, preserving current aspect ratio
    SCALE_XYZ,
    // Scaling along one of two axes (shift key + drag scale handle)
    SCALE_SINGLE_AXIS
  } DragState;
  DragState dragState;
  Plane dragPlane;
  GleemV3f dragOffset;

  typedef enum
  {
    SCALE_XY,
    SCALE_YZ,
    SCALE_ZX
  } ScaleAxes;

  /** All of the line segments comprising the faces */
  ManipPart *lineSegs[12];

  class FaceInfo
  {
  public:
    ManipPart *lineSegs[4];
    /** The invisible square comprising this face's invisible, but
	pickable, geometry */
    ManipPart *centerSquare;
    GleemV3f origNormal;
    GleemV3f normal;
    ScaleAxes scaleAxes;
  };
  vector<FaceInfo> faces;
  vector<ManipPart *> highlightedGeometry;
  vector<ManipPart *> draggedGeometry;
  ManipPart *createLineSeg(const GleemV3f &translation,
			   const GleemV3f &xAxis,
			   const GleemV3f &yAxis);
  ManipPart *createFace(const GleemV3f &translation,
			const GleemV3f &normal,
			const GleemV3f &up);

  /** Each rotation handle points off to two faces corresponding to
      the planes in which that handle can rotate. It also points to
      two circles which appear during dragging to indicate to the user
      in which plane the manipulator is being rotated. */
  class RotateHandleInfo
  {
  public:
    ManipPart *geometry;
    int faceIdx0;
    //    int circleIdx0;
    int faceIdx1;
    //    int circleIdx1;
  };
  vector<RotateHandleInfo> rotateHandles;
  ManipPart *createRotateHandle(const GleemV3f &direction);
  PlaneUV rotatePlane;
  float startAngle;
  GleemRot startRot;

  ScaleAxes scaleAxes;
  /** Each scale handle points off to its three adjacent faces. */
  class ScaleHandleInfo
  {
  public:
    ManipPart *geometry;
    /** The indices of the three adjacent faces */
    int faceIndices[3];
  };
  vector<ScaleHandleInfo> scaleHandles;
  ManipPart *createScaleHandle(const GleemV3f &direction);
  // State variables for XYZ scaling
  /** This line's direction must point outward from the center of the
      cube. */
  Line scaleXYZLine;
  float origScaleLen;
  GleemV3f origScale;
  // State variables for single-axis scaling
  PlaneUV scaleAxisPlane;
  GleemV3f scaleAxisOffset;
  GleemV2f scaleAxisOrigUV;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_HANDLE_BOX_MANIP_H
