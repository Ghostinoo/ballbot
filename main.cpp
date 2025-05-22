#include <iostream>
#include "oop/headers/motors.hpp"
#include "serial/headers/imu.hpp"
#include "oop/headers/estimator.hpp"
#include <cstring>
#include <thread>
#include <cmath>

// void shellThread() {
//   std::string cmd;
//   while (true) {
//     std::getline(std::cin, cmd);
//     if (cmd == "exit") break;
//     std::cout << "Comando: " << cmd << std::endl;
//     if (cmd == "stop") {
//       return;
//     }
//   }
// }

// void cb2(WitRegType reg, uint16_t *data, size_t len) {
//   if (reg != WitRegType::q0) return;
//   std::cout << "Quat: " << data[0] << " | " << data[1] << " | " << data[2] << std::endl;
// }

const float scalar = 200.0f;
#define DTR (M_PI/180.0f)

Vector3 vel;

int main(int argc, char* argv[]) {
  if (argc >= 2 && !strcmp(argv[1], "stop")) {
    Motors::PowerOff();
    return 0;
  }

  IMU::Initialize();
  // IMU::RegisterRegUpdateCallback(cb2);

  auto now = std::chrono::high_resolution_clock::now();

  while (true) {
    now = std::chrono::high_resolution_clock::now();

    IMUState state = IMU::getState();

    std::cout << "Quaternion " << state.orientation.w << " | " << state.orientation.x << " | " << state.orientation.y << " | " << state.orientation.z << std::endl;
    std::cout << "Angular Velocity " << state.angularVelocity.x << " | " << state.angularVelocity.y << " | " << state.angularVelocity.z << std::endl;
    std::cout << "Temperature " << state.temperature << std::endl;
    std::cout << "==========================" << std::endl;

    std::this_thread::sleep_until(now + std::chrono::milliseconds(20));
  }

  IMU::Destroy();
}
