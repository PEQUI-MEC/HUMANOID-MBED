#pragma once

#include "mbed.h"
#include "config.h"

class DataManager {
 private:
  DataManager();

  int16_t realPosition[NUM_SERVOS];
  int16_t goalPosition[NUM_SERVOS];
  int16_t servo_pos_pitch;
  int16_t servo_pos_yaw;
  AnalogIn vref;

 public:
  int16_t getRealPosition(uint8_t id);
  int16_t getGoalPosition(uint8_t id);
  void setRealPosition(uint8_t id, int16_t p);
  void setGoalPosition(uint8_t id, int16_t p);

  int16_t getPitchServoPosition();
  int16_t getYawServoPosition();
  void setPitchServoPosition(int16_t p);
  void setYawServoPosition(int16_t p);

  uint8_t getVref();

  static DataManager& getInstance() {
    static DataManager instance;
    return instance;
  }
};
