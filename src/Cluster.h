#pragma once

#include <array>
#include "mbed.h"
#include "BufferSerial.h"
#include "XYZrobotServo.h"
#include "config.h"
#include "DataManager.h"

class Cluster {
public:
  Cluster(PinName tx, PinName rx, std::array<uint8_t, CLUSTER_SIZE> ids, uint32_t baud = 115200);
  ~Cluster();
  void start(void);
  void run(void);
  void readPositions(void);

private:
  uint8_t size;
  std::array<XYZrobotServo, CLUSTER_SIZE> servos;
  BufferSerial serial;
  Thread thread;
  DataManager* data;
};
