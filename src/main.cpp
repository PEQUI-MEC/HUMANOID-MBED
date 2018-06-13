#include "mbed.h"
#include "SerialPins.h"
#include "utils/ServoDiagnostics.h"
#include "utils/Sinusoidal.h"

DigitalOut led1(LED1);
Serial pc(USBTX, USBRX);

int main() {
  pc.baud(115200); // Usando minicom, o maximo é 115200
  printf("Initializing...\n");

  Sinusoidal s(13);
  s.run(400, 800, 0.5);

  while (true) {
    led1 = !led1;
    wait(0.5);
  }
}
