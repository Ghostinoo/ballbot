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


const float scalar = 15.0f;
#define DTR (M_PI/180.0f)

Vector3 vel;

int main(int argc, char* argv[]) {
  if (argc >= 2 && !strcmp(argv[1], "stop")) {
    Motors::PowerOff();
    return 0;
  }

  Motors::Initialize();
  Motors::PowerOn();

  for (int i = 0; i < 3; i++)
    for (float i = 0; i < 360; i++)
    {
      vel = scalar * Vector3(cosf(i*DTR), sin(i*DTR), 0);
      Motors::SetSpeed(&vel);
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      Estimator::readPos();
    }

  Motors::PowerOff();
}
