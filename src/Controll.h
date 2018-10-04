#pragma once

#include "BufferSerial.h"
#include "DataManager.h"
#include "config.h"
#include "mbed.h"

class Controll {
 public:
  Controll(PinName tx, PinName rx, uint32_t baud = 115200);
  void requestUpdate(void);

 private:
  void readBytes(uint8_t* data, uint8_t size);
  void writeBytes(uint8_t* data, uint8_t size);
  uint8_t checksum(uint8_t cmd, uint8_t size, uint8_t* data, uint8_t dataSize);
  void sendPositions(void);
  bool readResponse(void);
  void updatePositions(uint8_t* pos, uint8_t size);

  static const uint8_t serialBufferSize = 50;

  uint32_t baudRate;
  BufferSerial serial;
  DataManager* data;
};
