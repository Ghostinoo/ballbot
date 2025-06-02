
#pragma once
#include <stdint.h>
#include <thread>
#include <atomic>
#include <shared_mutex>
#include <vector>
#include "REGS.h"
#include "../../oop/typings/state.hpp"

// Libreria per WT901


class IMU {
  private:
    IMU() {};
    ~IMU() {};

    static struct InputBuffer {
      uint8_t data[WIT_DATA_BUFF_SIZE];
      uint32_t index;
    } inputBuffer;

    static std::thread dataInputThread;
    static std::atomic<bool> dataInputThreadRunning;

    typedef void (*OnRegUpdateCallback)(WitRegType reg, uint16_t *data, size_t len);
    static std::vector<OnRegUpdateCallback> onRegUpdate;

    static WitRegType readRegIndex;
  
    static int16_t sensorData[REGSIZE];
    static std::shared_mutex sensorDataMutex;
  
    static void copeWithData(WitOutputHeadType regType, uint16_t *data, size_t len);
    static void handleDataIn(uint8_t data);

    static uint8_t checksum(uint8_t *data, size_t len);
    static bool checkRange(short value, short min, short max);

    static void dataInputLoop();

  public:

    static void Initialize();
    static void Destroy() noexcept;
    static void RegisterRegUpdateCallback(void (*callback)(WitRegType, uint16_t *, size_t));

    static bool WriteReg(WitRegType reg, uint16_t data);
    static bool RequestReg(WitRegType reg, size_t count);
    static int16_t ReadReg(WitRegType reg);
    static void ReadRegBulk(WitRegType reg, uint16_t *data, size_t count);

    static bool StartAccelerometerCalibration();
    static bool StopAccelerometerCalibration();
    static bool StartMagnetometerCalibration();
    static bool StopMagnetometerCalibration();

    static bool SetBandwidth(WitBandwidthType bandwidth);
    static bool SetOutputRate(WitOutputRateType rate);
    static bool SetContent(WitContentType rsw);
    static bool SetOrientation();

    static IMUState getState();
};