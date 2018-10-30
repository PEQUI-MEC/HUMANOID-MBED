#pragma once

#include "config.h"

#ifdef CFG_ROBOT_F

#include <array>
#include "DataManager.h"
#include "PwmServo.h"
#include "mbed.h"

class PwmCluster {
 public:
  PwmCluster();
  ~PwmCluster();
  void start(void);
  void loop(void);

 private:
  std::array<PwmServo, NUM_SERVOS> servos;
  Thread thread;
};

#endif
