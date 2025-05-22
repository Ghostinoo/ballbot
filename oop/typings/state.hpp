
#pragma once

#include "../../calculus/headers/vectors.hpp"
#include "../../calculus/headers/quaternions.hpp"

struct IMUState {
  Quaternion orientation;
  Vector3 angularVelocity;
  float temperature;
};
