
#include "../headers/quaternions.hpp"

Quaternion Quaternion::operator*(const Quaternion& q) const {
  return Quaternion(
    w * q.w - x * q.x - y * q.y - z * q.z,
    w * q.x + x * q.w + y * q.z - z * q.y,
    w * q.y - x * q.z + y * q.w + z * q.x,
    w * q.z + x * q.y - y * q.x + z * q.w
  );
}