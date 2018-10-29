#include "Gimbal.h"

Gimbal::Gimbal()
    : servo_pitch(NUM_SERVOS - 1, PWM1, AD1),
      servo_yaw(NUM_SERVOS, PWM2, AD2),
      bno(BNO055_ADDRESS_B, SDA1, SCL1) {
  DigitalOut red(LED3, 1);
  if (bno.begin()) red = false;

  // Enable logic is inverted for PwmServos
  DigitalOut enr(config::enablePin[servo_pitch.getId() - 1], 1);
  wait_ms(INIT_WAIT);
  DigitalOut eny(config::enablePin[servo_yaw.getId() - 1], 1);
  wait_ms(INIT_WAIT);
}

Gimbal::~Gimbal() {
  printf("WARN: Destructing Gimbal...");
  thread.terminate();
  DigitalOut enr(config::enablePin[servo_pitch.getId()], 0);
  DigitalOut eny(config::enablePin[servo_yaw.getId()], 0);
}

void Gimbal::start(void) {
  thread.start(callback(this, &Gimbal::loop));
}

void Gimbal::loop(void) {
  DataManager& data = DataManager::getInstance();
  uint8_t pitch_id = servo_pitch.getId();
  uint8_t yaw_id = servo_yaw.getId();
  int16_t goal, pitch_pos, yaw_pos;

  int16_t servo_pos_yall = 0;
  int16_t servo_pos_pitch = 0;

  int16_t from;
  int16_t to;
  int16_t dir_angle;
  int16_t esq_angle;
  int16_t min_angle;

  while (true) {
    imu::Vector<3> euler = bno.getVector(BNO055::VECTOR_EULER);
    pitch_pos = (euler.y() + 180) * 10;
    yaw_pos = euler.x() * 10;

    data.setRealPosition(pitch_id, pitch_pos);
    data.setRealPosition(yaw_id, yaw_pos);

    // Computing pitch goal position on servo_pitch
    from = pitch_pos;
    to = data.getGoalPosition(pitch_id);

    if (from < 0) from += 3600;
    if (from > to) {
      esq_angle = from - to;
      dir_angle = 3600 - esq_angle;
    } else {
      dir_angle = to - from;
      esq_angle = 3600 - dir_angle;
    }

    min_angle = (esq_angle < dir_angle ? esq_angle : dir_angle);
    if (min_angle > 20) {
      if (dir_angle > esq_angle)
        goal = (servo_pos_pitch + 2 < 700) ? servo_pos_pitch + 2 : 700;
      else
        goal = (servo_pos_pitch - 2 > -300) ? servo_pos_pitch - 2 : -300;

      servo_pos_pitch = goal;
      servo_pitch.setPosition(goal);
    }

    // Computing yaw goal position on servo_yaw
    from = yaw_pos;
    to = data.getGoalPosition(yaw_id);

    if (from < 0) from += 3600;
    if (from > to) {
      esq_angle = from - to;
      dir_angle = 3600 - esq_angle;
    } else {
      dir_angle = to - from;
      esq_angle = 3600 - dir_angle;
    }

    min_angle = (esq_angle < dir_angle) ? esq_angle : dir_angle;
    if (min_angle > 20) {
      if (dir_angle > esq_angle)
        goal = (servo_pos_yall + 7 < 900) ? servo_pos_yall + 7 : 900;
      else
        goal = (servo_pos_yall - 7 > -900) ? servo_pos_yall - 7 : -900;

      servo_pos_yall = goal;
      servo_yaw.setPosition(goal);
    }

    wait_ms(10);
  }
}
