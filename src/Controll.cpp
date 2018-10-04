#include "Controll.h"

Controll::Controll(PinName tx, PinName rx, uint32_t baud): serial(tx, rx, serialBufferSize) {
  baudRate = baud;
  serial.baud(baud);
  this->data = &DataManager::getInstance();
}

void Controll::readBytes(uint8_t* data, uint8_t size) {
  uint8_t byte = 0;
  while (byte < size) {
    if (serial.readable()) {
      data[byte] = (uint8_t)serial.getc();
      byte++;
    } else {
      wait(1.0 / (float)baudRate);
    }
  }
}

void Controll::writeBytes(uint8_t* data, uint8_t size) {
  for (uint8_t i = 0; i < size; i++)
    serial.putc(data[i]);
}

uint8_t Controll::checksum(uint8_t cmd, uint8_t size, uint8_t* data, uint8_t dataSize) {
  uint8_t checksum = cmd ^ size;
  for (uint8_t i = 0; i < dataSize; i++) checksum ^= data[i];
  return checksum;
}

void Controll::sendPositions() {
  const uint8_t dataSize = NUM_SERVOS * 2;

  uint8_t header[P_HEADER_SIZE];
  header[0] = P_HEADER_BYTE;
  header[1] = P_UPDATE_ID;
  header[2] = P_HEADER_SIZE + dataSize;

  uint8_t data[dataSize];
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
    data[i * 2] = this->data->getRealPosition(i);

  header[3] = this->checksum(header[1], header[2], data, dataSize);

  writeBytes(header, P_HEADER_SIZE);
  writeBytes(data, dataSize);
}

bool Controll::readResponse() {
  uint8_t header[P_HEADER_SIZE];
  readBytes(header, P_HEADER_SIZE);

  if (header[0] != P_HEADER_BYTE) return false;
  if (header[1] == P_ERROR_ID) return false;

  const uint8_t dataSize = header[2] - P_HEADER_SIZE;
  if (dataSize < (NUM_SERVOS * 2)) return false;

  uint8_t data[dataSize];
  readBytes(data, dataSize);

  if (header[3] != checksum(header[1], header[2], data, dataSize))
    return false;

  updatePositions(data, dataSize);
  return true;
}

void Controll::updatePositions(uint8_t* pos, uint8_t size) {
  for (uint8_t i = 0; i < size; i += 2) {
    uint16_t position = (pos[i] << 8) + pos[i + 1];
    this->data->setDesiredPosition(i / 2, position);
  }
}

void Controll::requestUpdate() {
  bool success = false;
  while (!success) {
    sendPositions();
    success = readResponse();
  }
}
