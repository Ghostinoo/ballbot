
#include "../headers/uart.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

char dev[] = "/dev/ttyS0";
UART wt901 = UART(115200, dev);

UART::UART(int baudRate, char* terminal) noexcept : baud(baudRate), fd(-1), device(terminal) {}

bool UART::open() {return open(baud); }
bool UART::open(int baudRate) {
  fd = ::open(device, O_RDWR | O_NOCTTY );
  if (fd < 0) throw std::runtime_error("Errore nell'apertura della porta UART");
  if (!isatty(STDIN_FILENO)) std::runtime_error("STD Input non è un terminale");
  #ifdef DEBUG
    std::cout << "UART opening device '" << device << "'..." << std::endl;
  #endif

  struct termios tio {};

  if (tcgetattr(fd, &tio) != 0)  throw std::runtime_error("Errore nella lettura della configurazione della porta UART");

  memset(&tio, 0, sizeof(tio));
  tio.c_cflag = CLOCAL | CREAD | CS8;
  tio.c_cflag &= ~(PARENB | CSTOPB);
  tio.c_cc[VTIME] = 0;
  tio.c_cc[VMIN] = 0;
  
  speed_t speed;
  switch (baudRate) {
      case 2400: speed = B2400; break;
      case 4800: speed = B4800; break;
      case 9600: speed = B9600; break;
      case 115200: speed = B115200; break;
      case 230400: speed = B230400; break;
      case 460800: speed = B460800; break;
      default:    speed = B9600; break;
  }
  cfsetspeed(&tio, speed);
  
  tcflush(fd, TCIFLUSH);
  if (tcsetattr(fd, TCSANOW, &tio) != 0) 
      throw std::runtime_error("Failed to configure UART");
  
  #ifdef DEBUG
    std::cout << "UART device '" << device << "' opened with baud rate " << baudRate << std::endl;
  #endif

  return true;
}

void UART::close() noexcept {
  try { if (fd >= 0) ::close(fd); } catch(...) {}
  #ifdef DEBUG
    std::cout << "UART device '" << device << "' closed" << std::endl;
  #endif
}

bool UART::write(const uint8_t* data, size_t count) {
  #ifdef DEBUG
    std::cout << "UART writing " << std::dec << count << " bytes to '" << device << "'" << std::endl;
  #endif
  return (::write(fd, data, count) == (ssize_t)count) ? true : throw std::runtime_error("Errore nella scrittura dei dati UART");
}

bool UART::read(uint8_t* data, size_t count) {
  #ifdef DEBUG
    if (count > 1) std::cout << "UART reading " << std::dec << count << " bytes from '" << device << "'" << std::endl;
  #endif
  return (::read(fd, data, count) == (ssize_t)count) ? true : throw std::runtime_error("Errore nella lettura dei dati UART");
}

bool UART::writeNoExcept(const uint8_t* data, size_t count) noexcept {
  try {
    return this->write(data, count);
  } catch (const std::exception& e) {
    #ifdef DEBUG
      std::cerr << "UART write error: " << e.what() << std::endl;
    #endif
    return false;
  }
}

bool UART::readNoExcept(uint8_t* data, size_t count) noexcept {
  try {
    return this->read(data, count);
  } catch (const std::exception& e) {
    #ifdef DEBUG
      std::cerr << "UART read error: " << e.what() << std::endl;
    #endif
    return false;
  }
}