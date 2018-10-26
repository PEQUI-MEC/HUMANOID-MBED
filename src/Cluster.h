#pragma once

#include <array>
#include "BufferSerial.h"
#include "DataManager.h"
#include "XYZrobotServo.h"
#include "config.h"
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
  std::array<XYZrobotServo, CLUSTER_SIZE> servos;
  BufferSerial serial;
  Thread thread;
};
