#include "Cluster.h"

Cluster::Cluster(PinName tx, PinName rx, uint8_t* ids, uint8_t length, uint32_t baud): serial(tx, rx, 32) {
  this->size = length;
  this->data = &DataManager::getInstance();

  for (uint8_t i = 0; i < length; i++) {
    uint8_t id = ids[i];
    printf("Enabling servo %d...\n", id);
    DigitalOut en(config::enablePin[id - 1], 0);
    Thread::wait(INIT_WAIT);
    this->servos.push_back(XYZrobotServo(id, this->serial, baud));
    printf("Servo %d enabled!\n", id);
  }
}

void Cluster::start() {
  this->thread = new Thread();
  this->thread->start(callback(&Cluster::thread_starter, this));
}

void Cluster::run() {
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

void Cluster::thread_starter(void const *p) {
  Cluster* instance = (Cluster*)p;
  if (CLUSTER_READ_ONLY)
    instance->readPositions();
  else
    instance->run();
}
