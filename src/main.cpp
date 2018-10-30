#include "AsyncCommunication.h"
#include "Cluster.h"
#include "Communication.h"
#include "Gimbal.h"
#include "PwmCluster.h"
#include "config.h"
#include "mbed.h"

// TODO: Configurar tensão admisivel dos XYZ

Serial pc(USBTX, USBRX, COM_BAUD_RATE);

void disable_all_enable_pins();

int main() {
  disable_all_enable_pins();
  wait_ms(1000);

  /**
   * Initializing Gimbal
   **/
  Gimbal gimbal;
  gimbal.start();

  /**
   * Initializing Body Servos
   **/
#ifdef CFG_ROBOT_X
  Cluster c2(SERIAL_TX7, SERIAL_RX7, {1, 2, 3});
  c2.start();
#endif

#ifdef CFG_ROBOT_F
  PwmCluster cluster;
  cluster.start();
#endif

  /**
   * Initializing Communication
   **/
#ifdef CFG_COM_SYNC
  Communication com(USBTX, USBRX);
  com.start();
#endif

#ifdef CFG_COM_ASYNC
  AsyncCommunication acom(USBTX, USBRX);
  acom.start();
#endif

  /**
   * Main thread waits forever
   **/
  DigitalOut led1(LED1, 1);
  while (true) Thread::signal_wait(2);
}

void disable_all_enable_pins() {
  for (uint8_t i = 0; i < NUM_SERVOS; i++)
    DigitalOut en(config::enablePin[i], !ENABLE_ACTIVE);
}
