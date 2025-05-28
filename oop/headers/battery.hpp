
#pragma once
#include <iostream>

class Battery {
  private:
    static float voltage;
    Battery() {}
    ~Battery() {}
  
  public:
    static void updateFromADC(uint8_t data);
    static float getVoltage() {
      return voltage;
    }
};
