#include "DataManager.h"

DataManager::DataManager() : vref(VREF) {
  int i = 0;
  for (i = 0; i < NUM_SERVOS; i++)
    this->goalPosition[i] = config::initialPosition[i];
}

int16_t DataManager::getRealPosition(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->realPosition[id-1];
}

int16_t DataManager::getGoalPosition(uint8_t id) {
  if ((id <= 0) || (id > NUM_SERVOS)) return -1;
  return this->goalPosition[id-1];
}

void DataManager::setRealPosition(uint8_t id, int16_t p) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->realPosition[id-1] = p;
}

void DataManager::setGoalPosition(uint8_t id, int16_t p) {
  if ((id <= 0) || (id > NUM_SERVOS)) return;
  this->goalPosition[id-1] = p;
}

int16_t DataManager::getPitchServoPosition() {
  return servo_pos_pitch;
}

int16_t DataManager::getYawServoPosition() {
  return servo_pos_yaw;
}

void DataManager::setPitchServoPosition(int16_t p) {
  servo_pos_pitch = p;
}

void DataManager::setYawServoPosition(int16_t p) {
  servo_pos_yaw = p;
}

uint8_t DataManager::getVref() {
  return (uint8_t)(0xFF * vref.read());
}
