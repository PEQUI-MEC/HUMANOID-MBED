#include "PwmCluster.h"

#ifdef CFG_F

PwmCluster::PwmCluster()
  : servos({{
        {1, PWM1, AD1},
        {2, PWM2, AD2},
        {3, PWM3, AD3},
        {4, PWM4, AD4},
        {5, PWM5, AD5},
        {6, PWM6, AD6},
        {7, PWM7, AD7},
        {8, PWM8, AD8},
        {9, PWM9, AD9},
        {10, PWM10, AD10},
        {11, PWM11, AD11},
        {12, PWM12, AD12},
    }}) {
  for (uint8_t i = 0; i < servos.size(); i++) {
    uint8_t id = servos[i].getId();
    DigitalOut en(config::enablePin[id - 1], ENABLE_ACTIVE);
    wait_ms(INIT_WAIT);
  }
}

PwmCluster::~PwmCluster() {
  printf("WARN: Destructing PwmCluster...");
  this->thread.terminate();
  for (uint8_t i = 0; i < servos.size(); i++)
    DigitalOut en(config::enablePin[servos[i].getId() - 1], !ENABLE_ACTIVE);
}

void PwmCluster::start() {
  thread.start(callback(this, &PwmCluster::loop));
}

void PwmCluster::loop() {
  DataManager& data = DataManager::getInstance();
  uint8_t i;

  while (true) {
    for (i = 0; i < servos.size(); i++) {
      uint8_t id = servos[i].getId();
      servos[i].setPosition(data.getGoalPosition(id));
      data.setRealPosition(id, servos[i].readPosition());
    }
  }
}

#endif
