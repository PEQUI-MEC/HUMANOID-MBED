#include "PwmServo.h"

PwmServo::PwmServo(uint8_t id, PinName pwm_pin, PinName fb_pin): pwm(pwm_pin), feedback(fb_pin) {
  this->id = id;
  pwm.period(PWM_PERIOD);
  pwm.write(0);
}

PwmServo::~PwmServo() {
  this->deactivate();
}

void PwmServo::activate() {
  if (goal != (int16_t)0x8000) setPosition(goal);
}

void PwmServo::deactivate() {
  pwm.write(0);
}

uint8_t PwmServo::getId() {
  return id;
}

void PwmServo::setPosition(int16_t pos) {
  goal = pos;
  float duty = range_map(pos, -900, 900, DUTY_MIN, DUTY_MAX);
  pwm.write(duty);
}

int16_t PwmServo::readPosition() {
  float read = feedback.read();
  return range_map(read, FEEDBACK_MIN, FEEDBACK_MAX, -900, 900);
}
