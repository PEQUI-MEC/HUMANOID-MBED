#include "Cluster.h"

#ifdef CFG_ROBOT_X

Cluster::Cluster(PinName tx, PinName rx, std::array<uint8_t, CLUSTER_SIZE> ids, uint32_t baud)
  : servos({{
        {1, this->serial, baud, true, 0},
        {2, this->serial, baud, false, 0},
        {3, this->serial, baud, true, 0},
        {4, this->serial, baud, true, 0},
        {5, this->serial, baud, true, 0},
        {6, this->serial, baud, false, 45},
        {7, this->serial, baud, true, 0},
        {8, this->serial, baud, true, 0},
        {9, this->serial, baud, false, 0},
        {10, this->serial, baud, false, 0},
        {11, this->serial, baud, true, 0},
        {12, this->serial, baud, false, -45}
    }}),
    serial(tx, rx, 32)
{
  serial.baud(baud);
  this->size = ids.size();
}

Cluster::~Cluster() {
  printf("WARN: Destructing Cluster...");
  this->thread.terminate();
  for (uint8_t i = 0; i < size; i++)
    this->servos[i].disable();
}

void Cluster::start() {
  if (CLUSTER_READ_ONLY)
    this->thread.start(callback(this, &Cluster::readPositions));
  else
    this->thread.start(callback(this, &Cluster::loop));
}

void Cluster::loop() {
  DataManager& data = DataManager::getInstance();
  uint8_t i = 0;

  while (true) {
    for (i = 0; i < this->size; i++) {
      uint8_t id = this->servos[i].getId();
      this->servos[i].setPosition(data.getGoalPosition(id));

      // XYZrobotServoStatus status = servo.readStatus();
      // if (!servo.getLastError()) {
      //   int16_t pos = range_map(status.position, 0, 1023, -1800, 1500);
      //   data.setRealPosition(id, pos);
      // }
    }
  }
}

void Cluster::readPositions() {
  uint8_t i = 0;

  while(true) {
    for(i = 0; i < this->size; i++) {
      SerialServo& servo = this->servos[i];

      int16_t pos = servo.readPosition();
      if (servo.getLastError())
        printf("Servo %d: Error %d\n", servo.getId(), servo.getLastError());
      else
        printf("Servo %d: %d\n", servo.getId(), pos);
    }
  }
}

#endif
