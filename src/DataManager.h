#pragma once

#include "mbed.h"
#include "config.h"

class DataManager {
private:
  DataManager();

  uint16_t realPosition[NUM_SERVOS];
  uint16_t iBus[NUM_SERVOS];
  uint16_t desiredPosition[NUM_SERVOS];

public:
  uint16_t getRealPosition(uint8_t id);
  uint16_t getIBus(uint8_t id);
  uint16_t getDesiredPosition(uint8_t id);

  void setRealPosition(uint8_t id, uint16_t p);
  void setIBus(uint8_t id, uint16_t c);
  void setDesiredPosition(uint8_t id, uint16_t p);

  static DataManager& getInstance() {
      static DataManager instance;
      return instance;
  }
};
