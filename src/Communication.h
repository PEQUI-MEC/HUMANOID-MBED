#pragma once

#include "BufferSerial.h"
#include "DataManager.h"
#include "config.h"
#include "mbed.h"

class Communication {
 public:
  Communication(PinName tx, PinName rx, uint32_t baud = 115200);
  ~Communication();
  void start(void);

  static const uint8_t SIGNAL_CONTINUE = 1;
  static const uint8_t STATUS_PENDING = 0;
  static const uint8_t STATUS_ERROR = 1;
  static const uint8_t STATUS_TIMEOUT = 2;
  static const uint8_t STATUS_DATA_TIMEOUT = 3;
  static const uint8_t STATUS_INVALID = 4;
  static const uint8_t STATUS_DONE = 10;

 private:
  void loop(void);
  void signalPeriod(void);
  void sendError(void);
  void sendTimeout(void);
  void sendUpdate(void);
  uint8_t readGoal(void);

  bool readBytes(uint8_t* data, uint8_t size, uint16_t timeout = 0);
  void writeBytes(uint8_t* data, uint8_t size);
  uint8_t checksum(uint8_t cmd, uint8_t size, uint8_t* data, uint8_t dataSize);

  void updateGoals(uint8_t* pos, uint8_t size);

  static const uint8_t serialBufferSize = 50;

  uint16_t count;  // TO REMOVE
  Timer timer;     // TO REMOVE

  uint32_t baudRate;
  Thread thread;
  Timeout timeout;
  DigitalOut led;
  BufferSerial serial;
  DataManager* data;
};
