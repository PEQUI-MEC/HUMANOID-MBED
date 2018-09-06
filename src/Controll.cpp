#include "Controll.h"

Controll::Controll(PinName tx, PinName rx, uint32_t baud): serial(tx, rx, dataSize) {
  baudRate = baud;
  serial.baud(baud);
}

bool Controll::readBytes(uint8_t* data, uint8_t size, uint16_t timeout) {
  int byte = 0;
  Timer timer;
  timer.start();

  while (byte < size) {
    if (serial.readable()) {
      data[byte] = (uint8_t)serial.getc();
      byte++;
    } else {
      wait(1.0 / baudRate);
    }
  }
  return !(byte < size);
}

void Controll::writeBytes(uint8_t* data, uint8_t size) {
  for (int i = 0; i < size; i++) {
    serial.putc(data[i]);
  }
}

uint8_t Controll::checksum(uint8_t* data, uint8_t size) {
  uint8_t checksum = header;
  for (uint8_t i = 0; i < size; i++)
    checksum ^= data[i];
  return checksum;
}

void Controll::sendPositions() {
  DataController& dc = DataController::getInstance();
  uint8_t data[dataSize];

  data[0] = header;

  for (uint8_t i = 0; i < NUM_SERVOS; i++)
    data[i + 1] = dc.getRealPosition(i);

  data[dataSize - 1] = this->checksum(data);

  writeBytes(data, dataSize);
}

bool Controll::readResponse() {
  DataController& dc = DataController::getInstance();

  uint8_t response[dataSize];
  bool success = readBytes(response);

  if (!success) return false;
  if (response[0] != header) return false;
  if (response[dataSize-1] != checksum(response)) return false;

  for (uint8_t i = 0; i < dataSize - 2; i += 2) {
    uint16_t pos = (response[i] << 8) + response[i+1];
    dc.setDesiredPosition(i/2, pos);
  }

  return true;
}

void Controll::requestUpdate() {
  bool success = false;
  while (!success) {
    sendPositions();
    success = readResponse();
  }
}
