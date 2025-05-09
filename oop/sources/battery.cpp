
#include "../headers/battery.hpp"

float Battery::voltage = 0.0f;

void Battery::updateFromADC(uint8_t data) {
  voltage = (float)data;
}