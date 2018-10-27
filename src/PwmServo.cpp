#include "PwmServo.h"

PwmServo::PwmServo(uint8_t id, PinName pwm_pin, PinName fb_pin, float period,
                   float duty_min, float duty_max, float fb_min, float fb_max)
    : pwm(pwm_pin), feedback(fb_pin) {
  this->id = id;
  this->period = period;
  this->duty_min = duty_min;
  this->duty_max = duty_max;
  this->fb_min = fb_min;
  this->fb_max = fb_max;

  pwm.period(period);
  pwm.write(0);
}

void PwmServo::activate() {
  if (goal != -1) setPosition(goal);
}

void PwmServo::deactivate() {
  pwm.write(0);
}

uint8_t PwmServo::getId() {
  return id;
}

void PwmServo::setPosition(int16_t pos) {
  goal = pos;
  float duty = range_map(pos, -900, 900, duty_min, duty_max);
  pwm.write(duty);
}

int16_t PwmServo::readPosition() {
  float read = feedback.read();
  return range_map(read, fb_min, fb_max, -900, 900);
}
