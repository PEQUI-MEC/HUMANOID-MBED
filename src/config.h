#pragma once

#include <stdint.h>
#include "mbed.h"

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
#define EN1 PE_5
#define EN2 PF_8
#define EN3 PF_9
#define EN4 PE_6
#define EN5 PE_13
#define EN6 PE_14
#define EN7 PE_12
#define EN8 PE_10
#define EN9 PD_12
#define EN10 PD_13
#define EN11 PA_3
#define EN12 PB_3
#define EN13 PA_8
#define EN14 PB_10
#define EN15 PB_4
#define EN16 PB_5
#define EN17 PD_14
#define EN18 PD_15
#define EN19 PE_9
#define EN20 PE_11
#define EN21 PA_13
#define EN22 PC_3
#define PWM1 PF_6
#define PWM2 PA_15
#define AD1 PF_7
#define AD2 PC_2
#define SCL1 PB_8
#define SDA1 PB_9
#define VREF PA_5

#define NUM_SERVOS 20 // 18 (Body) + 2 (Gimbal)
#define CLUSTER_SIZE 3
#define CLUSTER_READ_ONLY false
#define INIT_WAIT 1000
#define PLAYTIME 0

// Communication
#define PERIOD 1000 // ms
#define P_HEADER_BYTE 0xE8
#define P_HEADER_SIZE 4 // HEADERB CMDID SIZE CHECKSUM
#define P_ERROR_ID 0x00
#define P_ERROR_SIZE P_HEADER_SIZE
#define P_UPDATE_ID 0x01
#define P_UPDATE_SIZE_SEND (P_HEADER_SIZE + 1 + (NUM_SERVOS * 2))
#define P_UPDATE_SIZE_RECEIVE (P_HEADER_SIZE + (NUM_SERVOS * 2))

namespace config {
  const uint16_t initialPosition[NUM_SERVOS] = {
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  };

  const PinName enablePin[22] = {
    EN1, EN2, EN3,
    EN4, EN5, EN6,
    EN7, EN8, EN9,
    EN10, EN11, EN12,
    EN13, EN14, EN15,
    EN16, EN17, EN18,
    EN19, EN20, EN21, EN22
  };
}
