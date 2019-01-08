#pragma once

#include "BufferSerial.h"
#include "XYZrobotServo.h"
#include "config.h"
#include "mbed.h"
#include "utils/range_map.h"

#define XYZ_POS_MIN 0
#define XYZ_POS_MAX 1023

class SerialServo {
 public:
  SerialServo(uint8_t id, BufferSerial& serial, uint32_t baud = 115200, bool reverse = false, int16_t offset = 0);
  ~SerialServo();
  void enable(void);
  void disable(void);

  uint8_t getId();
  void setPosition(int16_t pos);
  int16_t readPosition(void);

  uint8_t getLastError();

 private:
  uint8_t id;
  bool reverse;
  int16_t offset;

  XYZrobotServo xyz;
};
