
#pragma once
#include <iostream>

class I2C {
  private:
    int device;
    int bus;
    int fr, fw;

  public:
    I2C(int deviceAttr);

    bool open();
    bool write(const uint8_t* data, size_t count);
    bool writeByte(uint8_t data);
    bool read(uint8_t* data, size_t count);
    void close();
};

extern I2C arduino;