
#include "oop/headers/motors.hpp"
#include "serial/headers/imu.hpp"
#include "oop/headers/estimator.hpp"
#include "oop/headers/battery.hpp"
#include "helpers/helpers.hpp"
#include <iostream>
#include <cstring>
#include <thread>
#include <cmath>
#include <atomic>
#include "conf.h"

std::atomic<bool> running{true};


void thing() {
  IMUState state;
  float theta;
  // Vector3 axis;
  const Vector3 base(0, 0, 1);
  auto now = std::chrono::steady_clock::now();
  while (running) {
    state = IMU::getState();

    theta = acosf(base.dot(state.orv) / (base.magnitude() * state.orv.magnitude()));

    // std::cout << "Quaternione: " << state.orientation.w << " | " << state.orientation.x << " | " << state.orientation.y << " | " << state.orientation.z << "\n";
    // std::cout << "Roll: " << state.orv.x << "° | Pitch: " << state.orv.y << "° | Yaw: " << state.orv.z << "°\n";
    // std::cout << "Velocità angolare: " << state.angularVelocity.x << " | " << state.angularVelocity.y << " | " << state.angularVelocity.z << "\n";
    // std::cout << "Temperatura: " << state.temperature << "\n";
    // std::cout << "Batteria: " << Battery::getVoltage() << "V" << "\n";
    std::cout << "Angolo errore: " << theta * 180.0f / M_PI << "°\n";
    std::cout << "=============" << std::endl;

    std::this_thread::sleep_until(now + std::chrono::milliseconds(12));
    now = std::chrono::steady_clock::now();
  } 
  std::cout << "Thread terminato." << std::endl;
}

int main(int argc, char* argv[]) {
  try {

    AssignPriority();

    IMU::Initialize();
    Motors::Initialize();

    InitializeShell(argc, argv);

    IMU::SetBandwidth(WitBandwidthType::BANDWIDTH_44HZ);
    IMU::SetOutputRate(WitOutputRateType::RRATE_200HZ);
    IMU::SetContent(
      WitContentType::RSW_ACC +
      WitContentType::RSW_GYRO +
      WitContentType::RSW_ANGLE +
      WitContentType::RSW_V
    );

    // Motors::PowerOn();

    std::thread imuThread(thing);  


    imuThread.join();
    Motors::PowerOff();
    IMU::Destroy();
    
    return 0;
  }
  catch (const Returner& e) {
    if (e.isError()) std::cout << "Errore: ";
    std::cout << e.what() << std::endl;
    if (e.shouldTerminate()) {
      Motors::PowerOff();
      IMU::Destroy();
      return 0;
    }
  }

  return 0;
}