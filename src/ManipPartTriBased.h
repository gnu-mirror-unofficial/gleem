/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM_MANIP_PART_TRI_BASED_H
#define _GLEEM_MANIP_PART_TRI_BASED_H

#include <vector.h>
#include <Namespace.h>
#include <GleemDLL.h>
#include <Util.h>
#include <Manip.h>
#include <Linalg.h>

GLEEM_ENTER_NAMESPACE

/** Triangle-based manipulator part. This is the base class for most of
    the ManipParts that GLEEM uses internally. You can feel free to
    subclass this if you want to replace geometry in the manipulators,
    or re-derive from ManipPart. See ManipPartLineSeg for an example. */

GLEEM_INTERNAL class GLEEMDLL ManipPartTriBased : public ManipPart
{
public:
  ManipPartTriBased(Manip *parent);
  virtual ~ManipPartTriBased();

  /** Default color is (0.8, 0.8, 0.8) */
  void setColor(const GleemV3f &color);
  const GleemV3f &getColor() const;

  /** Default highlight color is (0.8, 0.8, 0) */
  void setHighlightColor(const GleemV3f &highlightColor);
  const GleemV3f &getHighlightColor() const;

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

protected:
  /** Caller retains ownership of memory. */
  void setVertices(GleemV3f *vertices, int numVertices);
  GleemV3f *getVertices() const;
  int getNumVertices() const;
  
  /** Normals are specified per-vertex. Caller retains ownership of
      memory. */
  void setNormals(GleemV3f *normals, int numNormals);
  GleemV3f *getNormals() const;
  int getNumNormals() const;

  /** Creates triangles out of vertex and normal lists. -1 indicates
      end of triangle. Caller retains ownership of memory. */
  void setVertexIndices(int *vertexIndices, int numVertexIndices);
  int *getVertexIndices() const;
  int getNumVertexIndices() const;

  /** Creates triangles out of vertex and normal lists. -1 indicates
      end of triangle. Caller retains ownership of memory. */
  void setNormalIndices(int *normalIndices, int numNormalIndices);
  int *getNormalIndices() const;
  int getNumNormalIndices() const;

private:
  void recalcVertices();

  GleemV3f color;
  GleemV3f highlightColor;
  bool highlighted;
  bool pickable;
  bool visible;
  /** Pointers down to subclass-specific data */
  GleemV3f *vertices;
  int numVertices;
  GleemV3f *normals;
  int numNormals;
  int *vertexIndices;
  int numVertexIndices;
  int *normalIndices;
  int numNormalIndices;
  /** Current transformation matrix */
  GleemMat4f xform;
  /** Transformed vertices */
  vector<GleemV3f> curVertices;
  /** Transformed normals */
  vector<GleemV3f> curNormals;
};

GLEEM_EXIT_NAMESPACE

#endif  // #defined _GLEEM_MANIP_PART_TRI_BASED_H
