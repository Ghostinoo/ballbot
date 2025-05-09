
#include "../headers/i2c.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <iostream>


I2C arduino = I2C(0x1A);

I2C::I2C(int deviceAttr) : device(deviceAttr), bus(1), fr(-1), fw(-1) {}

bool I2C::open() {
  std::string path = "/dev/i2c-" + std::to_string(bus);
  fr = ::open(path.c_str(), O_RDONLY);
  fw = ::open(path.c_str(), O_WRONLY);
  if (fr < 0 || fw < 0) return false;

  if (ioctl(fr, I2C_SLAVE, device) < 0 || ioctl(fw, I2C_SLAVE, device) < 0)
    throw std::runtime_error("Errore nella configurazione del dispositivo I2C");

  #ifdef DEBUG
    std::cout << "I2C device opened: " << std::hex << device << " on bus " << bus << std::endl;
  #endif

  return true;
}

bool I2C::write(const uint8_t* data, size_t count) {
  #ifdef DEBUG
    std::cout << "I2C writing "<< std::dec << count << " bytes to " << std::hex << device << std::endl;
  #endif
  return (::write(fw, data, count) == (ssize_t)count) ? true : throw std::runtime_error("Errore nella scrittura dei dati I2C");
}

bool I2C::read(uint8_t* data, size_t count) {
  #ifdef DEBUG
    std::cout << "I2C reading " << std::dec << count << " bytes from " << std::hex << device << std::endl;
  #endif
  return (::read(fr, data, count) == (ssize_t)count) ? true : throw std::runtime_error("Errore nella lettura dei dati I2C");
}

bool I2C::writeByte(uint8_t data) {
  #ifdef DEBUG
    std::cout << "I2C writing byte 0x" << std::hex << (int)data << " to " << device << std::endl;
  #endif
  return (::write(fw, &data, 1) == 1) ? true : throw std::runtime_error("Errore nella scrittura dei dati I2C");
}

void I2C::close() {
  if (fr >= 0) ::close(fr);
  if (fw >= 0) ::close(fw);
  #ifdef DEBUG
    std::cout << "I2C device closed: " << std::hex << device << " on bus " << bus << std::endl;
  #endif
}


// Version with std::vector

// bool I2C::write(const std::vector<uint8_t>& data) {
//   return (::write(fw, data.data(), data.size()) == (ssize_t)data.size()) ? true : throw std::runtime_error("Errore nella scrittura dei dati I2C");
// }

// std::vector<uint8_t> I2C::read(size_t count) {
//   std::vector<uint8_t> buffer(count);
//   if (!(::read(fr, buffer.data(), count) == (ssize_t)count))
//     throw std::runtime_error("Errore nella lettura dei dati I2C");
//   return buffer;
// }