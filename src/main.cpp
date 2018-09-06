#include "mbed.h"
#include "config.h"
#include "Cluster.h"
#include "DataController.h"

Serial pc(USBTX, USBRX, 115200); // Usando minicom, o maximo é 115200

int main() {
  DataController& data = DataController::getInstance();

  uint8_t ids[] = {1, 10, 14};
  Cluster c(SERIAL_TX7, SERIAL_RX7, ids, 3);
  c.start();

  // Executar movimento senoidal
  DigitalOut led(LED1);
  int i = 0;

  Timer timer;
  timer.start();
  float t;
  float f = 0.5;

  int min = 522;
  int max = 700;
  int amplitude = (max - min)/2;
  int referencia = min + amplitude;
  int pos = min;

  while(true) { // Sempre é necessário um loop no main
    t = timer.read();
    pos = ((amplitude * cos(2 * M_PI * f * t)) + referencia);

    if (pos >= min && pos <= max) {
      led = true;

      for (i = 0; i < 3; i++)
        data.setDesiredPosition(ids[i], pos);
    } else {
      led = false;
    }
  }
}
