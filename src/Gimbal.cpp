#include "Gimbal.h"

Gimbal::Gimbal()
    : servo_pitch(NUM_SERVOS - 1, PWM1, AD1, 1, 2, 3, 4, 5),
      servo_yaw(NUM_SERVOS, PWM2, AD2, 1, 2, 3, 4, 5),
      bno(BNO055_ADDRESS_B) {
  // servo_pitch(NUM_SERVOS - 1, PWM1, AD1, f, dmin, dmax, omin, omax),
  // servo_yaw(NUM_SERVOS, PWM2, AD2, f, dmin, dmax, omin, omax) {
  DigitalOut red(LED3, 1);
  if (bno.begin()) red = false;

  imu::Vector<3> euler = bno.getVector(BNO055::VECTOR_EULER);
  yaw_offset = euler.x() * 10;

  DigitalOut enr(config::enablePin[servo_pitch.getId()], 0);
  wait_ms(INIT_WAIT);
  DigitalOut eny(config::enablePin[servo_yaw.getId()], 0);
  wait_ms(INIT_WAIT);
  printf("Gimbal enabled!\n");
}

Gimbal::~Gimbal() {
  printf("WARN: Destructing Gimbal...");
  thread.terminate();
  for (uint8_t i = NUM_SERVOS - 2; i < NUM_SERVOS; i++)
    DigitalOut en(config::enablePin[i], 1);
}

void Gimbal::start(void) {
  thread.start(callback(this, &Gimbal::loop));
}

void Gimbal::loop(void) {
  DataManager& data = DataManager::getInstance();
  uint8_t pitch_id = servo_pitch.getId();
  uint8_t yaw_id = servo_yaw.getId();
  int16_t goal, delta, pitch_pos, yaw_pos;

  while (true) {
    imu::Vector<3> euler = bno.getVector(BNO055::VECTOR_EULER);
    pitch_pos = euler.y() * 10;
    yaw_pos = euler.x() * 10;
    data.setRealPosition(pitch_id, pitch_pos);
    data.setRealPosition(yaw_id, yaw_pos);

    delta = data.getGoalPosition(pitch_id) - pitch_pos;
    if (abs(delta) > 2)
      goal = pitch_pos + (delta > 0 ? 2 : -2);
    else
      goal = pitch_pos + delta;
    servo_pitch.setPosition(goal);
    data.setRealPosition(1, goal);

    delta = data.getGoalPosition(yaw_id) - yaw_pos;
    if (abs(delta) > 2)
      goal = yaw_pos + (delta > 0 ? 2 : -2);
    else
      goal = yaw_pos + delta;
    servo_yaw.setPosition(goal);
    data.setRealPosition(2, goal);

    wait_ms(5);
  }
}
