#pragma once

#include "mbed.h"
#include "config.h"
#include "Cluster.h"

void startClusters();

void init() {
  // Disable all enable pins
  for (uint8_t i = 0; i < 22; i++)
    DigitalOut en(config::enablePin[i], 1);

  Thread::wait(1000);

  DataController::getInstance(); // First instance
  startClusters();
}

void startClusters() {
  uint8_t ids2[3] = {1, 2, 3};
  Cluster c2(SERIAL_TX2, SERIAL_RX2, ids2, 3);
  c2.start();
}
