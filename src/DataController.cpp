#include "DataController.h"

DataController::DataController() {
  int i = 0;
  for (i = 0; i < NUM_SERVOS; i++)
    this->desiredPosition[i] = config::initialPosition[i];
}

uint16_t DataController::getRealPosition(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->realPosition[id-1];
}

uint16_t DataController::getIBus(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->iBus[id-1];
}

uint16_t DataController::getDesiredPosition(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->desiredPosition[id-1];
}

void DataController::setRealPosition(uint8_t id, uint16_t p) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->realPosition[id-1] = p;
}

void DataController::setIBus(uint8_t id, uint16_t i) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->iBus[id-1] = i;
}

void DataController::setDesiredPosition(uint8_t id, uint16_t p) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->desiredPosition[id-1] = p;
}
