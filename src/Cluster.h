#pragma once

#include "config.h"

#ifdef CFG_ROBOT_X

#include <array>
#include "BufferSerial.h"
#include "DataManager.h"
#include "SerialServo.h"
#include "mbed.h"
#include "utils/range_map.h"

class Cluster {
 public:
  Cluster(PinName tx, PinName rx, std::array<uint8_t, CLUSTER_SIZE> ids, uint32_t baud = 115200);
  ~Cluster();
  void start(void);
  void loop(void);
  void readPositions(void);

 private:
  uint8_t size;
  std::array<SerialServo, CLUSTER_SIZE> servos;
  BufferSerial serial;
  Thread thread;
};

#endif
