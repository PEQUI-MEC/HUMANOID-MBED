#include "Cluster.h"

Cluster::Cluster(PinName tx, PinName rx, uint8_t* ids, uint8_t length, uint32_t baud): serial(tx, rx, 32) {
  this->size = length;
  this->data = &DataController::getInstance();

  for (int i = 0; i < length; i++)
    this->servos.push_back(XYZrobotServo(ids[i], this->serial, baud));
}

void Cluster::start() {
  this->thread = new Thread();
  this->thread->start(callback(&Cluster::thread_starter, this));
}

void Cluster::run() {
  DigitalOut led(LED2);
  uint8_t i = 0;

  while(true) {
    for(i = 0; i < this->size; i++) {
      XYZrobotServo& servo = this->servos[i];

      uint8_t id = servo.getId();
      uint16_t pos = this->data->getTargetP(servo.getId());

      printf("Servo %d: Target %d\n", id, pos);
      servo.setPosition(pos, 0);

      XYZrobotServoStatus status = servo.readStatus();
      if (servo.getLastError()) {
        printf("Servo %d: Error %d\n", id, servo.getLastError());
      } else {
        this->data->setServoP(id, status.position);
        this->data->setServoI(id, status.iBus);
        printf("Servo %d: %d %d\n", id, status.position, status.iBus);
      }
    }

    // Dados para plotar o grafico
    printf("\n$%d ", this->data->getTargetP(this->servos[0].getId()));
    for(i = 0; i < this->size; i++) {
      uint8_t id = this->servos[i].getId();
      if (i == this->size-1)
        printf("%d %d;\n", this->data->getServoP(id), this->data->getServoI(id));
      else
        printf("%d %d ", this->data->getServoP(id), this->data->getServoI(id));
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
        printf("Servo %d: $%d %d;\n", servo.getId(), status.position, status.iBus);
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
