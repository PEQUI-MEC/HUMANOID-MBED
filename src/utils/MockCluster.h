#pragma once

#include "../DataManager.h"
#include "config.h"
#include "mbed.h"

class MockCluster {
 public:
  MockCluster();
  ~MockCluster();
  void start(void);
  void run(void);

 private:
  uint8_t size;
  Thread thread;
  DataManager* data;
};
