#include "mbed.h"
#include "utils/range_map.h"

class PwmServo {
 public:
  PwmServo(uint8_t id, PinName pwm_pin, PinName fb_pin, float f, float duty_min,
           float duty_max, float fb_min, float fb_max);
  void activate(void);
  void deactivate(void);

  uint8_t getId();
  void setPosition(int16_t pos);
  int16_t readPosition(void);

 private:
  uint8_t id;
  float frequency;
  float duty_min;
  float duty_max;
  float fb_min;
  float fb_max;
  int16_t goal = -1;

  PwmOut pwm;
  AnalogIn feedback;
};
