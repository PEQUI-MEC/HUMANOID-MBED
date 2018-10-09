#include "Cluster.h"
#include "Communication.h"
#include "config.h"
#include "mbed.h"

Serial pc(USBTX, USBRX, 115200);  // Usando minicom, o maximo é 115200
DigitalOut led1(LED1);

void disable_all_enable_pins();

int main() {
  printf("Initializing...\n");  // TO REMOVE

  disable_all_enable_pins();
  wait_ms(1000);

  /**
   * Initializing Clusters
   **/
  Cluster c2(SERIAL_TX7, SERIAL_RX7, {1, 2, 3});
  c2.start();

  /**
   * Initializing Communication
   **/
  Communication com(SERIAL_TX5, SERIAL_RX5, 115200);
  com.start();

  /**
   * Main thread waits forever
   **/
  while (true) {
    led1 = true;
    Thread::signal_wait(2);
  }
}

void disable_all_enable_pins() {
  for (uint8_t i = 0; i < 22; i++)
    DigitalOut en(config::enablePin[i], 1);
}
