
#pragma once

#include "../../calculus/headers/vectors.hpp"
#include "../../calculus/headers/quaternions.hpp"
#include <shared_mutex>
#include <mutex>
#include <chrono>


class Estimator {
  private:

    static struct IMUState {
      Vector3 pos;
      Quaternion base;
      Quaternion ball;
    } state;

    static struct Position {
      private:
        Vector3 value;
        std::shared_mutex mutex;
      public:
        std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdate;
        void set(const Vector3& newValue);
        Vector3 get();
    } position;

    Estimator() {}
    ~Estimator() {}

  public:
    static void updatePosition(Vector3 pos);
    static void readPos();
};