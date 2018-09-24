#pragma once

#include <vector>
#include "mbed.h"
#include "BufferSerial.h"
#include "XYZrobotServo.h"
#include "config.h"
#include "DataManager.h"

class Cluster {
public:
  Cluster(PinName tx, PinName rx, uint8_t* ids, uint8_t length, uint32_t baud = 115200);
  void start(void);
  void run(void);
  void readPositions(void);

  static void thread_starter(void const *p);

private:
  uint8_t size;
  std::vector<XYZrobotServo> servos;
  BufferSerial serial;
  Thread* thread;
  DataManager* data;
};
