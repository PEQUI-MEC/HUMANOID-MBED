#include "AsyncCommunication.h"

#ifdef CFG_COM_ASYNC

AsyncCommunication::AsyncCommunication(PinName tx, PinName rx, uint32_t baud): led(LED2, 0), serial(tx, rx, baud) {
  baudRate = baud;
  data = &DataManager::getInstance();
}

AsyncCommunication::~AsyncCommunication() {
  printf("WARN: Destructing AsyncCommunication...");
  led = false;
  thread.terminate();
}

void AsyncCommunication::start() {
  led = true;
  thread.start(callback(this, &AsyncCommunication::loop));
  serial.attach(callback(this, &AsyncCommunication::serialIn));
  wait_ms(100);
  ticker.attach(callback(this, &AsyncCommunication::signalPeriod), (float)COM_PERIOD/1000);
}

void AsyncCommunication::signalPeriod() {
  thread.signal_set(AsyncCommunication::SIGNAL_CONTINUE);
}

void AsyncCommunication::loop() {
  while (true) {
    Thread::signal_wait(AsyncCommunication::SIGNAL_CONTINUE);
    Thread::signal_clr(AsyncCommunication::SIGNAL_CONTINUE);
    sendMessage();
    processMessage();
    // led = !led;
  }
}

void AsyncCommunication::serialIn() {
  uint8_t data = serial.getc();
  if (data == COM_HEADER_BYTE && lastData == COM_HEADER_BYTE)
    isStoring = true;

  if (data == COM_FOOTER_BYTE && lastData == COM_FOOTER_BYTE && isStoring) {
    storeData(data);
    moveMessage();
    reset();
  }

  if (isStoring) {
    bool success = storeData(data);
    if (!success) reset();
  }

  lastData = data;
}

bool AsyncCommunication::storeData(uint8_t data) {
  if (bindex >= bufferSize)
    return false;

  buffer[bindex] = data;
  bindex++;
  return true;
}

void AsyncCommunication::moveMessage() {
  if (bindex != COM_RECEIVE_SIZE - 1)
    return;

  message[0] = 0xF1;
  for (uint8_t i = 0; i < bindex; i++)
    message[i + 1] = buffer[i];

  messageReady = true;
}

void AsyncCommunication::reset() {
  isStoring = false;
  bindex = 0;
}

void AsyncCommunication::sendMessage() {
  uint8_t header[COM_HEADER_SIZE];
  header[0] = COM_HEADER_BYTE;
  header[1] = COM_HEADER_BYTE;

  uint8_t data[COM_TRANSMIT_DATA_SIZE];

  int16_t pos = this->data->getRealPosition(NUM_SERVOS - 1);
  data[0] = (pos >> 8) & 0xFF;
  data[1] = pos & 0xFF;

  pos = this->data->getRealPosition(NUM_SERVOS);
  data[2] = (pos >> 8) & 0xFF;
  data[3] = pos & 0xFF;

  pos = this->data->getPitchServoPosition();
  data[4] = (pos >> 8) & 0xFF;
  data[5] = pos & 0xFF;

  pos = this->data->getYawServoPosition();
  data[6] = (pos >> 8) & 0xFF;
  data[7] = pos & 0xFF;

  data[COM_TRANSMIT_DATA_SIZE - 1] = this->data->getVref();

  header[2] = checksum(data, COM_TRANSMIT_DATA_SIZE);

  writeBytes(header, COM_HEADER_SIZE);
  writeBytes(data, COM_TRANSMIT_DATA_SIZE);
  serial.putc(COM_FOOTER_BYTE);
  serial.putc(COM_FOOTER_BYTE);
}

void AsyncCommunication::processMessage() {
  if (!messageReady) return;
  if (message[2] != checksum(message)) return;

  led = !led;

  for (uint8_t i = 0; i < COM_RECEIVE_DATA_SIZE; i++) {
    if (i % 2) continue;
    int16_t p = (int16_t)(message[i + COM_HEADER_SIZE] << 8);
    p = p + (int16_t)(message[i + COM_HEADER_SIZE + 1]);
    data->setGoalPosition((i / 2) + 1, p);
  }

  messageReady = false;
}

void AsyncCommunication::writeBytes(uint8_t* data, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    serial.putc(data[i]);
  }
}

uint8_t AsyncCommunication::checksum(uint8_t* data, uint8_t dataSize) {
  uint8_t checksum = data[0];
  for (uint8_t i = 1; i < dataSize; i++) checksum ^= data[i];
  return checksum;
}

uint8_t AsyncCommunication::checksum(std::array<uint8_t, COM_RECEIVE_SIZE> message) {
  uint8_t checksum = message[COM_HEADER_SIZE];
  for (uint8_t i = 1; i < COM_RECEIVE_DATA_SIZE; i++)
    checksum ^= message[COM_HEADER_SIZE + i];
  return checksum;
}

#endif
