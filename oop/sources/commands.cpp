
#include "../headers/shell.hpp"
#include "../headers/motors.hpp"
#include "../../serial/headers/imu.hpp"
#include <iostream>


void ShellCommands::setProportionalGain(float gain) {
  std::cout << "Impostato guadagno proporzionale a: " << gain << std::endl;
}

void ShellCommands::stopMotors(bool kill) {
  if (!kill) return;
  Motors::PowerOff();
  throw Returner("Motori fermati.", true, false);
}

void ShellCommands::calibrate(bool calibrate) {
  if (!calibrate) return;
  std::cout << "Inizio calibrazione del dispositivo..." << std::endl;
  IMU::SetOrientation();
  std::cout << "Azzeramento dell'angolo completato con successo." << std::endl;
}