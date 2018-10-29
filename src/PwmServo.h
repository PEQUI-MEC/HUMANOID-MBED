#pragma once

#include "mbed.h"
#include "utils/range_map.h"

// TODO: Medir o range de saida dos servos pwm

#define PWM_PERIOD 0.01216
#define DUTY_MIN 0.045
#define DUTY_MAX 0.195
#define FEEDBACK_MIN 0
#define FEEDBACK_MAX 1

class PwmServo {
 public:
  PwmServo(uint8_t id, PinName pwm_pin, PinName fb_pin);
  ~PwmServo();
  void activate(void);
  void deactivate(void);

  uint8_t getId();
  void setPosition(int16_t pos);
  int16_t readPosition(void);

 private:
  uint8_t id;
  int16_t goal = (int16_t)0x8000;

  PwmOut pwm;
  AnalogIn feedback;
};
