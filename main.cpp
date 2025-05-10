#include <iostream>
#include "oop/headers/motors.hpp"
#include "serial/headers/imu.hpp"
#include <cstring>
#include <thread>

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


int main(int argc, char* argv[]) {
  if (argc >= 2 && !strcmp(argv[1], "stop")) {
    Motors::PowerOff();
    return 0;
  }


  
}