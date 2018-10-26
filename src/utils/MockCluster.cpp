#include "MockCluster.h"

MockCluster::MockCluster() {
  this->size = NUM_SERVOS;
}

MockCluster::~MockCluster() {
  printf("WARN: Destructing MockCluster...");
  this->thread.terminate();
}

void MockCluster::start() {
  this->thread.start(callback(this, &MockCluster::run));
}

void MockCluster::run() {
  uint8_t id = 0;
  DataManager& data = DataManager::getInstance();

  while (true) {
    for (id = 1; id <= this->size; id++) {
      int16_t pos = data.getGoalPosition(id);
      data.setRealPosition(id, pos);
    }

    wait_ms(1);
  }
}
