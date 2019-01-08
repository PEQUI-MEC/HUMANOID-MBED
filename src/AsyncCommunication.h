#pragma once

#include "config.h"

#ifdef CFG_COM_ASYNC

#include <array>
#include "DataManager.h"
#include "mbed.h"

class AsyncCommunication {
 public:
  AsyncCommunication(PinName tx, PinName rx, uint32_t baud = COM_BAUD_RATE);
  ~AsyncCommunication();
  void start(void);

  static const uint8_t SIGNAL_CONTINUE = 1;

 private:
  void loop(void);
  void signalPeriod(void);
  void sendMessage(void);
  void processMessage(void);

  void serialIn(void);
  bool storeData(uint8_t data);
  void moveMessage(void);
  void reset(void);

  void writeBytes(uint8_t* data, uint8_t size);
  uint8_t checksum(uint8_t* data, uint8_t dataSize);
  uint8_t checksum(std::array<uint8_t, COM_RECEIVE_SIZE> message);

  const static uint8_t bufferSize  = 50;
  std::array<uint8_t, bufferSize> buffer;
  std::array<uint8_t, COM_RECEIVE_SIZE> message;
  uint8_t bindex = 0;
  uint8_t lastData = 0;
  bool isStoring = false;
  bool messageReady = false;

  uint32_t baudRate;
  Thread thread;
  Ticker ticker;
  DigitalOut led;
  Serial serial;
  DataManager* data;
};

#endif
