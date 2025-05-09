#include <iostream>
#include "oop/headers/motors.hpp"
#include "serial/headers/imu.hpp"
#include <cstring>
#include <thread>

void shellThread() {
  std::string cmd;
  while (true) {
    std::getline(std::cin, cmd);
    if (cmd == "exit") break;
    std::cout << "Comando: " << cmd << std::endl;
    if (cmd == "stop") {
      return;
    }
  }
}

auto stort = std::chrono::high_resolution_clock::now();
int media = 0;
int variance = 0;
int count = 0;

int main(int argc, char* argv[]) {
  if (argc >= 2 && !strcmp(argv[1], "stop")) {
    Motors::powerOff();
    return 0;
  }

  IMU::RegisterRegUpdateCallback([](WitRegType reg, uint16_t *data, size_t len) {
    if (reg != WitRegType::q0) return;
    auto now = std::chrono::high_resolution_clock::now() - stort;
    auto n = std::chrono::duration_cast<std::chrono::microseconds>(now).count();
    media += n;
    variance += abs(n - media/count);
    count++;

    stort = std::chrono::high_resolution_clock::now();
  });

  // IMU::RegisterRegUpdateCallback([](WitRegType reg, uint16_t *data, size_t len) {
  //   if (reg != WitRegType::RRATE) return;
  //   auto now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
  //   std::cout << "Update RRATE: " << now << std::endl;
  // });

  IMU::Initialize();
  // IMU::SetOutputRate(WitOutputRateType::RRATE_NONE);
  // IMU::SetBandwitdth(WitBandwidthType::BANDWIDTH_10HZ);
  // std::this_thread::sleep_for(std::chrono::seconds(10));
  // IMU::SetOutputRate(WitOutputRateType::RRATE_100HZ);
  // IMU::SetBandwitdth(WitBandwidthType::BANDWIDTH_256HZ);
  // std::this_thread::sleep_for(std::chrono::seconds(10));
  // for (int i=0; i<300; i++) {
  //   IMU::RequestReg(WitRegType::RRATE, 1);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(100));
  // }
  std::this_thread::sleep_for(std::chrono::seconds(15));
  std::cout << "Media: " << (media/count) << " Variance: " << (variance/count) << std::endl;
  IMU::Destroy();
}