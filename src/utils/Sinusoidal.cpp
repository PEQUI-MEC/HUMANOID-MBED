#include "Sinusoidal.h"

Sinusoidal::Sinusoidal(uint8_t id, PinName tx, PinName rx, uint32_t baud): serial(tx, rx, 32) {
  this->id = id;
  this->baud = baud;
  serial.baud(baud);
}

void Sinusoidal::run(uint16_t min, uint16_t max, float f, uint8_t playtime) {
  DigitalOut led1(LED1); // Marca se o comando de setar posição está sendo enviado
  DigitalOut led2(LED2); // Marca se está recebendo resposta do servo

  XYZrobotServo servo(id, serial, baud);
  XYZrobotServoStatus status;

  Timer timer;
  timer.start();
  float t;

  int amplitude = (max - min)/2;
  int referencia = min + amplitude;
  int pos = min;

  while(true) {
    // wait_ms(50);
    t = timer.read();
    pos = ((amplitude * cos(2 * M_PI * f * t)) + referencia);

    if (pos >= min && pos <= max) {
      led1 = true;
      servo.setPosition(pos, playtime);
    } else {
      led1 = false;
    }

    status = servo.readStatus();
    if (servo.getLastError()) {
      led2 = false;
      printf("Error: %d\n", servo.getLastError());
    } else {
      led2 = true;
      printf("$%d %d;\n", pos, status.position);
    }
  }
}
