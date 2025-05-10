
#include "../headers/motors.hpp"
#include "../../serial/headers/i2c.hpp"
#include "../headers/battery.hpp"
#include <cmath>
#include <algorithm>
#include <thread>


const float SQRT2 = sqrtf(2.0f);
const float SQRT3 = sqrtf(3.0f);
const float SQRT6 = SQRT2 * SQRT3;


Motor::Motor(MotorPosition pos) {
  speed.f = 0;
  switch (pos) {
    case MotorPosition::FRONT:
      position = Vector3(SQRT2/2, 0, SQRT2/2) * BALL_WHEEL_RATIO;
      normal = Vector3(0, 1, 0);
      break;
    case MotorPosition::REAR_LEFT: 
      position = Vector3(-SQRT2/4, SQRT6/4, SQRT2/2) * BALL_WHEEL_RATIO;
      normal = Vector3(-SQRT3/2, -0.5f, 0) ;
      break;
    case MotorPosition::REAR_RIGHT:
      position = Vector3(-SQRT2/4, -SQRT6/4, SQRT2/2) * BALL_WHEEL_RATIO;
      normal = Vector3(SQRT3/2, -0.5F, 0);
      break;
    default:
      throw std::invalid_argument("Posizione motore non valida");
  }
}


Motor Motors::front = Motor(MotorPosition::FRONT);
Motor Motors::rear_r = Motor(MotorPosition::REAR_RIGHT);
Motor Motors::rear_l = Motor(MotorPosition::REAR_LEFT);
Vector3 Motors::speed = Vector3(0, 0, 0);


void Motors::Initialize() {
  #ifdef DEBUG
    std::cout << "---------\nMotors Initializing...\n";
  #endif
  Motors::PowerOff();
  Motors::writeSpeed();
  #ifdef DEBUG
    std::cout << "Motors Initialized\n---------\n";
  #endif
}


void Motors::PowerOff() {
  arduino.open();
  arduino.writeByte(CMD_POWER_OFF);
  #ifdef DEBUG
    std::cout << "Sent I2C Power OFF command\n";
  #endif
  arduino.close();
}
void Motors::PowerOn() {
  arduino.open();
  arduino.writeByte(CMD_POWER_ON);
  #ifdef DEBUG
    std::cout << "Sent I2C Power ON command\n";
  #endif
  arduino.close();
}


void Motors::computeVectors(Vector3 *vel) {

  // Compute the cross product of the velocity vector with the position of each motor
  // and project it onto the normal vector of each motor.
  // Then is calculated the magnitude and verse of the speed for each motor.
  // The speed is finally clamped to the maximum speed defined by MOTOR_MAX_SPEED.

  Vector3 front_comp = vel->cross(front.position).project(front.normal);
  Vector3 rear_l_comp = vel->cross(rear_l.position).project(rear_l.normal);
  Vector3 rear_r_comp = vel->cross(rear_r.position).project(rear_r.normal);

  front.speed.f =  std::clamp(((front_comp.y  < 0) ? -1 : 1) * front_comp .magnitude(), -MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);
  rear_l.speed.f = std::clamp(((rear_l_comp.x > 0) ? -1 : 1) * rear_l_comp.magnitude(), -MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);
  rear_r.speed.f = std::clamp(((rear_r_comp.x < 0) ? -1 : 1) * rear_r_comp.magnitude(), -MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);

  #ifdef DEBUG
    std::cout << "Computed speeds: F " << front.speed.f << ", RL " << rear_l.speed.f << ", RR " << rear_r.speed.f << "\n";
  #endif
}
void Motors::writeSpeed() {
  Communication cmd, result;
  cmd.header = CMD_SET_SPEED;
  cmd.data[0].f = front.speed.f;
  cmd.data[1].f = rear_l.speed.f;
  cmd.data[2].f = rear_r.speed.f;

  arduino.open();
  arduino.write(reinterpret_cast<uint8_t*>(&cmd), sizeof(cmd));
  arduino.read(reinterpret_cast<uint8_t*>(&result), sizeof(result));
  arduino.close();

  #ifdef DEBUG
    std::cout << "Sent I2C speeds: " << result.data[0].f << ", " << result.data[1].f << ", " << result.data[2].f << "\n";
  #endif

  if (!(result == cmd))
    throw std::runtime_error("Errore di comunicazione con ATMega328.");

  Battery::updateFromADC(result.header);
};
void Motors::SetSpeed(Vector3 *vel) {
  Motors::computeVectors(vel);
  Motors::writeSpeed();
  std::thread(&Motors::updateEstimator, speed).detach();
  speed = *vel;
}


void Motors::updateEstimator(Vector3 vel) {
  
}