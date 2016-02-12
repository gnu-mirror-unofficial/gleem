/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

#include <assert.h>
#include <cmath>
#include "../include/_Linalg.h"
#include <memory.h>

// Can you believe Microsoft?
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

//
// _GleemV3f
//

_GleemV3f::_GleemV3f()
{
}

_GleemV3f::_GleemV3f(const _GleemV3f &arg)
{
  setValue(arg);
}

_GleemV3f::_GleemV3f(float x, float y, float z)
{
  val[0] = x; val[1] = y; val[2] = z; 
}

_GleemV3f &
_GleemV3f::operator=(const _GleemV3f &arg)
{
  setValue(arg); return *this;
}

int
_GleemV3f::operator ==(const _GleemV3f &arg) const
{
  return ((val[0] == arg.val[0]) &&
	  (val[1] == arg.val[1]) &&
	  (val[2] == arg.val[2]));
}

float &
_GleemV3f::operator[](int i)
{
  return val[i];
}

const float &
_GleemV3f::operator[](int i) const
{
  return val[i];
}

void
_GleemV3f::setValue(float x, float y, float z)
{
  val[0] = x; val[1] = y; val[2] = z;
}

void
_GleemV3f::setValue(const _GleemV3f &arg)
{
  val[0] = arg.val[0]; val[1] = arg.val[1]; val[2] = arg.val[2];
}

const float *
_GleemV3f::getValue() const
{
  return val;
}

float
_GleemV3f::dot(const _GleemV3f &arg) const
{
  return val[0] * arg.val[0] + val[1] * arg.val[1] + val[2] * arg.val[2];
}

float
_GleemV3f::length() const
{
  return sqrtf(this->dot(*this));
}

float
_GleemV3f::lengthSquared() const
{
  return this->dot(*this);
}

void
_GleemV3f::normalize()
{
  float len = sqrtf(this->dot(*this));
  if (len != 0.0f)
    _GleemV3f::scale(*this, 1.0f / len, *this);
}

_GleemV3f
_GleemV3f::operator +(const _GleemV3f &a) const
{
  _GleemV3f dest;
  _GleemV3f::add(*this, a, dest);
  return dest;
}

_GleemV3f
_GleemV3f::operator -(const _GleemV3f &a) const
{
  _GleemV3f dest;
  _GleemV3f::sub(*this, a, dest);
  return dest;
}

_GleemV3f &
_GleemV3f::operator +=(const _GleemV3f &a)
{
  _GleemV3f::add(*this, a, *this);
  return *this;
}

_GleemV3f &
_GleemV3f::operator -=(const _GleemV3f &a)
{
  _GleemV3f::sub(*this, a, *this);
  return *this;
}

_GleemV3f
_GleemV3f::operator *(float a) const
{
  _GleemV3f dest;
  _GleemV3f::scale(*this, a, dest);
  return dest;
}

_GleemV3f
operator *(float a, const _GleemV3f &v)
{
  _GleemV3f dest;
  _GleemV3f::scale(v, a, dest);
  return dest;
}

_GleemV3f &
_GleemV3f::operator *=(float a)
{
  _GleemV3f::scale(*this, a, *this);
  return *this;
}

void
_GleemV3f::add(const _GleemV3f &a, const _GleemV3f &b, _GleemV3f &c)
{
  c.val[0] = a.val[0] + b.val[0];
  c.val[1] = a.val[1] + b.val[1];
  c.val[2] = a.val[2] + b.val[2];
}

void
_GleemV3f::addScaled(const _GleemV3f &a, float s,
		   const _GleemV3f &b, _GleemV3f &c)
{
  c.val[0] = a.val[0] + s * b.val[0];
  c.val[1] = a.val[1] + s * b.val[1];
  c.val[2] = a.val[2] + s * b.val[2];
}

void
_GleemV3f::sub(const _GleemV3f &a, const _GleemV3f &b, _GleemV3f &c)
{
  c.val[0] = a.val[0] - b.val[0];
  c.val[1] = a.val[1] - b.val[1];
  c.val[2] = a.val[2] - b.val[2];
}

void
_GleemV3f::scale(const _GleemV3f &a, float s, _GleemV3f &c)
{
  c.val[0] = s * a.val[0];
  c.val[1] = s * a.val[1];
  c.val[2] = s * a.val[2];
}

void
_GleemV3f::cross(const _GleemV3f &a, const _GleemV3f &b, _GleemV3f &c)
{
  c.val[0] = a.val[1] * b.val[2] - b.val[1] * a.val[2];
  c.val[1] = b.val[0] * a.val[2] - a.val[0] * b.val[2];
  c.val[2] = a.val[0] * b.val[1] - b.val[0] * a.val[1];
}

ostream &
operator<<(ostream &os, const _GleemV3f &vec)
{
  os << "[ " << vec[0] << ", " << vec[1] << ", " << vec[2] << " ]" << endl;
  return os;
}

//
// _GleemV2f
//

_GleemV2f::_GleemV2f()
{
}

_GleemV2f::_GleemV2f(const _GleemV2f &arg)
{
  setValue(arg);
}

_GleemV2f::_GleemV2f(float x, float y)
{
  val[0] = x; val[1] = y;
}

float &
_GleemV2f::operator[](int i)
{
  return val[i];
}

const float &
_GleemV2f::operator[](int i) const
{
  return val[i];
}

int
_GleemV2f::operator ==(const _GleemV2f &arg) const
{
  return ((val[0] == arg.val[0]) &&
	  (val[1] == arg.val[1]));
}

void
_GleemV2f::setValue(float x, float y)
{
  val[0] = x; val[1] = y;
}

void
_GleemV2f::setValue(const _GleemV2f &arg)
{
  val[0] = arg.val[0]; val[1] = arg.val[1];
}

float
_GleemV2f::dot(const _GleemV2f &arg) const
{
  return val[0] * arg.val[0] + val[1] * arg.val[1];
}

float
_GleemV2f::length() const
{
  return sqrtf(this->dot(*this));
}

float
_GleemV2f::lengthSquared() const
{
  return this->dot(*this);
}

void
_GleemV2f::normalize()
{
  float len = sqrtf(this->dot(*this));
  if (len != 0.0f)
    _GleemV2f::scale(*this, 1.0f / len, *this);
}

_GleemV2f
_GleemV2f::operator +(const _GleemV2f &a) const
{
  _GleemV2f dest;
  _GleemV2f::add(*this, a, dest);
  return dest;
}

_GleemV2f
_GleemV2f::operator -(const _GleemV2f &a) const
{
  _GleemV2f dest;
  _GleemV2f::sub(*this, a, dest);
  return dest;
}

_GleemV2f &
_GleemV2f::operator +=(const _GleemV2f &a)
{
  _GleemV2f::add(*this, a, *this);
  return *this;
}

_GleemV2f &
_GleemV2f::operator -=(const _GleemV2f &a)
{
  _GleemV2f::sub(*this, a, *this);
  return *this;
}

_GleemV2f
_GleemV2f::operator *(float a) const
{
  _GleemV2f dest;
  _GleemV2f::scale(*this, a, dest);
  return dest;
}

_GleemV2f
operator *(float a, const _GleemV2f &v)
{
  _GleemV2f dest;
  _GleemV2f::scale(v, a, dest);
  return dest;
}

_GleemV2f &
_GleemV2f::operator *=(float a)
{
  _GleemV2f::scale(*this, a, *this);
  return *this;
}

void
_GleemV2f::add(const _GleemV2f &a, const _GleemV2f &b, _GleemV2f &c)
{
  c.val[0] = a.val[0] + b.val[0];
  c.val[1] = a.val[1] + b.val[1];
}

void
_GleemV2f::addScaled(const _GleemV2f &a, float s,
		   const _GleemV2f &b, _GleemV2f &c)
{
  c.val[0] = a.val[0] + s * b.val[0];
  c.val[1] = a.val[1] + s * b.val[1];
}

void
_GleemV2f::sub(const _GleemV2f &a, const _GleemV2f &b, _GleemV2f &c)
{
  c.val[0] = a.val[0] - b.val[0];
  c.val[1] = a.val[1] - b.val[1];
}

void
_GleemV2f::scale(const _GleemV2f &a, float s, _GleemV2f &c)
{
  c.val[0] = s * a.val[0];
  c.val[1] = s * a.val[1];
}

ostream &
operator<<(ostream &os, const _GleemV2f &vec)
{
  os << "[ " << vec[0] << ", " << vec[1] << " ]" << endl;
  return os;
}

//
// _GleemMat4f
//

_GleemMat4f::_GleemMat4f()
{
}

void
_GleemMat4f::makeIdent()
{
  memset(m, 0, sizeof(m));
  m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
}

void
_GleemMat4f::setTranslation(const _GleemV3f &trans)
{
  m[0][3] = trans[0];
  m[1][3] = trans[1];
  m[2][3] = trans[2];
}

void
_GleemMat4f::setRotation(const _GleemRot &rot)
{
  rot.toMatrix(*this);
}

void
_GleemMat4f::getRotation(_GleemRot &rot) const
{
  rot.fromMatrix(*this);
}

const float *
_GleemMat4f::operator[](int i) const
{
  return m[i];
}

float *
_GleemMat4f::operator[](int i)
{
  return m[i];
}

void
_GleemMat4f::invertRigid()
{
  float t;
  // Transpose upper left 3x3
  t = m[0][1];
  m[0][1] = m[1][0];
  m[1][0] = t;
  t = m[0][2];
  m[0][2] = m[2][0];
  m[2][0] = t;
  t = m[1][2];
  m[1][2] = m[2][1];
  m[2][1] = t;
  // Transform negative translation by this
  _GleemV3f negTrans(-m[0][3], -m[1][3], -m[2][3]);
  _GleemV3f trans;
  xformDir(negTrans, trans);
  m[0][3] = trans[0];
  m[1][3] = trans[1];
  m[2][3] = trans[2];
}

void
_GleemMat4f::mult(const _GleemMat4f &a, const _GleemMat4f &b, _GleemMat4f &c)
{
  for (int rc = 0; rc < 4; rc++)
    for (int cc = 0; cc < 4; cc++)
      {
	float tmp = 0.0f;
	for (int i = 0; i < 4; i++)
	  tmp += a[rc][i] * b[i][cc];
	c[rc][cc] = tmp;
      }
}

void
_GleemMat4f::xformPt(const _GleemV3f &src, _GleemV3f &dest) const
{
  for (int rc = 0; rc < 3; rc++)
    {
      float tmp = 0.0f;
      for (int cc = 0; cc < 3; cc++)
	{
	  tmp += m[rc][cc] * src[cc];
	}
      tmp += m[rc][3];
      dest[rc] = tmp;
    }
}

void
_GleemMat4f::xformDir(const _GleemV3f &src, _GleemV3f &dest) const
{
  for (int rc = 0; rc < 3; rc++)
    {
      float tmp = 0.0f;
      for (int cc = 0; cc < 3; cc++)
	{
	  tmp += m[rc][cc] * src[cc];
	}
      dest[rc] = tmp;
    }
}

//
// _GleemMat3f
//

_GleemMat3f::_GleemMat3f()
{
}

void
_GleemMat3f::makeIdent()
{
  m[0][1] = m[0][2] = m[1][0] = m[1][2] = m[2][0] = m[2][1] = 0;
  m[0][0] = m[1][1] = m[2][2] = 1;
}

const float *
_GleemMat3f::operator[](int i) const
{
  return m[i];
}

float *
_GleemMat3f::operator[](int i)
{
  return m[i];
}

void
_GleemMat3f::setCol(int i, const _GleemV3f &v)
{
  assert((i >= 0) && (i <= 2));
  m[0][i] = v[0];
  m[1][i] = v[1];
  m[2][i] = v[2];
}

void
_GleemMat3f::setRow(int i, const _GleemV3f &v)
{
  assert((i >= 0) && (i <= 2));
  m[i][0] = v[0];
  m[i][1] = v[1];
  m[i][2] = v[2];
}

void
_GleemMat3f::transpose()
{
  float t;
  t = m[0][1];
  m[0][1] = m[1][0];
  m[1][0] = t;

  t = m[0][2];
  m[0][2] = m[2][0];
  m[2][0] = t;

  t = m[1][2];
  m[1][2] = m[2][1];
  m[2][1] = t;
}

float
_GleemMat3f::determinant() const
{
  return (m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) +
	  m[0][1] * (m[2][0] * m[1][2] - m[1][0] * m[2][2]) +
	  m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]));
}

bool
_GleemMat3f::invert()
{
  float det = determinant();
  if (det == 0.0f)
    return false;

  // Form cofactor matrix
  float cf[3][3];
  cf[0][0] = m[1][1] * m[2][2] - m[2][1] * m[1][2];
  cf[0][1] = m[2][0] * m[1][2] - m[1][0] * m[2][2];
  cf[0][2] = m[1][0] * m[2][1] - m[2][0] * m[1][1];
  cf[1][0] = m[2][1] * m[0][2] - m[0][1] * m[2][2];
  cf[1][1] = m[0][0] * m[2][2] - m[2][0] * m[0][2];
  cf[1][2] = m[2][0] * m[0][1] - m[0][0] * m[2][1];
  cf[2][0] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
  cf[2][1] = m[1][0] * m[0][2] - m[0][0] * m[1][2];
  cf[2][2] = m[0][0] * m[1][1] - m[1][0] * m[0][1];

  // Now copy back transposed
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      m[i][j] = cf[j][i] / det;
  return true;
}

void
_GleemMat3f::xformVec(const _GleemV3f &src, _GleemV3f &dest) const
{
  dest[0] = (m[0][0] * src[0] +
	     m[0][1] * src[1] +
	     m[0][2] * src[2]);
  dest[1] = (m[1][0] * src[0] +
	     m[1][1] * src[1] +
	     m[1][2] * src[2]);
  dest[2] = (m[2][0] * src[0] +
	     m[2][1] * src[1] +
	     m[2][2] * src[2]);
#if 0
  for (int rc = 0; rc < 3; rc++)
    {
      dest[rc] = 0;
      for (int cc = 0; cc < 3; cc++)
	dest[rc] += m[rc][cc] * src[cc];
    }
#endif
}

void
_GleemMat3f::mult(const _GleemMat3f &a,
		const _GleemMat3f &b,
		_GleemMat3f &c)
{
  for (int rc = 0; rc < 3; rc++)
    for (int cc = 0; cc < 3; cc++)
      {
	c[rc][cc] = 0;
	for (int i = 0; i < 3; i++)
	  c[rc][cc] += a[rc][i] * b[i][cc];
      }
}

_GleemMat3f
_GleemMat3f::operator*(const _GleemMat3f &arg) const
{
  _GleemMat3f retval;
  mult(*this, arg, retval);
  return retval;
}


//
// _GleemMat2f
//

_GleemMat2f::_GleemMat2f()
{
}

void
_GleemMat2f::makeIdent()
{
  m[0][0] = m[1][1] = 1;
  m[0][1] = m[1][0] = 0;
}

const float *
_GleemMat2f::operator[](int i) const
{
  return m[i];
}

float *
_GleemMat2f::operator[](int i)
{
  return m[i];
}

void
_GleemMat2f::setCol(int i, const _GleemV2f &v)
{
  assert((i == 0) || (i == 1));
  m[0][i] = v[0];
  m[1][i] = v[1];
}

void
_GleemMat2f::setRow(int i, const _GleemV2f &v)
{
  assert((i == 0) || (i == 1));
  m[i][0] = v[0];
  m[i][1] = v[1];
}

void
_GleemMat2f::transpose()
{
  float t = m[0][1];
  m[0][1] = m[1][0];
  m[1][0] = t;
}

float
_GleemMat2f::determinant() const
{
  return (m[0][0] * m[1][1] - m[1][0] * m[0][1]);
}

bool
_GleemMat2f::invert()
{
  float det = determinant();
  if (det == 0.0f)
    return false;

  // Create transpose of cofactor matrix in place
  float t = m[0][0];
  m[0][0] = m[1][1];
  m[1][1] = t;
  m[0][1] = -m[0][1];
  m[1][0] = -m[1][0];
  
  // Now divide by determinant
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++)
      m[i][j] /= det;
  return true;
}

void
_GleemMat2f::xformVec(const _GleemV2f &src, _GleemV2f &dest) const
{
  dest[0] = (m[0][0] * src[0] +
	     m[0][1] * src[1]);
  dest[1] = (m[1][0] * src[0] +
	     m[1][1] * src[1]);
}

void
_GleemMat2f::mult(const _GleemMat2f &a,
		const _GleemMat2f &b,
		_GleemMat2f &c)
{
  for (int rc = 0; rc < 2; rc++)
    for (int cc = 0; cc < 2; cc++)
      {
	c[rc][cc] = 0;
	for (int i = 0; i < 2; i++)
	  c[rc][cc] += a[rc][i] * b[i][cc];
      }
}

_GleemMat2f
_GleemMat2f::operator *(const _GleemMat2f &arg) const
{
  _GleemMat2f retval;
  mult(*this, arg, retval);
  return retval;
}

//
// _GleemRot
//

_GleemRot::_GleemRot()
{
  init();
}

_GleemRot::_GleemRot(const _GleemV3f &axis, float angle)
{
  setValue(axis, angle);
}

void
_GleemRot::init()
{
  val[0] = 1;
  val[1] = 0;
  val[2] = 0;
  val[3] = 0;
}

int
_GleemRot::operator==(const _GleemRot &arg) const
{
  return ((val[0] == arg.val[0]) &&
	  (val[1] == arg.val[1]) &&
	  (val[2] == arg.val[2]) &&
	  (val[3] == arg.val[3]));
}

bool
_GleemRot::withinEpsilon(const _GleemRot &arg, float epsilon) const
{
  return ((fabs(val[0] - arg.val[0]) < epsilon) &&
	  (fabs(val[1] - arg.val[1]) < epsilon) &&
	  (fabs(val[2] - arg.val[2]) < epsilon) &&
	  (fabs(val[3] - arg.val[3]) < epsilon));
}

void
_GleemRot::setValue(const _GleemV3f &axis, float angle)
{
  float halfTheta = angle / 2.0f;
  val[0] = cosf(halfTheta);
  float sinHalfTheta = sinf(halfTheta);
  _GleemV3f realAxis = axis;
  realAxis.normalize();
  val[1] = realAxis[0] * sinHalfTheta;
  val[2] = realAxis[1] * sinHalfTheta;
  val[3] = realAxis[2] * sinHalfTheta;
}

float
_GleemRot::getValue(_GleemV3f &axis) const
{
  // FIXME: Is this numerically stable? Is there a better way to
  // extract the angle from a quaternion?
  float retval = 2.0f * acosf(val[0]);
  axis[0] = val[1];
  axis[1] = val[2];
  axis[2] = val[3];
  axis.normalize();
  return retval;
}

void
_GleemRot::invert()
{
  val[1] = -val[1];
  val[2] = -val[2];
  val[3] = -val[3];
}

void
_GleemRot::invert(const _GleemRot &rot, _GleemRot &nrot)
{
  nrot.val[0] = rot.val[0];
  nrot.val[1] = -rot.val[1];
  nrot.val[2] = -rot.val[2];
  nrot.val[3] = -rot.val[3];
}

float
_GleemRot::length() const
{
  return sqrtf(lengthSquared());
}

float
_GleemRot::lengthSquared() const
{
  return (val[0] * val[0] +
	  val[1] * val[1] +
	  val[2] * val[2] +
	  val[3] * val[3]);	  
}

void
_GleemRot::normalize()
{
  float len = length();
  val[0] /= len;
  val[1] /= len;
  val[2] /= len;
  val[3] /= len;
}

void
_GleemRot::mult(const _GleemRot &a, const _GleemRot &b, _GleemRot &c)
{
  c.val[0] = (a.val[0] * b.val[0] - a.val[1] * b.val[1] -
	      a.val[2] * b.val[2] - a.val[3] * b.val[3]);
  c.val[1] = (a.val[0] * b.val[1] + a.val[1] * b.val[0] +
	      a.val[2] * b.val[3] - a.val[3] * b.val[2]);
  c.val[2] = (a.val[0] * b.val[2] + a.val[2] * b.val[0] -
	      a.val[1] * b.val[3] + a.val[3] * b.val[1]);
  c.val[3] = (a.val[0] * b.val[3] + a.val[3] * b.val[0] +
	      a.val[1] * b.val[2] - a.val[2] * b.val[1]);
}

_GleemRot
_GleemRot::operator*(const _GleemRot &arg)
{
  _GleemRot retval;
  mult(*this, arg, retval);
  return retval;
}

/** Implementation from B. K. P. Horn's _Robot Vision_ textbook */

void
_GleemRot::toMatrix(_GleemMat4f &mat) const
{
  float q00 = val[0] * val[0];
  float q11 = val[1] * val[1];
  float q22 = val[2] * val[2];
  float q33 = val[3] * val[3];
  // Diagonal elements
  mat[0][0] = q00 + q11 - q22 - q33;
  mat[1][1] = q00 - q11 + q22 - q33;
  mat[2][2] = q00 - q11 - q22 + q33;
  // 0,1 and 1,0 elements
  float q03 = val[0] * val[3];
  float q12 = val[1] * val[2];
  mat[0][1] = 2.0f * (q12 - q03);
  mat[1][0] = 2.0f * (q03 + q12);
  // 0,2 and 2,0 elements
  float q02 = val[0] * val[2];
  float q13 = val[1] * val[3];
  mat[0][2] = 2.0f * (q02 + q13);
  mat[2][0] = 2.0f * (q13 - q02);
  // 1,2 and 2,1 elements
  float q01 = val[0] * val[1];
  float q23 = val[2] * val[3];
  mat[1][2] = 2.0f * (q23 - q01);
  mat[2][1] = 2.0f * (q01 + q23);
}

/** From Watt and Watt, _Advanced Animation and Rendering Techniques_. */

void
_GleemRot::fromMatrix(const _GleemMat4f &mat)
{
  // FIXME: Should reimplement to follow Horn's advice of using
  // eigenvector decomposition to handle roundoff error in given
  // matrix.

  float tr, s;
  int i, j, k;
  
  tr = mat[0][0] + mat[1][1] + mat[2][2];
  if (tr > 0.0)
    {
      s = sqrtf(tr + 1.0f);
      val[0] = s * 0.5f;
      s = 0.5f / s;
      val[1] = (mat[2][1] - mat[1][2]) * s;
      val[2] = (mat[0][2] - mat[2][0]) * s;
      val[3] = (mat[1][0] - mat[0][1]) * s;
    }
  else
    {
      i = 0;
      if (mat[1][1] > mat[0][0])
	i = 1;
      if (mat[2][2] > mat[i][i])
	i = 2;
      j = (i+1)%3;
      k = (j+1)%3;
      s = sqrtf( (mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0f);
      val[i+1] = s * 0.5f;
      s = 0.5f / s;
      val[0] = (mat[k][j] - mat[j][k]) * s;
      val[j+1] = (mat[j][i]+ mat[i][j]) * s;
      val[k+1] = (mat[k][i] + mat[i][k]) * s;
    }
}

/** Implementation is from Horn's _Robot Vision_ */

void
_GleemRot::rotateVector(const _GleemV3f &src, _GleemV3f &dest)
{
  _GleemV3f qCrossX;
  _GleemV3f qCrossXCrossQ;
  _GleemV3f qVec(val[1], val[2], val[3]);
  
  _GleemV3f::cross(qVec, src, qCrossX);
  _GleemV3f::cross(qCrossX, qVec, qCrossXCrossQ);
  _GleemV3f::scale(qCrossX, 2.0f * val[0], qCrossX);
  _GleemV3f::scale(qCrossXCrossQ, -2.0f, qCrossXCrossQ);
  _GleemV3f::add(src, qCrossX, dest);
  _GleemV3f::add(dest, qCrossXCrossQ, dest);

  /*
  _GleemRot tmp, tmp2;
  _GleemRot x;
  x.val[0] = 0;
  x.val[1] = src[0];
  x.val[2] = src[1];
  x.val[3] = src[2];
  mult(*this, x, tmp);
  invert();
  mult(tmp, *this, tmp2);
  invert();
  dest[0] = tmp2.val[1];
  dest[1] = tmp2.val[2];
  dest[2] = tmp2.val[3];
  */
}

_GleemV3f
_GleemRot::rotateVector(const _GleemV3f &src)
{
  _GleemV3f dest;
  rotateVector(src, dest);
  return dest;
}

ostream &
operator<<(ostream &os, const _GleemRot &vec)
{
  os << "[ " << vec.val[0] << ", " << vec.val[1] << ", "
     << vec.val[2] << ", " << vec.val[3] << " ]" << endl;
  return os;
}
