#pragma once

#include "BufferSerial.h"
#include "DataController.h"
#include "config.h"
#include "mbed.h"

class Controll {
 public:
  Controll(PinName tx, PinName rx, uint32_t baud = 115200);
  void requestUpdate(void);

 private:
  bool readBytes(uint8_t* data, uint8_t size = dataSize, uint16_t timeout = 0);
  void writeBytes(uint8_t* data, uint8_t size = dataSize);
  uint8_t checksum(uint8_t* data, uint8_t size = dataSize - 1);
  void sendPositions(void);
  bool readResponse(void);

  static const uint8_t header = P_HEADER;
  static const uint8_t dataSize = P_SIZE;

  uint8_t buffer[dataSize];
  uint32_t baudRate;
  BufferSerial serial;
  DataController* data;
};
