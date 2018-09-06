#pragma once

#include <stdint.h>

#define DEBUG true

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

#define NUM_SERVOS 18
#define CLUSTER_READ_ONLY false

// Protocol
#define P_HEADER 0xE8
#define P_SIZE ((NUM_SERVOS * 2) + 2)

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
