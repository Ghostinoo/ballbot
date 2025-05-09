#include "../headers/vectors.hpp"
#include <cmath>

Vector3::Vector3(float x, float y, float z): x(x), y(y), z(z) {}

float Vector3::magnitude() const {
  return std::sqrt(x*x + y*y + z*z);
}

Vector3 Vector3::operator+(const Vector3& other) const {
  return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
  return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const {
  return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::cross(const Vector3& other) const {
  return Vector3(
    y * other.z - z * other.y,
    z * other.x - x * other.z,
    x * other.y - y * other.x
  );
}

float Vector3::dot(const Vector3& other) const {
  return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::normalize() const {
  float magnitude = this->magnitude();
  return (magnitude !=0) ? (*this)*(1.0f/magnitude) : *this;
}

Vector3 Vector3::project(const Vector3& other) const {
  return other * (this->dot(other) / (other.x * other.x + other.y * other.y + other.z * other.z));
}

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}