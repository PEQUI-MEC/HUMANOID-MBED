#include "Gimbal.h"

Gimbal::Gimbal()
    : servo_pitch(NUM_SERVOS - 1, PWM1, AD1, 0.01216, 0.045, 0.195, 0.1, 0.7),
      servo_yaw(NUM_SERVOS, PWM2, AD2, 0.01216, 0.045, 0.195, 0.1, 0.7),
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
  // printf("Gimbal enabled!\n"); // TO REMOVE
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
  servo_pitch.setPosition(0);
  servo_yaw.setPosition(0);

  int servo_pos_yall = 0;
  int servo_pos_pitch = 0;

  int t = 0;
  while (true) {
    imu::Vector<3> euler = bno.getVector(BNO055::VECTOR_EULER);
    pitch_pos = (euler.y()+180) * 10;
    yaw_pos = euler.x() * 10;
    data.setRealPosition(pitch_id, pitch_pos);
    data.setRealPosition(yaw_id, yaw_pos);
    
    int y = data.getGoalPosition(pitch_id);
    int x = (pitch_pos);
    if (x < 0)
        x += 3600;

    int dir_angle;
    int esq_angle;
    int min_angle;

    
    if (x > y) {
      esq_angle = x - y;
      dir_angle = 3600 - esq_angle;
    } else {
      dir_angle = y - x;
      esq_angle = 3600 - dir_angle;
    }
    min_angle = (esq_angle < dir_angle)?esq_angle:dir_angle;
    if (min_angle > 50) {
      if (dir_angle > esq_angle) {
        goal = (servo_pos_pitch + 2<700)?servo_pos_pitch + 2:700;
      }else{
        goal = (servo_pos_pitch - 2>0)?servo_pos_pitch - 2:0;
      }
      servo_pos_pitch = goal;
      servo_pitch.setPosition(goal);
    }
    
    data.setRealPosition(3, goal);
    //data.setRealPosition(3, data.getGoalPosition(pitch_id));

    y = data.getGoalPosition(yaw_id);
    x = (yaw_pos);
    if (x < 0)
        x += 3600;


    if (x > y) {
      esq_angle = x - y;
      dir_angle = 3600 - esq_angle;
    } else {
      dir_angle = y - x;
      esq_angle = 3600 - dir_angle;
    }
    min_angle = (esq_angle < dir_angle)?esq_angle:dir_angle;
    if (min_angle > 50) {
      if (dir_angle > esq_angle) {
        goal = (servo_pos_yall + 10<900)?servo_pos_yall + 10:900;
      }else{
        goal = (servo_pos_yall - 10>-900)?servo_pos_yall - 10:-900;
      }
      data.setRealPosition(1, esq_angle);
      data.setRealPosition(2, dir_angle);
      servo_pos_yall = goal;
      servo_yaw.setPosition(goal);
    }
    data.setRealPosition(4, goal);
    //data.setRealPosition(4, data.getGoalPosition(yaw_id));

    wait_ms(20);
  }
}
