#include "Communication.h"

Communication::Communication(PinName tx, PinName rx, uint32_t baud): serial(tx, rx, serialBufferSize) {
  baudRate = baud;
  serial.baud(baud);
  data = &DataManager::getInstance();
}

Communication::~Communication() {
  printf("WARN: Destructing Communication...");
  thread.terminate();
}

void Communication::start() {
  thread.start(callback(this, &Communication::loop));
  wait_ms(50);
  signalPeriod();
}

void Communication::loop() {
  uint8_t status;

  while (true) {
    Thread::signal_wait(Communication::SIGNAL_CONTINUE);
    Thread::signal_clr(Communication::SIGNAL_CONTINUE);

    status = Communication::STATUS_PENDING;
    serial.flush();
    sendUpdate();

    while (true) {
      status = readGoal();

      switch (status) {
        case Communication::STATUS_ERROR:
          wait_ms(1);
          serial.flush();
          sendUpdate();
          break;

        case Communication::STATUS_INVALID:
          wait_ms(1);
          serial.flush();
          sendError();
          break;
      }
    }
  }
}

void Communication::signalPeriod() {
  timeout.attach(callback(this, &Communication::signalPeriod), (float)PERIOD / 1000.0);
  thread.signal_set(Communication::SIGNAL_CONTINUE);
}

void Communication::sendUpdate() {
  uint8_t header[P_HEADER_SIZE];
  header[0] = P_HEADER_BYTE;
  header[1] = P_UPDATE_ID;
  header[2] = P_UPDATE_SIZE_SEND;

  const uint8_t dataSize = P_UPDATE_SIZE_SEND - P_HEADER_SIZE;
  uint8_t data[P_UPDATE_SIZE_SEND - P_HEADER_SIZE];
  for (uint8_t i = 0; i < NUM_SERVOS * 2; i += 2) {
    uint16_t pos = this->data->getRealPosition((i/2) + 1);
    data[i] = (pos >> 8) & 0xFF;
    data[i+1] = pos & 0xFF;
  }

  data[dataSize - 1] = this->data->getVref();

  header[3] = checksum(header[1], header[2], data, dataSize);

  writeBytes(header, P_HEADER_SIZE);
  writeBytes(data, dataSize);
}

bool Communication::readGoal() {
  bool timedout;
  uint8_t header[P_HEADER_SIZE];
  timedout = readBytes(header, P_HEADER_SIZE);

  if (timedout) return Communication::STATUS_TIMEOUT;
  if (header[0] != P_HEADER_BYTE) return Communication::STATUS_INVALID;
  if (header[1] == P_ERROR_ID) return Communication::STATUS_ERROR;
  if (header[1] != P_GOAL_ID) return Communication::STATUS_INVALID;
  if (header[2] != P_GOAL_SIZE) return Communication::STATUS_INVALID;

  const uint8_t dataSize = P_GOAL_SIZE - P_HEADER_SIZE;
  uint8_t data[P_GOAL_SIZE - P_HEADER_SIZE];
  timedout = readBytes(data, dataSize, 3);

  if (timedout) return Communication::STATUS_INVALID;
  if (header[3] != checksum(header[1], header[2], data, dataSize))
    return Communication::STATUS_INVALID;

  updateGimbal(data[dataSize - 2], data[dataSize - 1]);
  updateBody((int16_t*)data, NUM_SERVOS);
  return Communication::STATUS_DONE;
}

void Communication::updateGimbal(int8_t pitch, int8_t yaw) {
  data->setGimbalPitch(pitch);
  data->setGimbalYaw(yaw);
}

void Communication::updateBody(int16_t* pos, uint8_t size) {
  for (uint8_t i = 0; i < size; i++)
    data->setDesiredPosition(i, pos[i]);
}

bool Communication::readBytes(uint8_t* data, uint8_t size, uint16_t timeout) {
  uint8_t byte = 0;
  Timer timer;
  if (timeout > 0) timer.start();

  while (byte < size && (timeout <= 0 || timer.read_ms() < timeout)) {
    if (serial.readable()) {
      data[byte] = (uint8_t)serial.getc();
      byte++;
    } else {
      wait(1.0 / (double)baudRate);
    }
  }

  return (byte == size);
}

void Communication::writeBytes(uint8_t* data, uint8_t size) {
  for (uint8_t i = 0; i < size; i++)
    serial.putc(data[i]);
}

uint8_t Communication::checksum(uint8_t cmd, uint8_t size, uint8_t* data, uint8_t dataSize) {
  uint8_t checksum = cmd ^ size;
  for (uint8_t i = 0; i < dataSize; i++) checksum ^= data[i];
  return checksum;
}
