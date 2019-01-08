#include "ServoDiagnostics.h"

#ifdef CFG_X

// TODO: Ler a posição de vários motores
// TODO: Controlar a posição do motor atraves da serial do pc

ServoDiagnostics::ServoDiagnostics(PinName tx, PinName rx, int baud) {
  this->tx = tx;
  this->rx = rx;
  this->baudRate = baud;

  this->serial = new BufferSerial(tx, rx, 32);
  this->serial->baud(baud);
}

void ServoDiagnostics::detectServo(int id) {
  XYZrobotServo servo(id, *this->serial, this->baudRate);
  servo.readStatus();

  if (servo.getLastError()) {
    if (servo.getLastError() == (uint8_t)XYZrobotServoError::HeaderTimeout) {
      printf("Timeout with id %d...\n", id);
    } else {
      printf("ID %d error: %d\n", id, servo.getLastError());
    }
    return;
  }

  printf("ID %d: Servo detected\n", id);

  // Make the servo's LED shine magenta for one second.
  // blinkServoLed(servo);
}

void ServoDiagnostics::detectServos(int rps) {
  printf("Detecting servos at %d baud...\n", this->baudRate);

  for (int id = 1 ; id <= 255 ; id++) {
    detectServo(id);
    wait_ms(1000/rps);
  }
}

void ServoDiagnostics::detectServos(int* ids, int length, int rps) {
  printf("Detecting servos at %d baud...\n", this->baudRate);

  for (int i = 0 ; i < length ; i++) {
    detectServo(ids[i]);
    wait_ms(1000/rps);
  }
}

void ServoDiagnostics::trackServoPosition(int id, int rps) {
  DigitalOut led1(LED1);
  XYZrobotServo servo(id, *this->serial, this->baudRate);
  XYZrobotServoStatus status;

  while(true) {
    wait_ms(1000/rps);

    status = servo.readStatus();
    if (servo.getLastError()) {
      led1 = 0;
      printf("Error: %d\n", servo.getLastError());
    } else {
      led1 = 1;
      printf("$%d;\n", status.position);
    }
  }
}

void ServoDiagnostics::setServoPosition(int id, int position, int playtime) {
  XYZrobotServo servo(id, *this->serial, this->baudRate);
  printf("Setting position to %d...\n", position);
  servo.setPosition(position, playtime);
}

void ServoDiagnostics::setServoPosition(
  int id,
  int* positions,
  int length,
  float interval,
  int playtime
) {
  XYZrobotServo servo(id, *this->serial, this->baudRate);

  for (int i = 0 ; i < length ; i++) {
    printf("Setting position to %d...\n", positions[i]);
    servo.setPosition(positions[i], playtime);
    wait_ms(interval);
  }
}

#endif
