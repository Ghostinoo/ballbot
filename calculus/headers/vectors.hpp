
#pragma once

#include <ostream>

struct Vector3 {

  float x,y,z;

  Vector3 (float x, float y, float z);
  Vector3 (): x(0), y(0), z(0) {}

  float magnitude() const;
  Vector3 operator+(const Vector3& other) const;
  Vector3 operator-(const Vector3& other) const;
  Vector3 operator*(float scalar) const;
  Vector3 cross(const Vector3& other) const;
  float dot(const Vector3& other) const;
  Vector3 normalize() const;
  Vector3 project(const Vector3& other) const;

  friend std::ostream& operator<<(std::ostream&, const Vector3& v);
};