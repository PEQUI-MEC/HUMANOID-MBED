#include "DataManager.h"

DataManager::DataManager() {
  int i = 0;
  for (i = 0; i < NUM_SERVOS; i++)
    this->desiredPosition[i] = config::initialPosition[i];
}

uint16_t DataManager::getRealPosition(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->realPosition[id-1];
}

uint16_t DataManager::getIBus(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->iBus[id-1];
}

uint16_t DataManager::getDesiredPosition(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->desiredPosition[id-1];
}

void DataManager::setRealPosition(uint8_t id, uint16_t p) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->realPosition[id-1] = p;
}

void DataManager::setIBus(uint8_t id, uint16_t i) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->iBus[id-1] = i;
}

void DataManager::setDesiredPosition(uint8_t id, uint16_t p) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->desiredPosition[id-1] = p;
}
