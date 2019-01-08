#pragma once

#include <stdint.h>
#include "mbed.h"

/**
 * Pinout Definitions
 **/
#define EN1     PC_14
#define EN2     PC_15
#define EN3     PH_0
#define EN4     PH_1
#define EN5     PD_3
#define EN6     PG_2
#define EN7     PG_3
#define EN8     PE_2
#define EN9     PE_4
#define EN10    PF_2
#define EN11    PG_1
#define EN12    PG_15
#define PWM1    PA_15
#define PWM2    PE_5
#define PWM3    PE_6
#define PWM4    PB_6
#define PWM5    PC_7
#define PWM6    PA_8
#define PWM7    PB_10
#define PWM8    PB_4
#define PWM9    PB_5
#define PWM10   PB_3
#define PWM11   PD_13
#define PWM12   PD_12
#define AD1     PF_6
#define AD2     PF_7
#define AD3     PC_2
#define AD4     PC_3
#define AD5     PA_0
#define AD6     PA_4
#define AD7     PC_0
#define AD8     PF_8
#define AD9     PF_9
#define AD10    PA_6
#define AD11    PA_7
#define AD12    PA_3
#define SCL1    PB_8
#define SDA1    PB_9
#define VREF    PA_5

/**
 * Servos Definitions
 **/
#define ENABLE_ACTIVE true
#define NUM_SERVOS 12 // 10 (Body) + 2 (Gimbal)
#define INIT_WAIT 1000

namespace config {
  const uint16_t initialPosition[NUM_SERVOS] = {
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  };

  const PinName enablePin[12] = {
    EN1, EN2, EN3,
    EN4, EN5, EN6,
    EN7, EN8, EN9,
    EN10, EN11, EN12
  };
}
