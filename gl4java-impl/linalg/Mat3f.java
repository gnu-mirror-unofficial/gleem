/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998, 1999, 2002 Kenneth B. Russell (kbrussel@alum.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

package gnu.gleem.linalg;

/** 3x3 matrix class useful for simple linear algebra. Representation
    is (as Mat4f) in row major order and assumes multiplication by
    column vectors on the right. */

public class Mat3f {
  private float[] data;

  /** Creates new matrix initialized to the zero matrix */
  public Mat3f() {
    data = new float[9];
  }

  /** Initialize to the identity matrix. */
  public void makeIdent() {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (i == j) {
          set(i, j, 1.0f);
        } else {
          set(i, j, 0.0f);
        }
      }
    }
  }
  
  /** Gets the (i,j)th element of this matrix, where i is the row
      index and j is the column index */
  public float get(int i, int j) {
    return data[3 * i + j];
  }

  /** Sets the (i,j)th element of this matrix, where i is the row
      index and j is the column index */
  public void set(int i, int j, float val) {
    data[3 * i + j] = val;
  }

  /** Set column i (i=[0..2]) to vector v. */
  public void setCol(int i, Vec3f v) {
    set(0, i, v.x());
    set(1, i, v.y());
    set(2, i, v.z());
  }

  /** Set row i (i=[0..2]) to vector v. */
  public void setRow(int i, Vec3f v) {
    set(i, 0, v.x());
    set(i, 1, v.y());
    set(i, 2, v.z());
  }

  /** Transpose this matrix in place. */
  public void transpose() {
    float t;
    t = get(0, 1);
    set(0, 1, get(1, 0));
    set(1, 0, t);

    t = get(0, 2);
    set(0, 2, get(2, 0));
    set(2, 0, t);

    t = get(1, 2);
    set(1, 2, get(2, 1));
    set(2, 1, t);
  }

  /** Return the determinant. Computed across the zeroth row. */
  public float determinant() {
    return (get(0, 0) * (get(1, 1) * get(2, 2) - get(2, 1) * get(1, 2)) +
            get(0, 1) * (get(2, 0) * get(1, 2) - get(1, 0) * get(2, 2)) +
            get(0, 2) * (get(1, 0) * get(2, 1) - get(2, 0) * get(1, 1)));
  }

  /** Full matrix inversion in place. If matrix is singular, returns
      false and matrix contents are untouched. If you know the matrix
      is orthonormal, you can call transpose() instead. */
  public boolean invert() {
    float det = determinant();
    if (det == 0.0f)
      return false;

    // Form cofactor matrix
    Mat3f cf = new Mat3f();
    cf.set(0, 0, get(1, 1) * get(2, 2) - get(2, 1) * get(1, 2));
    cf.set(0, 1, get(2, 0) * get(1, 2) - get(1, 0) * get(2, 2));
    cf.set(0, 2, get(1, 0) * get(2, 1) - get(2, 0) * get(1, 1));
    cf.set(1, 0, get(2, 1) * get(0, 2) - get(0, 1) * get(2, 2));
    cf.set(1, 1, get(0, 0) * get(2, 2) - get(2, 0) * get(0, 2));
    cf.set(1, 2, get(2, 0) * get(0, 1) - get(0, 0) * get(2, 1));
    cf.set(2, 0, get(0, 1) * get(1, 2) - get(1, 1) * get(0, 2));
    cf.set(2, 1, get(1, 0) * get(0, 2) - get(0, 0) * get(1, 2));
    cf.set(2, 2, get(0, 0) * get(1, 1) - get(1, 0) * get(0, 1));

    // Now copy back transposed
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        set(i, j, cf.get(j, i) / det);
    return true;
  }

  /** Multiply a 3D vector by this matrix. NOTE: src and dest must be
      different vectors. */
  public void xformVec(Vec3f src, Vec3f dest) {
    dest.set(get(0, 0) * src.x() +
             get(0, 1) * src.y() +
             get(0, 2) * src.z(),
             
             get(1, 0) * src.x() +
             get(1, 1) * src.y() +
             get(1, 2) * src.z(),

             get(2, 0) * src.x() +
             get(2, 1) * src.y() +
             get(2, 2) * src.z());
  }

  /** Returns this * b; creates new matrix */
  public Mat3f mul(Mat3f b) {
    Mat3f tmp = new Mat3f();
    tmp.mul(this, b);
    return tmp;
  }

  /** this = a * b */
  public void mul(Mat3f a, Mat3f b) {
    for (int rc = 0; rc < 3; rc++)
      for (int cc = 0; cc < 3; cc++) {
        float tmp = 0.0f;
        for (int i = 0; i < 3; i++)
          tmp += a.get(rc, i) * b.get(i, cc);
        set(rc, cc, tmp);
      }
  }

  public Matf toMatf() {
    Matf out = new Matf(3, 3);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        out.set(i, j, get(i, j));
      }
    }
    return out;
  }

  public String toString() {
    String endl = System.getProperty("line.separator");
    return "(" +
      get(0, 0) + ", " + get(0, 1) + ", " + get(0, 2) + endl +
      get(1, 0) + ", " + get(1, 1) + ", " + get(1, 2) + endl +
      get(2, 0) + ", " + get(2, 1) + ", " + get(2, 2) + ")";
  }
}
