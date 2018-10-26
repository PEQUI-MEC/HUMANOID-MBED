#include "Cluster.h"

Cluster::Cluster(PinName tx, PinName rx, std::array<uint8_t, CLUSTER_SIZE> ids, uint32_t baud)
  : servos({{
        {{ids[0]}, this->serial, baud},
        {{ids[1]}, this->serial, baud},
        {{ids[2]}, this->serial, baud},
    }}),
    serial(tx, rx, 32)
{
  serial.baud(baud);
  this->size = ids.size();

  for (uint8_t i = 0; i < size; i++) {
    uint8_t id = this->servos[i].getId();
    printf("Enabling servo %d...\n", id);
    DigitalOut en(config::enablePin[id - 1], 0);
    wait_ms(INIT_WAIT);
    printf("Servo %d enabled!\n", id);
  }
}

Cluster::~Cluster() {
  printf("WARN: Destructing Cluster...");
  this->thread.terminate();
  for (uint8_t i = 0; i < size; i++)
    DigitalOut en(config::enablePin[this->servos[i].getId() - 1], 1);
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
      XYZrobotServo& servo = this->servos[i];

      uint8_t id = servo.getId();
      uint16_t goal = range_map(data.getGoalPosition(id), -18000, 18000, 0, 1023);

      servo.setPosition(goal, PLAYTIME);

      XYZrobotServoStatus status = servo.readStatus();
      if (!servo.getLastError()) {
        int16_t pos = range_map(status.position, 0, 1023, -1800, 1800);
        data.setRealPosition(id, pos);
      }
    }
  }
}

void Cluster::readPositions() {
  uint8_t i = 0;

  while(true) {
    for(i = 0; i < this->size; i++) {
      XYZrobotServo& servo = this->servos[i];

      XYZrobotServoStatus status = servo.readStatus();
      if (servo.getLastError())
        printf("Servo %d: Error %d\n", servo.getId(), servo.getLastError());
      else
        printf("Servo %d: %d %d\n", servo.getId(), status.position, status.iBus);
    }
  }
}
