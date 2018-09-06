#include "../config.h"
#include "BufferSerial.h"
#include "XYZrobotServo.h"
#include "mbed.h"

class Sinusoidal {
public:
  Sinusoidal(int id = 1, PinName tx = SERIAL_TX7, PinName rx = SERIAL_RX7, int baud = 115200);
  void run(int min, int max, float f, int playtime = 0);

private:
  int id;
  XYZrobotServo* servo;
};
