/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_LINE_SEG_H
#define _GLEEM_MANIP_PART_LINE_SEG_H

#include <gleem/Namespace.h>
#include <gleem/GleemDLL.h>
#include <gleem/Util.h>
#include <gleem/ManipPart.h>

GLEEM_ENTER_NAMESPACE

/** A line segment from (-1, 0, 0) to (1, 0, 0). */

GLEEM_INTERNAL class GLEEMDLL ManipPartLineSeg : public ManipPart
{
public:
  ManipPartLineSeg(Manip *parent);
  virtual ~ManipPartLineSeg();

  /** Default color is (0.8, 0.8, 0.8) */
  void setColor(const GleemV3f &color);
  const GleemV3f &getColor() const;

  /** Default highlight color is (0.8, 0.8, 0) */
  void setHighlightColor(const GleemV3f &highlightColor);
  const GleemV3f &getHighlightColor() const;

  /** Implementation of ManipPart interface */
  virtual void render() const;
  /** LineSegs don't have any dimension and are therefore
      unpickable. This method is empty. */
  virtual void intersectRay(const GleemV3f &rayStart,
			    const GleemV3f &rayDirection,
			    vector<HitPoint> &results);
  virtual void setTransform(const GleemMat4f &xform);
  virtual void highlight();
  virtual void clearHighlight();
  /** setPickable has no effect since a line segment is never pickable */
  virtual void setPickable(bool pickable);
  /** Always returns false */
  virtual bool getPickable() const;
  /** Default is visible */
  virtual void setVisible(bool visible);
  virtual bool getVisible() const;

private:
  void recalcVertices();

  GleemV3f color;
  GleemV3f highlightColor;
  bool highlighted;
  bool visible;
  /** Current transformation matrix */
  GleemMat4f xform;
  /** Transformed vertices */
  vector<GleemV3f> curVertices;
  /** Transformed normals */
  vector<GleemV3f> curNormals;

  static float verticesAsFloats[][3];
  static GleemV3f *vertices;
  static int numVertices;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MANIP_PART_LINE_SEG_H
