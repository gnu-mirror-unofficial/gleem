/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#ifndef _GLEEM__LINALG_H
#define _GLEEM__LINALG_H

#include <GleemDLL.h>
#include <iostream>
#include <bool.h>

class _GleemRot;
class _GleemMat4f;

// This is not close to a complete linear algebra library, and was not
// intended to be. I would discourage users from applying this code
// outside this library; most of it is straightforward, but parts like
// the matrix inverses are almost guaranteed to be poorly conditioned.

/** 3-element vector. */

class GLEEMDLL _GleemV3f {
 public:
  _GleemV3f();
  _GleemV3f(const _GleemV3f &arg);
  _GleemV3f(float x, float y, float z);
  _GleemV3f &operator=(const _GleemV3f &arg);
  int operator ==(const _GleemV3f &arg) const;
  float &operator[](int i);
  const float &operator[](int i) const;
  void setValue(float x, float y, float z);
  void setValue(const _GleemV3f &arg);
  const float *getValue() const;

  float dot(const _GleemV3f &arg) const;
  float length() const;
  float lengthSquared() const;
  void normalize();

  // operators for convenience. Most are not as efficient as the
  // statics because they return temporaries on the stack.

  /** this + a */
  _GleemV3f operator +(const _GleemV3f &a) const;
  /** this - a */
  _GleemV3f operator -(const _GleemV3f &a) const;
  /** this += a; returns reference to (mutated) this */
  _GleemV3f &operator +=(const _GleemV3f &a);
  /** this -= a; returns reference to (mutated) this */
  _GleemV3f &operator -=(const _GleemV3f &a);
  /** this * scalar */
  _GleemV3f operator *(float a) const;
  /** scalar * vector */
  friend _GleemV3f operator *(float a, const _GleemV3f &v);
  /** this *= scalar; returns reference to (mutated) this */
  _GleemV3f &operator *=(float a);

  // statics

  /** c = a + b. Can use source for dest. */
  static void add(const _GleemV3f &a, const _GleemV3f &b, _GleemV3f &c);
  /** c = a + s * b. Can use source for dest. */
  static void addScaled(const _GleemV3f &a, float s,
			const _GleemV3f &b, _GleemV3f &c);
  /** c = a - b. Can use source for dest. */
  static void sub(const _GleemV3f &a, const _GleemV3f &b, _GleemV3f &c);
  /** c = a * s. Can use source for dest. */
  static void scale(const _GleemV3f &a, float s, _GleemV3f &c);
  /** c = a cross b (CAN NOT USE SOURCE FOR DEST) */
  static void cross(const _GleemV3f &a, const _GleemV3f &b, _GleemV3f &c);

  /** Convenience operator. Puts vector on output stream looking like
      "[ v0, v1, v2 ]". */
  friend std::ostream &operator<<(std::ostream &os, const _GleemV3f &vec);

 private:
  float val[3];
};

/** 2-element vector */

class GLEEMDLL _GleemV2f {
public:
  _GleemV2f();
  _GleemV2f(const _GleemV2f &arg);
  _GleemV2f(float x, float y);
  float &operator[](int i);
  const float &operator[](int i) const;
  int operator ==(const _GleemV2f &arg) const;
  void setValue(float x, float y);
  void setValue(const _GleemV2f &arg);
  
  float dot(const _GleemV2f &arg) const;
  float length() const;
  float lengthSquared() const;
  void normalize();

  // operators for convenience. Most are not as efficient as the
  // statics because they return temporaries on the stack.

  /** this + a */
  _GleemV2f operator +(const _GleemV2f &a) const;
  /** this - a */
  _GleemV2f operator -(const _GleemV2f &a) const;
  /** this += a; returns reference to (mutated) this */
  _GleemV2f &operator +=(const _GleemV2f &a);
  /** this -= a; returns reference to (mutated) this */
  _GleemV2f &operator -=(const _GleemV2f &a);
  /** this * scalar */
  _GleemV2f operator *(float a) const;
  /** scalar * vector */
  friend _GleemV2f operator *(float a, const _GleemV2f &v);
  /** this *= scalar; returns reference to (mutated) this */
  _GleemV2f &operator *=(float a);

  // statics

  /** c = a + b. Can use source for dest. */
  static void add(const _GleemV2f &a, const _GleemV2f &b, _GleemV2f &c);
  /** c = a + s * b. Can use source for dest. */
  static void addScaled(const _GleemV2f &a, float s,
			const _GleemV2f &b, _GleemV2f &c);
  /** c = a - b. Can use source for dest. */
  static void sub(const _GleemV2f &a, const _GleemV2f &b, _GleemV2f &c);
  /** c = a * s. Can use source for dest. */
  static void scale(const _GleemV2f &a, float s, _GleemV2f &c);

  /** Convenience operator. Puts vector on output stream looking like
      "[ v0, v1 ]". */
  friend std::ostream &operator<<(std::ostream &os, const _GleemV2f &vec);

 private:
  float val[2];
};

/** A (very incomplete) 4x4 matrix class. Representation assumes
    multiplication by column vectors on the right. Storage is in row
    major order. That is, mat[0][2] means the zeroth row, second
    column. */

class GLEEMDLL _GleemMat4f {
public:
  /** Constructor does nothing; i.e., matrix contents will be garbage.
      Call makeIdent(), below, to initialize to identity. */
  _GleemMat4f();
  /** Initialize to the identity matrix. */
  void makeIdent();
  /** Sets the translation component of this matrix (i.e., the three
      top elements of the third column) without touching any of the
      other parts of the matrix */
  void setTranslation(const _GleemV3f &trans);
  /** Sets the rotation component of this matrix (i.e., the upper left
      3x3) without touching any of the other parts of the matrix */
  void setRotation(const _GleemRot &rot);
  /** Gets the upper left 3x3 of this matrix as a rotation. Currently
      does not work if there are scales. Ignores translation component. */
  void getRotation(_GleemRot &rot) const;
  /** Get the ith row (i=0..3) of this matrix. */
  const float *operator[](int i) const;
  /** Get the ith row (i=0..3) of this matrix. */
  float *operator[](int i);
  /** Inverts this matrix assuming that it represents a rigid
      transform (i.e., some combination of rotations and
      translations). Assumes column vectors. Algorithm: transposes
      upper left 3x3; negates translation in rightmost column and
      transforms by inverted rotation. */
  void invertRigid();
  /** C = A * B. NOTE: CAN NOT USE A SRC FOR THE DEST. */
  static void mult(const _GleemMat4f &a, const _GleemMat4f &b, _GleemMat4f &c);
  /** NOTE: ignores homogeneous coordinate, i.e., is not a full
      transformation.  NOTE: CAN NOT USE SRC FOR DEST. */
  void xformPt(const _GleemV3f &src, _GleemV3f &dest) const;
  /** Transforms SRC using only the upper left 3x3. NOTE: CAN NOT USE
      SRC FOR DEST. */
  void xformDir(const _GleemV3f &src, _GleemV3f &dest) const;
private:
  float m[4][4];
};

/** 3x3 matrix class useful for simple linear algebra. Representation
    is (as _GleemMat4f) in row major order and assumes multiplication by
    column vectors on the right. */

class GLEEMDLL _GleemMat3f {
public:
  /** Constructor does nothing; i.e., matrix contents will be garbage.
      Call makeIdent(), below, to initialize to identity. */
  _GleemMat3f();
  /** Initialize to the identity matrix. */
  void makeIdent();
  /** Get the ith row (i=0..2) of this matrix. */
  const float *operator[](int i) const;
  /** Get the ith row (i=0..3) of this matrix. */
  float *operator[](int i);
  /** Set column i (i=[0..2]) to vector v. Asserts that i is in
      bounds. */
  void setCol(int i, const _GleemV3f &v);
  /** Set row i (i=[0..2]) to vector v. Asserts that i is in
      bounds. */
  void setRow(int i, const _GleemV3f &v);
  /** Transpose this matrix in place.  */
  void transpose();
  /** Return the determinant. Computed across thw zeroth row. */
  float determinant() const;
  /** Full matrix inversion in place. If matrix is singular, returns
      false and matrix contents are untouched. If you know the matrix
      is orthonormal, you can call transpose() instead. */
  bool invert();
  /** Multiply a 3D vector by this matrix. CAN NOT USE SRC FOR DEST. */
  void xformVec(const _GleemV3f &src, _GleemV3f &dest) const;
  /** C = A * B. NOTE: CAN NOT USE A SRC FOR THE DEST. */
  static void mult(const _GleemMat3f &a, const _GleemMat3f &b, _GleemMat3f &c);
  /** Convenience operator. Returns this * arg, in that order. */
  _GleemMat3f operator*(const _GleemMat3f &arg) const;
private:
  float m[3][3];
};

/** 2x2 matrix class useful for simple linear algebra. Representation
    is (as _GleemMat4f) in row major order and assumes multiplication by
    column vectors on the right. */

class GLEEMDLL _GleemMat2f {
public:
  /** Constructor does nothing; i.e., matrix contents will be garbage.
      Call makeIdent(), below, to initialize to identity. */
  _GleemMat2f();
  /** Initialize to the identity matrix. */
  void makeIdent();
  /** Get the ith row (i=0..1) of this matrix. */
  const float *operator[](int i) const;
  /** Get the ith row (i=0..1) of this matrix. */
  float *operator[](int i);
  /** Set column i (i=[0..1]) to vector v. Asserts that i is in
      bounds. */
  void setCol(int i, const _GleemV2f &v);
  /** Set row i (i=[0..1]) to vector v. Asserts that i is in
      bounds. */
  void setRow(int i, const _GleemV2f &v);
  /** Transpose this matrix in place.  */
  void transpose();
  /** Return the determinant. */
  float determinant() const;
  /** Full matrix inversion in place. If matrix is singular, returns
      false and matrix contents are untouched. If you know the matrix
      is orthonormal, you can call transpose() instead. */
  bool invert();
  /** Multiply a 2D vector by this matrix. CAN NOT USE SRC FOR DEST. */
  void xformVec(const _GleemV2f &src, _GleemV2f &dest) const;
  /** C = A * B. NOTE: CAN NOT USE A SRC FOR THE DEST. */
  static void mult(const _GleemMat2f &a, const _GleemMat2f &b, _GleemMat2f &c);
  /** Convenience operator. Returns this * arg, in that order. */
  _GleemMat2f operator *(const _GleemMat2f &arg) const;
private:
  float m[3][3];
};

/** Recently changed to use quaternion representation instead of
    axis-angle. */

class GLEEMDLL _GleemRot {
public:
  /** Default constructor initializes to the identity quaternion */
  _GleemRot();
  /** Axis does not need to be normalized but must not be the zero
      vector. Angle is in radians. */
  _GleemRot(const _GleemV3f &axis, float angle);
  
  /** Re-initialize this quaternion to be the identity quaternion "e"
      (i.e., no rotation) */
  void init();

  //  float &operator[](int i);
  //  const float &operator[](int i) const;
  int operator ==(const _GleemRot &arg) const;

  /** Test for "approximate equality" -- performs componentwise test
      to see whether difference between all components is less than
      epsilon. */
  bool withinEpsilon(const _GleemRot &arg, float epsilon) const;

  //  void setValue(float x, float y, float z, float w);
  /** Axis does not need to be normalized but must not be the zero
      vector. Angle is in radians. */
  void setValue(const _GleemV3f &axis, float angle);

  /** Returns angle (in radians) and mutates the given vector to be
      the axis. */
  float getValue(_GleemV3f &axis) const;

  /** Mutate this quaternion to be its inverse. This is equivalent to
      the conjugate of the quaternion. */
  void invert();

  /** Set nrot to be the inverse (= conjugate) of rot. */
  static void invert(const _GleemRot &rot, _GleemRot &nrot);

  /** Length of this quaternion in four-space */
  float length() const;

  /** This dotted with this */
  float lengthSquared() const;

  /** Make this quaternion a unit quaternion again. If you are
      composing dozens of quaternions you probably should call this
      periodically to ensure that you have a valid rotation. */
  void normalize();

  /** Compose two rotations: C = A * B in that order. NOTE that
      because we assume a column vector representation that this
      implies that a vector rotated by the cumulative rotation will be
      rotated first by B, then A. NOTE: CAN NOT USE SOURCE FOR DEST */
  static void mult(const _GleemRot &a, const _GleemRot &b, _GleemRot &c);

  /** Convenience routine which returns a _GleemRot on the stack. Result
      = this * arg (in that order; see mult, above). */
  _GleemRot operator*(const _GleemRot &arg);

  /** Turns this _GleemRot into a 3x3 rotation matrix. NOTE: only mutates
      the upper-left 3x3 of the passed _GleemMat4f. */
  void toMatrix(_GleemMat4f &mat) const;
  
  /** Turns the upper left 3x3 of the passed matrix into a rotation.
      See _GleemMat4f::getRotation(), above. */
  void fromMatrix(const _GleemMat4f &mat);
  
  /** Rotate a vector by this quaternion. NOTE: CAN NOT USE SOURCE FOR
      DEST. */
  void rotateVector(const _GleemV3f &src, _GleemV3f &dest);

  /** Rotate a vector by this quaternion, returning result on stack. */
  _GleemV3f rotateVector(const _GleemV3f &src);

  /** Convenience operator. Puts quaternion on output stream looking
      like "[ q0, q1, q2, q3 ]". Note this does NOT correspond to the
      axis-angle representation, but to the actual quaternion
      components. */
  friend std::ostream &operator<<(std::ostream &os, const _GleemRot &rot);

 private:
  // Representation is a quaternion. Element 0 is the scalar part (=
  // cos(theta/2)), elements 1..3 the imaginary/"vector" part (=
  // sin(theta/2) * axis).
  float val[4];
};

#endif  // #defined _GLEEM__LINALG_H
