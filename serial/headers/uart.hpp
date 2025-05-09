
#pragma once

#include <iostream>

class UART {
  private:
    int baud;
    int fd;
    char* device;
  
  public:
    UART(int baudRate, char *terminal) noexcept;

    bool open();
    bool open(int baudRate);
    bool write(const uint8_t* data, size_t count);
    bool read(uint8_t* data, size_t count);
    bool writeNoExcept(const uint8_t* data, size_t count) noexcept;
    bool readNoExcept(uint8_t* data, size_t count) noexcept;
    void close();
};

extern UART wt901;