/***************************************************************************
  This is a library for the BNO055 orientation sensor

  Designed specifically to work with the Adafruit BNO055 Breakout.

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products

  These sensors use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by KTOWN for Adafruit Industries.

  MIT license, all text above must be included in any redistribution
 ***************************************************************************/

#include <limits.h>
#include <math.h>
#include "mbed.h"

#include "BNO055.h"

BNO055::BNO055(uint8_t address, PinName sda, PinName scl) : i2c(sda, scl) {
  _address = address;
}

bool BNO055::begin(bno055_opmode_t mode) {
  /* Enable I2C */
  // i2c->frequency(10000);

  /* Make sure we have the right device */
  uint8_t id = read8(BNO055_CHIP_ID_ADDR);
  if (id != BNO055_ID) {
    wait_ms(1000);  // hold on for boot
    id = read8(BNO055_CHIP_ID_ADDR);
    if (id != BNO055_ID) return false;
  }

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);

  /* Reset */
  write8(BNO055_SYS_TRIGGER_ADDR, 0x20);
  wait_ms(50);
  while (read8(BNO055_CHIP_ID_ADDR) != BNO055_ID) wait_ms(10);
  wait_ms(50);

  /* Set to normal power mode */
  write8(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
  wait_ms(10);

  write8(BNO055_PAGE_ID_ADDR, 0);

  /* Set the output units */
  /*
  uint8_t unitsel = (0 << 7) | // Orientation = Android
                    (0 << 4) | // Temperature = Celsius
                    (0 << 2) | // Euler = Degrees
                    (1 << 1) | // Gyro = Rads
                    (0 << 0);  // Accelerometer = m/s^2
  write8(BNO055_UNIT_SEL_ADDR, unitsel);
  */

  write8(BNO055_SYS_TRIGGER_ADDR, 0x0);
  wait_ms(10);
  /* Set the requested operating mode (see section 3.3) */
  setMode(mode);
  wait_ms(20);

  return true;
}

void BNO055::setMode(bno055_opmode_t mode) {
  _mode = mode;
  write8(BNO055_OPR_MODE_ADDR, _mode);
  wait_ms(30);
}

void BNO055::setExtCrystalUse(bool usextal) {
  bno055_opmode_t modeback = _mode;

  /* Switch to config mode (just in case since this is the default) */
  setMode(OPERATION_MODE_CONFIG);
  wait_ms(25);
  write8(BNO055_PAGE_ID_ADDR, 0);

  if (usextal) write8(BNO055_SYS_TRIGGER_ADDR, 0x80);
  else write8(BNO055_SYS_TRIGGER_ADDR, 0x00);

  wait_ms(10);
  setMode(modeback);
  wait_ms(20);
}

void BNO055::getSystemStatus(uint8_t* system_status, uint8_t* self_test_result, uint8_t* system_error) {
  write8(BNO055_PAGE_ID_ADDR, 0);

  /* System Status (see section 4.3.58)
     ---------------------------------
     0 = Idle
     1 = System Error
     2 = Initializing Peripherals
     3 = System Iniitalization
     4 = Executing Self-Test
     5 = Sensor fusio algorithm running
     6 = System running without fusion algorithms */

  if (system_status != 0)
    *system_status = read8(BNO055_SYS_STAT_ADDR);

  /* Self Test Results (see section )
     --------------------------------
     1 = test passed, 0 = test failed

     Bit 0 = Accelerometer self test
     Bit 1 = Magnetometer self test
     Bit 2 = Gyroscope self test
     Bit 3 = MCU self test

     0x0F = all good! */

  if (self_test_result != 0)
    *self_test_result = read8(BNO055_SELFTEST_RESULT_ADDR);

  /* System Error (see section 4.3.59)
     ---------------------------------
     0 = No error
     1 = Peripheral initialization error
     2 = System initialization error
     3 = Self test result failed
     4 = Register map value out of range
     5 = Register map address out of range
     6 = Register map write error
     7 = BNO low power mode not available for selected operat ion mode
     8 = Accelerometer power mode not available
     9 = Fusion algorithm configuration error
     A = Sensor configuration error */

  if (system_error != 0)
    *system_error = read8(BNO055_SYS_ERR_ADDR);

  wait_ms(200);
}

void BNO055::getRevInfo(bno055_rev_info_t* info) {
  uint8_t a, b;

  memset(info, 0, sizeof(bno055_rev_info_t));

  info->accel_rev = read8(BNO055_ACCEL_REV_ID_ADDR);
  info->mag_rev = read8(BNO055_MAG_REV_ID_ADDR);
  info->gyro_rev = read8(BNO055_GYRO_REV_ID_ADDR);
  info->bl_rev = read8(BNO055_BL_REV_ID_ADDR);

  a = read8(BNO055_SW_REV_ID_LSB_ADDR);
  b = read8(BNO055_SW_REV_ID_MSB_ADDR);
  info->sw_rev = (((uint16_t)b) << 8) | ((uint16_t)a);
}

void BNO055::getCalibration(uint8_t* sys, uint8_t* gyro, uint8_t* accel, uint8_t* mag) {
  uint8_t calData = read8(BNO055_CALIB_STAT_ADDR);

  if (sys != NULL) *sys = (calData >> 6) & 0x03;
  if (gyro != NULL) *gyro = (calData >> 4) & 0x03;
  if (accel != NULL) *accel = (calData >> 2) & 0x03;
  if (mag != NULL) *mag = calData & 0x03;
}

int8_t BNO055::getTemp(void) {
  int8_t temp = (int8_t)(read8(BNO055_TEMP_ADDR));
  return temp;
}

imu::Vector<3> BNO055::getVector(vector_type_t vector_type) {
  imu::Vector<3> xyz;
  unsigned char buffer[6];
  memset(buffer, 0, 6);

  int16_t x, y, z;
  x = y = z = 0;

  readLen((bno055_reg_t)vector_type, (char*)buffer, 6);

  x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
  y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
  z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);

  switch (vector_type) {
    case VECTOR_MAGNETOMETER:
      /* 1uT = 16 LSB */
      xyz[0] = ((double)x) / 16.0;
      xyz[1] = ((double)y) / 16.0;
      xyz[2] = ((double)z) / 16.0;
      break;
    case VECTOR_GYROSCOPE:
      /* 1rps = 900 LSB */
      xyz[0] = ((double)x) / 900.0;
      xyz[1] = ((double)y) / 900.0;
      xyz[2] = ((double)z) / 900.0;
      break;
    case VECTOR_EULER:
      /* 1 degree = 16 LSB */
      xyz[0] = ((double)x) / 16.0;
      xyz[1] = ((double)y) / 16.0;
      xyz[2] = ((double)z) / 16.0;
      break;
    case VECTOR_ACCELEROMETER:
    case VECTOR_LINEARACCEL:
    case VECTOR_GRAVITY:
      /* 1m/s^2 = 100 LSB */
      xyz[0] = ((double)x) / 100.0;
      xyz[1] = ((double)y) / 100.0;
      xyz[2] = ((double)z) / 100.0;
      break;
  }

  return xyz;
}

imu::Quaternion BNO055::getQuat(void) {
  unsigned char buffer[8];
  memset(buffer, 0, 8);

  int x, y, z, w;
  x = y = z = w = 0;

  /* Read quat data (8 bytes) */
  readLen(BNO055_QUATERNION_DATA_W_LSB_ADDR, (char*)buffer, 8);
  w = (((uint16_t)buffer[1]) << 8) | ((uint16_t)buffer[0]);
  x = (((uint16_t)buffer[3]) << 8) | ((uint16_t)buffer[2]);
  y = (((uint16_t)buffer[5]) << 8) | ((uint16_t)buffer[4]);
  z = (((uint16_t)buffer[7]) << 8) | ((uint16_t)buffer[6]);

  const double scale = (1.0 / (1 << 14));
  imu::Quaternion quat(scale * w, scale * x, scale * y, scale * z);
  return quat;
}

bool BNO055::write8(bno055_reg_t reg, char value) {
  char cmd[2];
  cmd[0] = reg;
  cmd[1] = value;

  i2c.write(_address << 1, cmd, 1, true);
  return true;
}

char BNO055::read8(bno055_reg_t reg) {
  char reg_to_write = (char)reg;
  char to_read = 0;

  i2c.write(_address << 1, &reg_to_write, 1, true);
  i2c.read(_address << 1, &to_read, 1, false);

  return to_read;
}

bool BNO055::readLen(bno055_reg_t reg, char* buffer, int len) {
  char reg_to_write = (char)(reg);

  i2c.write(_address << 1, &reg_to_write, 1, true);
  i2c.read(_address << 1, buffer, len, false);

  return true;
}
