#include "Sinusoidal.h"

Sinusoidal::Sinusoidal(int id, PinName tx, PinName rx, int baud) {
  BufferSerial* serial = new BufferSerial(tx, rx, 32);
  this->id = id;
  this->servo = new XYZrobotServo(id, *serial, baud);
}

void Sinusoidal::run(int min, int max, float f, int playtime) {
  DigitalOut led1(LED1); // Marca se o comando de setar posição está sendo enviado
  DigitalOut led2(LED2); // Marca se está recebendo resposta do servo

  XYZrobotServo &servo = *this->servo;
  XYZrobotServoStatus status;

  Timer timer;
  timer.start();
  float t;

  int amplitude = (max - min)/2;
  int referencia = min + amplitude;
  int pos = min;

  // Variaveis para calcular a corrente rms
  int ultimo = timer.read_ms();
  float soma = 0;
  int qtdParcial = 0;
  int qtdTotal = 0;
  float Irms = 0;

  while(true) {
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
      qtdParcial++;
      soma += pow(status.iBus, 2);
      printf("$%d %d %d %d %d;\n", pos, status.position, status.iBus, (int)Irms, qtdTotal*20);
    }

    if ((timer.read_ms() - ultimo) >= 100) {
      qtdTotal = qtdParcial;
      printf("Calculando RMS de %d requisicoes...\n", qtdTotal);
      Irms = sqrt(soma/qtdParcial);
      soma = 0;
      qtdParcial = 0;
      ultimo = timer.read_ms();
    }
  }
}
