
#include "../headers/uart.hpp"
#include "../headers/imu.hpp"
#include <cstring>
#include <chrono>
#include <mutex>


IMU::InputBuffer IMU::inputBuffer = {{},0};

std::thread IMU::dataInputThread;
std::atomic<bool> IMU::dataInputThreadRunning = false;

WitRegType IMU::readRegIndex;

IMU::OnRegUpdateCallback IMU::onRegUpdate[] = {nullptr};
int IMU::onRegUpdateIndex = 0;

uint16_t IMU::sensorData[REGSIZE] = {0};
std::shared_mutex IMU::sensorDataMutex;


void IMU::copeWithData(WitOutputHeadType regType, uint16_t *data, size_t len) {

  struct RegDescriptor {
    WitRegType reg;
    size_t size = 0;
    uint16_t *pointer;
  } reg1, reg2;

  reg1.size = 4;
  reg1.pointer = data;
  reg2.pointer = data + 3;

  switch (regType) {
    case WIT_ACC:       reg1.reg = AX;    reg1.size = 3; reg2.reg = TEMP;     reg2.size = 1; break;
    case WIT_ANGLE:     reg1.reg = Roll;  reg1.size = 3; reg2.reg = VERSION;  reg2.size = 1; break;
    case WIT_GYRO:      reg1.reg = GX;    reg1.size = 3; break;
    case WIT_MAGNETIC:  reg1.reg = HX;    reg1.size = 3; break;
    case WIT_TIME:      reg1.reg = YYMM; break;
    case WIT_DPORT:     reg1.reg = D0Status; break;
    case WIT_PRESS:     reg1.reg = PressureL; break;
    case WIT_GPS:       reg1.reg = LonL; break;
    case WIT_VELOCITY:  reg1.reg = GPSHeight; break;
    case WIT_QUATER:    reg1.reg = q0;  break;
    case WIT_GSA:       reg1.reg = SVNUM;  break;
    case WIT_REGVALUE:  reg1.reg = readRegIndex;  break;
    default: return;
  }

  if (len == 3) { reg1.size = 3; reg2.size = 0; }

  std::unique_lock<std::shared_mutex> lock(sensorDataMutex);

  memcpy(&sensorData[reg1.reg], reg1.pointer, reg1.size);
  for (int i = 0; i < onRegUpdateIndex; i++)
    if (onRegUpdate[i] != nullptr)
      std::thread(onRegUpdate[i], reg1.reg, reg1.pointer, reg1.size).detach();
  #ifdef DEBUG
    std::cout << "IMU Reg Update: 0x" << std::hex << reg1.reg << " - Data: ";
    for (size_t i = 0; i < reg1.size; i++) std::cout << std::hex << (int)reg1.pointer[i];
    std::cout << std::endl;
  #endif

  if (reg2.size > 0) {
    memcpy(&sensorData[reg2.reg], reg2.pointer, reg2.size);
    for (int i = 0; i < onRegUpdateIndex; i++)
      if (onRegUpdate[i] != nullptr)
        std::thread(onRegUpdate[i], reg2.reg, reg2.pointer, reg2.size).detach();
    #ifdef DEBUG
      std::cout << "IMU Reg Update: 0x" << std::hex << reg2.reg << " - Data: ";
      for (size_t i = 0; i < reg2.size; i++) std::cout << std::hex << (int)reg1.pointer[i];
      std::cout << std::endl;
    #endif
  }
}
void IMU::handleDataIn(uint8_t data) {

  union RegisterInputData {
    uint8_t dataIn[8];
    uint16_t data[4];
  };

  inputBuffer.data[inputBuffer.index++] = data;
  if (inputBuffer.data[0] != 0x55)
  {
    inputBuffer.index--;
    memcpy(inputBuffer.data, inputBuffer.data + 1, inputBuffer.index);
    return;
  }
  if (inputBuffer.index >= 11)
  {
    if (checksum(inputBuffer.data, inputBuffer.index - 1) != inputBuffer.data[inputBuffer.index - 1])
    {
      inputBuffer.index--;
      memcpy(inputBuffer.data, inputBuffer.data + 1, inputBuffer.index);
      return;
    }
    RegisterInputData regData;
    memcpy(regData.dataIn, inputBuffer.data + 2, sizeof(regData.dataIn));
    copeWithData((WitOutputHeadType)inputBuffer.data[1], regData.data, sizeof(regData.data));
    inputBuffer.index = 0;
  }
  if (inputBuffer.index >= WIT_DATA_BUFF_SIZE)
    inputBuffer.index = 0;
}


uint8_t IMU::checksum(uint8_t *data, size_t len) {
  uint8_t result = 0;
  for (uint32_t i = 0; i < len; i++) result += *(data + i);
  return result;
}
bool IMU::checkRange(short value, short min, short max) {
  return (value >= min && value <= max);
}


bool IMU::WriteReg(WitRegType reg, uint16_t data) {
  try {
    uint8_t buffer[5];
    buffer[0] = 0xFF;
    buffer[1] = 0xAA;
    buffer[2] = reg & 0xFF;
    buffer[3] = data & 0xFF;
    buffer[4] = data >> 8;
    return wt901.write(buffer, 5);
  } catch (const std::exception &e) {
    throw std::runtime_error("Errore nella scrittura del registro: " + std::to_string(reg));
    return false;
  }
}
bool IMU::RequestReg(WitRegType reg, size_t count) {
  if ((reg + count) >= REGSIZE || count > 4) throw std::invalid_argument("Registro o dimensione di lettura invalidi.");
  try { WriteReg(READADDR, reg); }
  catch (const std::exception &e) {
    throw std::runtime_error("Errore nella richiesta di lettura del registro: " + std::to_string(reg));
    return false;
  }
  readRegIndex = reg;
  return true;
}
uint16_t IMU::ReadReg(WitRegType reg) {
  if (reg >= REGSIZE) throw std::invalid_argument("Registro non valido.");
  std::shared_lock<std::shared_mutex> lock(sensorDataMutex);
  return sensorData[reg];
}
void IMU::ReadRegBulk(WitRegType reg, uint16_t *data, size_t count) {
  if ((reg + count) >= REGSIZE) throw std::invalid_argument("Registro o dimensione di lettura invalidi.");
  std::shared_lock<std::shared_mutex> lock(sensorDataMutex);
  memcpy(data, &sensorData[reg], count);
}


bool IMU::StartAccelerometerCalibration() {
  try {

    WriteReg(KEY, KEY_UNLOCK);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(CALSW, CALGYROACC);
    return true;

  } catch (const std::exception &e) {
    throw std::runtime_error("Errore nell'avvio della procedura di calibrazione dell'accelerometro.");
    return false;
  }
}
bool IMU::StopAccelerometerCalibration() {
  try {

    WriteReg(CALSW, NORMAL);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(SAVE, SAVE_PARAM);
    return true;

  } catch (const std::exception &e) {
    throw std::runtime_error("Errore nell'arresto della procedura di calibrazione dell'accelerometro.");
    return false;
  }
}
bool IMU::StartMagnetometerCalibration() {
  try {

    WriteReg(KEY, KEY_UNLOCK);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(CALSW, CALMAGMM);
    return true;

  } catch (const std::exception &e) {
    throw std::runtime_error("Errore nell'avvio della procedura di calibrazione del magnetometro.");
    return false;
  }
}
bool IMU::StopMagnetometerCalibration() {
  try {

    WriteReg(KEY, KEY_UNLOCK);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(CALSW, NORMAL);
    return true;

  } catch (const std::exception &e) {
    throw std::runtime_error("Errore nell'arresto della procedura di calibrazione del magnetometro.");
    return false;
  }
}


bool IMU::SetBandwitdth(WitBandwidthType bandwidth) {
  if (!checkRange(bandwidth, BANDWIDTH_256HZ, BANDWIDTH_5HZ)) {
    throw std::invalid_argument("Larghezza di banda non valida.");
    return false;
  }
  try {
    WriteReg(KEY, KEY_UNLOCK);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(BANDWIDTH, bandwidth);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(SAVE, SAVE_PARAM);
    return true;
  } catch (const std::exception &e) {
    throw std::runtime_error("Errore nella scrittura della larghezza di banda.");
    return false;
  }
}
bool IMU::SetOutputRate(WitOutputRateType rate) {
  if (!checkRange(rate, RRATE_02HZ, RRATE_NONE)) {
    throw std::invalid_argument("Frequenza di campionamento non valida.");
    return false;
  }
  try {
    WriteReg(KEY, KEY_UNLOCK);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(RRATE, rate);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(SAVE, SAVE_PARAM);
    return true;
  } catch (const std::exception &e) {
    throw std::runtime_error("Errore nella scrittura della frequenza di campionamento.");
    return false;
  }
}
bool IMU::SetContent(WitContentType rsw) {
  if (!checkRange(rsw, RSW_TIME, RSW_MASK)) {
    throw std::invalid_argument("Contenuti non valido.");
    return false;
  }
  try {
    WriteReg(KEY, KEY_UNLOCK);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(RSW, rsw);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    WriteReg(SAVE, SAVE_PARAM);
    return true;
  } catch (const std::exception &e) {
    throw std::runtime_error("Errore nella scrittura dei contenuti.");
    return false;
  }
}
WitContentType operator+(WitContentType a, WitContentType b) {
  return static_cast<WitContentType>(static_cast<int>(a) | static_cast<int>(b));
}


void IMU::Initialize() {
  wt901.open();
  dataInputThreadRunning = true;
  dataInputThread = std::thread(dataInputLoop);
}
void IMU::Destroy() {
  dataInputThreadRunning = false;
  dataInputThread.join();
  wt901.close();
}
void IMU::RegisterRegUpdateCallback(void (*callback)(WitRegType, uint16_t *, size_t)) {
  if (callback == nullptr) throw std::invalid_argument("Callback non valida.");
  onRegUpdate[onRegUpdateIndex++] = callback;
  #ifdef DEBUG
    std::cout << "IMU Callback Registered at position: " << onRegUpdateIndex << std::endl;
  #endif
};

void IMU::dataInputLoop() {
  uint8_t buffer[1];
  while (dataInputThreadRunning)
    while (wt901.readNoExcept(buffer, 1) && dataInputThreadRunning)
      handleDataIn(buffer[0]);
}
