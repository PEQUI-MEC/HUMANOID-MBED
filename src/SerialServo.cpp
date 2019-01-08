#include "SerialServo.h"

SerialServo::SerialServo(uint8_t id, BufferSerial& serial, uint32_t baud, bool reverse, int16_t offset): xyz(id, serial, baud) {
  this->id = id;
  this->reverse = reverse;
  this->offset = offset;
}

uint8_t SerialServo::getId() { return id; }

void SerialServo::setPosition(int16_t pos) {
  int16_t realPos = (pos - offset) * (reverse ? -1 : 1);
  uint16_t xyzPos = range_map(realPos, -1800, 1500, XYZ_POS_MIN, XYZ_POS_MAX);
  xyz.setPosition(xyzPos, PLAYTIME);
}

int16_t SerialServo::readPosition() {
  uint16_t xyzPos = xyz.readPosition() + offset;
  if (getLastError()) return 0x8000;
  return range_map(xyzPos, XYZ_POS_MIN, XYZ_POS_MAX, -1800, 1500);
}

uint8_t SerialServo::getLastError() {
  return xyz.getLastError();
}
