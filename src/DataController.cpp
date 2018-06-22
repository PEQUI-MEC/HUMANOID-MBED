#include "DataController.h"

DataController::DataController() {
  int i = 0;
  for (i = 0; i < NUM_SERVOS; i++)
    this->targetP[i] = config::defaultPos[i];
}

uint16_t DataController::getServoP(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->servoP[id-1];
}

uint16_t DataController::getServoI(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->servoI[id-1];
}

uint16_t DataController::getTargetP(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->targetP[id-1];
}

void DataController::setServoP(uint8_t id, uint16_t p) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->servoP[id-1] = p;
}

void DataController::setServoI(uint8_t id, uint16_t i) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->servoI[id-1] = i;
}

void DataController::setTargetP(uint8_t id, uint16_t p) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->targetP[id-1] = p;
}
