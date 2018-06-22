#include "mbed.h"
#include "BufferSerial.h"
#include "XYZrobotServo.h"
#include "../SerialPins.h"

class Sinusoidal {
public:
  Sinusoidal(int id = 1, PinName tx = SerialPins::TX7, PinName rx = SerialPins::RX7, int baud = 115200);
  void run(int min, int max, float f, int playtime = 0);

private:
  int id;
  XYZrobotServo* servo;
};
