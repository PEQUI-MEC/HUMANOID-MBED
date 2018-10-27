#include "Communication.h"

Communication::Communication(PinName tx, PinName rx, uint32_t baud): led(LED2, 0) , serial(tx, rx, serialBufferSize) {
  baudRate = baud;
  serial.baud(baud);
  data = &DataManager::getInstance();
  count = 0; // TO REMOVE
}

Communication::~Communication() {
  printf("WARN: Destructing Communication...");
  led = false;
  timer.stop(); // TO REMOVE
  thread.terminate();
}

void Communication::start() {
  led = true;
  thread.start(callback(this, &Communication::loop));
  wait_ms(50);
  signalPeriod();
  timer.start(); // TO REMOVE
}

void Communication::loop() {
  uint8_t status;

  while (true) {
    Thread::signal_wait(Communication::SIGNAL_CONTINUE);
    Thread::signal_clr(Communication::SIGNAL_CONTINUE);
    led = !led;

    status = Communication::STATUS_PENDING;
    serial.flush();
    sendUpdate();

    while (status != Communication::STATUS_DONE && status != Communication::STATUS_TIMEOUT) {
      status = readGoal();

      switch (status) {
        case Communication::STATUS_ERROR:
          wait_ms(1);
          serial.flush();
          sendUpdate();
          break;

        // case Communication::STATUS_TIMEOUT:
        case Communication::STATUS_DATA_TIMEOUT:
          wait_ms(1);
          serial.flush();
          sendTimeout();
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
  header[1] = P_HEADER_BYTE;
  header[2] = P_UPDATE_ID;
  header[3] = P_HEADER_SIZE + P_UPDATE_DATA_SIZE;

  uint8_t data[P_UPDATE_DATA_SIZE];
  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    uint16_t pos = this->data->getRealPosition(i+1);
    data[i * 2] = (pos >> 8) & 0xFF;
    data[(i * 2) + 1] = pos & 0xFF;
  }

  data[P_UPDATE_DATA_SIZE - 1] = this->data->getVref();

  header[4] = checksum(header[2], header[3], data, P_UPDATE_DATA_SIZE);

  writeBytes(header, P_HEADER_SIZE);
  writeBytes(data, P_UPDATE_DATA_SIZE);
}

void Communication::sendError() {
  uint8_t header[P_HEADER_SIZE];
  header[0] = P_HEADER_BYTE;
  header[1] = P_HEADER_BYTE;
  header[2] = P_ERROR_ID;
  header[3] = P_HEADER_SIZE;
  header[4] = P_ERROR_ID ^ P_HEADER_SIZE;
  writeBytes(header, P_HEADER_SIZE);
}

void Communication::sendTimeout() {
  uint8_t header[P_HEADER_SIZE];
  header[0] = P_HEADER_BYTE;
  header[1] = P_HEADER_BYTE;
  header[2] = P_TIMEOUT_ID;
  header[3] = P_HEADER_SIZE;
  header[4] = P_TIMEOUT_ID ^ P_HEADER_SIZE;
  writeBytes(header, P_HEADER_SIZE);
}

uint8_t Communication::readGoal() {
  bool timedout;
  uint8_t header[P_HEADER_SIZE];
  timedout = !readBytes(header, P_HEADER_SIZE, PERIOD - 15);

  if (timedout) return Communication::STATUS_TIMEOUT;
  if (header[0] != P_HEADER_BYTE || header[1] != P_HEADER_BYTE) return Communication::STATUS_INVALID;
  if (header[2] == P_ERROR_ID) return Communication::STATUS_ERROR;
  if (header[2] != P_GOAL_ID) return Communication::STATUS_INVALID;
  if (header[3] != P_HEADER_SIZE + P_GOAL_DATA_SIZE) return Communication::STATUS_INVALID;

  uint8_t data[P_GOAL_DATA_SIZE];
  timedout = !readBytes(data, P_GOAL_DATA_SIZE, 14);

  if (timedout) return Communication::STATUS_DATA_TIMEOUT;
  if (header[4] != checksum(header[2], header[3], data, P_GOAL_DATA_SIZE))
    return Communication::STATUS_INVALID;

  // TO REMOVE
  if (timer.read_ms() >= 1000) {
    // this->data->setRealPosition(1, count);
    timer.reset();
    count = 0;
  }
  count++;
  // TO REMOVE

  updateGoals(data, P_GOAL_DATA_SIZE);
  return Communication::STATUS_DONE;
}

void Communication::updateGoals(uint8_t* pos, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    if (i % 2) continue;
    int16_t p = (int16_t)(pos[i] << 8);
    p = p + (int16_t)(pos[i+1]);
    data->setGoalPosition((i / 2) + 1, p);
  }
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
