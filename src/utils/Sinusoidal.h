#include "../config.h"
#include "BufferSerial.h"
#include "XYZrobotServo.h"
#include "mbed.h"

class Sinusoidal {
public:
  Sinusoidal(uint8_t id = 1, PinName tx = SERIAL_TX7, PinName rx = SERIAL_RX7, uint32_t baud = 115200);
  void run(uint16_t min, uint16_t max, float f, uint8_t playtime = 0);

 private:
  uint8_t id;
  uint32_t baud;
  BufferSerial serial;
};
