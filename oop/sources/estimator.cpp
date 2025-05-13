
#include "../headers/estimator.hpp"
#include "../../conf.h"
#include <iostream>

Estimator::Position Estimator::position;
Estimator::IMUState Estimator::state;


void Estimator::Position::set(const Vector3& newValue) {
  std::unique_lock lock(mutex);
  value = newValue;
}

Vector3 Estimator::Position::get() {
  std::shared_lock<std::shared_mutex> lock(mutex);
  return value;
}

void Estimator::updatePosition(Vector3 pos) {
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> dt = now - position.lastUpdate;

  Vector3 result;
  Vector3 peripheralSpeed = BALL_RADIUS * pos.cross({0,0,1});

  position.set(position.get() + (peripheralSpeed * dt.count()));
  position.lastUpdate = now;
}

void Estimator::readPos() {
  std::cout << "Position: " << position.get() << std::endl;
}