#include "../config.h"
#include "BufferSerial.h"
#include "XYZrobotServo.h"
#include "mbed.h"

class ServoDiagnostics {
public:
  ServoDiagnostics(PinName tx = SERIAL_TX7, PinName rx = SERIAL_RX7, int baud = 115200);

  void detectServo(int id);
  void detectServos(int rps = 2);
  void detectServos(int* ids, int length, int rps = 2);

  void trackServoPosition(int id, int rate = 2);
  void setServoPosition(int id, int position, int playtime = 0);
  void setServoPosition(int id, int* positions, int length, float interval = 1, int playtime = 0);

private:
  int baudRate;
  PinName tx;
  PinName rx;
  BufferSerial* serial;
};
