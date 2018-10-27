#include "Cluster.h"
#include "Communication.h"
#include "config.h"
#include "mbed.h"
#include "Gimbal.h"
#include "BNO055.h"

Serial pc(USBTX, USBRX, 256000);
DigitalOut led1(LED1);

void disable_all_enable_pins();

#ifdef NONE
int main() {
  BNO055 bno(BNO055_ADDRESS_B, I2C_SDA, I2C_SCL);

  pc.printf("Orientation Sensor Raw Data Test\n");

  if (!bno.begin(BNO055::OPERATION_MODE_IMUPLUS)) {
    pc.printf("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!\n");
    while (true);
  } else
    pc.printf("BNO055 was detected!\n");

  wait(1);

  int8_t temp = bno.getTemp();
  pc.printf("Current Temperature: %d C\n", temp);
  // bno.setExtCrystalUse(true);

  pc.printf("Calibration status values: 0=uncalibrated, 3=fully calibrated\n");

  while (true) {
    imu::Vector<3> euler = bno.getVector(BNO055::VECTOR_GRAVITY);
    pc.printf("X: %f Y: %f Z: %f\t\t", euler.x(), euler.y(), euler.z());

    uint8_t calib_sys, calib_gyro, calib_accel, calib_mag = 0;
    bno.getCalibration(&calib_sys, &calib_gyro, &calib_accel, &calib_mag);
    pc.printf("CALIBRATION: Sys=%d, Gyro=%d, Accel=%d, Mag=%d\n", calib_sys, calib_gyro, calib_accel, calib_mag);
    wait_ms(100);
  }
}
#endif

#ifndef NONE
int main() {
  // printf("Initializing...\n");  // TO REMOVE

  disable_all_enable_pins();
  wait_ms(1000);

  /**
   * Initializing Clusters
   **/
  // Cluster c2(SERIAL_TX7, SERIAL_RX7, {1, 2, 3});
  // c2.start();

  Gimbal gimbal;
  gimbal.start();

  /**
   * Initializing Communication
   **/
  Communication com(USBTX, USBRX, 256000);
  com.start();

  /**
   * Main thread waits forever
   **/
  while (true) {
    led1 = true;
    Thread::signal_wait(2);
  }
}
#endif

void disable_all_enable_pins() {
  for (uint8_t i = 0; i < 22; i++)
    DigitalOut en(config::enablePin[i], 1);
}
