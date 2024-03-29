/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998, 1999, 2002 Kenneth B. Russell (kbrussel@alum.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

package gnu.gleem.linalg;

/** 4-element single-precision vector */

public class Vec4f {
  private float x;
  private float y;
  private float z;
  private float w;

  public Vec4f() {}

  public Vec4f(Vec4f arg) {
    set(arg);
  }

  public Vec4f(float x, float y, float z, float w) {
    set(x, y, z, w);
  }

  public Vec4f copy() {
    return new Vec4f(this);
  }

  public void set(Vec4f arg) {
    set(arg.x, arg.y, arg.z, arg.w);
  }

  public void set(float x, float y, float z, float w) {
    this.x = x;
    this.y = y;
    this.z = z;
    this.w = w;
  }

  /** Sets the ith component, 0 <= i < 4 */
  public void set(int i, float val) {
    switch (i) {
    case 0: x = val; break;
    case 1: y = val; break;
    case 2: z = val; break;
    case 3: w = val; break;
    default: throw new IndexOutOfBoundsException();
    }
  }

  /** Gets the ith component, 0 <= i < 4 */
  public float get(int i) {
    switch (i) {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    case 3: return w;
    default: throw new IndexOutOfBoundsException();
    }
  }

  public float x() { return x; }
  public float y() { return y; }
  public float z() { return z; }
  public float w() { return w; }

  public void setX(float x) { this.x = x; }
  public void setY(float y) { this.y = y; }
  public void setZ(float z) { this.z = z; }
  public void setW(float w) { this.w = w; }

  public float dot(Vec4f arg) {
    return x * arg.x + y * arg.y + z * arg.z + w * arg.w;
  }

  public float length() {
    return (float) Math.sqrt(lengthSquared());
  }

  public float lengthSquared() {
    return this.dot(this);
  }

  public void normalize() {
    float len = length();
    if (len == 0.0f) return;
    scale(1.0f / len);
  }

  /** Returns this * val; creates new vector */
  public Vec4f times(float val) {
    Vec4f tmp = new Vec4f(this);
    tmp.scale(val);
    return tmp;
  }

  /** this = this * val */
  public void scale(float val) {
    x *= val;
    y *= val;
    z *= val;
    w *= val;
  }

  /** Returns this + arg; creates new vector */
  public Vec4f plus(Vec4f arg) {
    Vec4f tmp = new Vec4f();
    tmp.add(this, arg);
    return tmp;
  }

  /** this = this + b */
  public void add(Vec4f b) {
    add(this, b);
  }

  /** this = a + b */
  public void add(Vec4f a, Vec4f b) {
    x = a.x + b.x;
    y = a.y + b.y;
    z = a.z + b.z;
    w = a.w + b.w;
  }

  /** Returns this + s * arg; creates new vector */
  public Vec4f addScaled(float s, Vec4f arg) {
    Vec4f tmp = new Vec4f();
    tmp.addScaled(this, s, arg);
    return tmp;
  }

  /** this = a + s * b */
  public void addScaled(Vec4f a, float s, Vec4f b) {
    x = a.x + s * b.x;
    y = a.y + s * b.y;
    z = a.z + s * b.z;
    w = a.w + s * b.w;
  }

  /** Returns this - arg; creates new vector */
  public Vec4f minus(Vec4f arg) {
    Vec4f tmp = new Vec4f();
    tmp.sub(this, arg);
    return tmp;
  }

  /** this = this - b */
  public void sub(Vec4f b) {
    sub(this, b);
  }

  /** this = a - b */
  public void sub(Vec4f a, Vec4f b) {
    x = a.x - b.x;
    y = a.y - b.y;
    z = a.z - b.z;
    w = a.w - b.w;
  }

  public Vecf toVecf() {
    Vecf out = new Vecf(4);
    for (int i = 0; i < 4; i++) {
      out.set(i, get(i));
    }
    return out;
  }

  public String toString() {
    return "(" + x + ", " + y + ", " + z + ")";
  }
}
