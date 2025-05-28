#include <iostream>
#include "oop/headers/motors.hpp"
#include "serial/headers/imu.hpp"
#include "oop/headers/estimator.hpp"
#include "oop/headers/shell.hpp"
#include "oop/headers/battery.hpp"
#include <cstring>
#include <thread>
#include <cmath>
#include <atomic>

std::atomic<bool> running{true};

void thing() {
  IMUState state;
  while (running) {
    state = IMU::getState();
    std::cout << "Quaternione: " << state.orientation.w << " | " << state.orientation.x << " | " << state.orientation.y << " | " << state.orientation.z << "\n";
    std::cout << "Velocità angolare: " << state.angularVelocity.x << " | " << state.angularVelocity.y << " | " << state.angularVelocity.z << "\n";
    std::cout << "Temperatura: " << state.temperature << "\n";
    std::cout << "Batteria: " << Battery::getVoltage() << "V" << "\n";
    std::cout << "=============" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50 ));
  }
}

int main(int argc, char* argv[]) {

  Shell::ProcessParameters(argc, argv);

  if (argc >= 2 && !strcmp(argv[1], "stop")) {
    Motors::PowerOff();
    return 0;
  }

  IMU::Initialize();

  std::thread imuThread(thing);

  Motors::Initialize();
  Motors::PowerOn();

  Vector3 vel(0,0,0);
  const float velocity = 10.0f;
  for (int j = 0; j < 5; j++)
    for (int i = 0; i < 360; i++) {
      vel.x = velocity * cosf(i * M_PI / 180);
      vel.y = velocity * sinf(i * M_PI / 180);
      Motors::SetSpeed(&vel);
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

  Motors::PowerOff();
  running = false;
  imuThread.join();
  IMU::Destroy();
  
  return 0;
}
