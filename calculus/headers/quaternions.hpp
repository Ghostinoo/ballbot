
#pragma once

struct Quaternion {

  float w, x, y, z;

  Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
  Quaternion() : w(0), x(0), y(0), z(0) {}
  Quaternion operator*(const Quaternion& q) const;
};