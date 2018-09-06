#include "mbed.h"
#include "config.h"
#include "Cluster.h"

void startClusters();

void init() {
  if (DEBUG) printf("Initializing...\n");

  DataController::getInstance(); // First instance
  startClusters();

  if (DEBUG) printf("Ready!\n");
}

void startClusters() {
  uint8_t ids2[3] = {1, 2, 3};
  Cluster c2(SERIAL_TX2, SERIAL_RX2, ids2, 3);
  c2.start();
}
