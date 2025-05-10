
#pragma once

#include <iostream>
#include <vector>
#include "../../calculus/headers/vectors.hpp"

#define MOTOR_MAX_SPEED 80000.0f // rad/s
#define BALL_DIAMETER 0.173f // m
#define WHEEL_DIAMETER 0.08f // m
const float BALL_WHEEL_RATIO = (BALL_DIAMETER / WHEEL_DIAMETER);

#define CMD_POWER_ON 0x01
#define CMD_POWER_OFF 0x02
#define CMD_SET_SPEED 0x03

union Speed {
  float f;
  uint8_t bytes[sizeof(float)];
};

struct __attribute__((packed)) Communication {
  uint8_t header;
  Speed data[3];

  bool operator==(const Communication& other) const {
    return data[0].f == other.data[0].f &&
           data[1].f == other.data[1].f &&
           data[2].f == other.data[2].f;
  }
};

enum class MotorPosition {
  FRONT,
  REAR_LEFT,
  REAR_RIGHT
};

struct Motor {
  Vector3 position;
  Vector3 normal;
  Speed speed;
  bool active;
  MotorPosition type;
  Motor(MotorPosition pos);
};

class Motors {
  private:
    static Vector3 speed;
    static Motor front, rear_r, rear_l;
  
    static void computeVectors(Vector3 *vel);
    static void writeSpeed();
    static void updateEstimator(Vector3 vel);
  
    Motors () {}
    ~Motors() {}

  public:
    static void Initialize();
    static void SetSpeed(Vector3 *vel);
    static void PowerOn();
    static void PowerOff();
};