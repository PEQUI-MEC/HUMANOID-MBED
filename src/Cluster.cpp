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
  this->data = &DataManager::getInstance();

  for (uint8_t i = 0; i < size; i++) {
    uint8_t id = this->servos[i].getId();
    printf("Enabling servo %d...\n", id);
    DigitalOut en(config::enablePin[id - 1], 0);
    Thread::wait(INIT_WAIT);
    printf("Servo %d enabled!\n", id);
  }
}

Cluster::~Cluster() {
  printf("WARN: Destructing Cluster...");
  this->thread.terminate();
  for (uint8_t i = 0; i < size; i++)
    DigitalOut en(config::enablePin[this->servos[i].getId() - 1], 0);
}

void Cluster::start() {
  if (CLUSTER_READ_ONLY)
    this->thread.start(callback(this, &Cluster::readPositions));
  else
    this->thread.start(callback(this, &Cluster::run));
}

void Cluster::run() {
  printf("Running cluster...\n");
  uint8_t i = 0;

  while(true) {
    for(i = 0; i < this->size; i++) {
      XYZrobotServo& servo = this->servos[i];

      uint8_t id = servo.getId();
      uint16_t pos = this->data->getDesiredPosition(id);

      servo.setPosition(pos, PLAYTIME);

      XYZrobotServoStatus status = servo.readStatus();
      if (!servo.getLastError()) {
        this->data->setRealPosition(id, status.position);
        this->data->setIBus(id, status.iBus);
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
