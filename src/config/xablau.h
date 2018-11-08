#pragma once

#include <stdint.h>
#include "mbed.h"

/**
 * Pinout Definitions
 **/
#define SERIAL_TX2 PD_5
#define SERIAL_RX2 PD_6
#define SERIAL_TX3 PB_10  // STDIO
#define SERIAL_RX3 PB_11  // STDIO
#define SERIAL_TX4 PA_0
#define SERIAL_RX4 PC_11
#define SERIAL_TX5 PC_12
#define SERIAL_RX5 PD_2
#define SERIAL_TX6 PC_6
#define SERIAL_RX6 PC_7
#define SERIAL_TX7 PE_8
#define SERIAL_RX7 PE_7
#define ENABLE PG_0
#define PWM1 PF_6
#define PWM2 PA_15
#define AD1 PF_7
#define AD2 PC_2
#define SCL1 PB_8
#define SDA1 PB_9
#define VREF PA_5

/**
 * Servos Definitions
 **/
#define ENABLE_ACTIVE false
#define NUM_SERVOS 20 // 18 (Body) + 2 (Gimbal)
#define CLUSTER_SIZE 12
#define CLUSTER_READ_ONLY false
#define INIT_WAIT 1000
#define PLAYTIME 1

namespace config {
  const uint16_t initialPosition[NUM_SERVOS] = {
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  };
}
