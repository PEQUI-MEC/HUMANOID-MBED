#pragma once

#include "mbed.h"
#include "config.h"

class DataController {
private:
  DataController();

  uint16_t servoP[NUM_SERVOS];
  uint16_t servoI[NUM_SERVOS];
  uint16_t targetP[NUM_SERVOS];

public:
  uint16_t getServoP(uint8_t id);
  uint16_t getServoI(uint8_t id);
  uint16_t getTargetP(uint8_t id);

  void setServoP(uint8_t id, uint16_t p);
  void setServoI(uint8_t id, uint16_t c);
  void setTargetP(uint8_t id, uint16_t p);

  static DataController& getInstance() {
      static DataController instance;
      return instance;
  }
};
