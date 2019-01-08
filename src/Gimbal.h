#pragma once

#include <array>
#include <cmath>
#include "BNO055.h"
#include "DataManager.h"
#include "PwmServo.h"
#include "config.h"
#include "mbed.h"

class Gimbal {
 public:
  Gimbal();
  ~Gimbal();
  void start(void);
  void loop(void);

 private:
  PwmServo servo_pitch;
  PwmServo servo_yaw;
  Thread thread;
  BNO055 bno;
};
