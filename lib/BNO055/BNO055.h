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

#ifndef __ADAFRUIT_BNO055_H__
#define __ADAFRUIT_BNO055_H__

#include "imumaths.h"
#include "mbed.h"

#define BNO055_ADDRESS_A (0x28)
#define BNO055_ADDRESS_B (0x29)
#define BNO055_ID (0xA0)

class BNO055 {
 public:
  typedef enum {
    /* Page id register definition */
    BNO055_PAGE_ID_ADDR = 0x07,

    /* PAGE0 REGISTER DEFINITION START*/
    BNO055_CHIP_ID_ADDR = 0x00,
    BNO055_ACCEL_REV_ID_ADDR = 0x01,
    BNO055_MAG_REV_ID_ADDR = 0x02,
    BNO055_GYRO_REV_ID_ADDR = 0x03,
    BNO055_SW_REV_ID_LSB_ADDR = 0x04,
    BNO055_SW_REV_ID_MSB_ADDR = 0x05,
    BNO055_BL_REV_ID_ADDR = 0x06,

    /* Accel data register */
    BNO055_ACCEL_DATA_X_LSB_ADDR = 0x08,
    BNO055_ACCEL_DATA_X_MSB_ADDR = 0x09,
    BNO055_ACCEL_DATA_Y_LSB_ADDR = 0x0A,
    BNO055_ACCEL_DATA_Y_MSB_ADDR = 0x0B,
    BNO055_ACCEL_DATA_Z_LSB_ADDR = 0x0C,
    BNO055_ACCEL_DATA_Z_MSB_ADDR = 0x0D,

    /* Mag data register */
    BNO055_MAG_DATA_X_LSB_ADDR = 0x0E,
    BNO055_MAG_DATA_X_MSB_ADDR = 0x0F,
    BNO055_MAG_DATA_Y_LSB_ADDR = 0x10,
    BNO055_MAG_DATA_Y_MSB_ADDR = 0x11,
    BNO055_MAG_DATA_Z_LSB_ADDR = 0x12,
    BNO055_MAG_DATA_Z_MSB_ADDR = 0x13,

    /* Gyro data registers */
    BNO055_GYRO_DATA_X_LSB_ADDR = 0x14,
    BNO055_GYRO_DATA_X_MSB_ADDR = 0x15,
    BNO055_GYRO_DATA_Y_LSB_ADDR = 0x16,
    BNO055_GYRO_DATA_Y_MSB_ADDR = 0x17,
    BNO055_GYRO_DATA_Z_LSB_ADDR = 0x18,
    BNO055_GYRO_DATA_Z_MSB_ADDR = 0x19,

    /* Euler data registers */
    BNO055_EULER_H_LSB_ADDR = 0x1A,
    BNO055_EULER_H_MSB_ADDR = 0x1B,
    BNO055_EULER_R_LSB_ADDR = 0x1C,
    BNO055_EULER_R_MSB_ADDR = 0x1D,
    BNO055_EULER_P_LSB_ADDR = 0x1E,
    BNO055_EULER_P_MSB_ADDR = 0x1F,

    /* Quaternion data registers */
    BNO055_QUATERNION_DATA_W_LSB_ADDR = 0x20,
    BNO055_QUATERNION_DATA_W_MSB_ADDR = 0x21,
    BNO055_QUATERNION_DATA_X_LSB_ADDR = 0x22,
    BNO055_QUATERNION_DATA_X_MSB_ADDR = 0x23,
    BNO055_QUATERNION_DATA_Y_LSB_ADDR = 0x24,
    BNO055_QUATERNION_DATA_Y_MSB_ADDR = 0x25,
    BNO055_QUATERNION_DATA_Z_LSB_ADDR = 0x26,
    BNO055_QUATERNION_DATA_Z_MSB_ADDR = 0x27,

    /* Linear acceleration data registers */
    BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR = 0x28,
    BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR = 0x29,
    BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR = 0x2A,
    BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR = 0x2B,
    BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR = 0x2C,
    BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR = 0x2D,

    /* Gravity data registers */
    BNO055_GRAVITY_DATA_X_LSB_ADDR = 0x2E,
    BNO055_GRAVITY_DATA_X_MSB_ADDR = 0x2F,
    BNO055_GRAVITY_DATA_Y_LSB_ADDR = 0x30,
    BNO055_GRAVITY_DATA_Y_MSB_ADDR = 0x31,
    BNO055_GRAVITY_DATA_Z_LSB_ADDR = 0x32,
    BNO055_GRAVITY_DATA_Z_MSB_ADDR = 0x33,

    /* Temperature data register */
    BNO055_TEMP_ADDR = 0x34,

    /* Status registers */
    BNO055_CALIB_STAT_ADDR = 0x35,
    BNO055_SELFTEST_RESULT_ADDR = 0x36,
    BNO055_INTR_STAT_ADDR = 0x37,

    BNO055_SYS_CLK_STAT_ADDR = 0x38,
    BNO055_SYS_STAT_ADDR = 0x39,
    BNO055_SYS_ERR_ADDR = 0x3A,

    /* Unit selection register */
    BNO055_UNIT_SEL_ADDR = 0x3B,
    BNO055_DATA_SELECT_ADDR = 0x3C,

    /* Mode registers */
    BNO055_OPR_MODE_ADDR = 0x3D,
    BNO055_PWR_MODE_ADDR = 0x3E,

    BNO055_SYS_TRIGGER_ADDR = 0x3F,
    BNO055_TEMP_SOURCE_ADDR = 0x40,

    /* Axis remap registers */
    BNO055_AXIS_MAP_CONFIG_ADDR = 0x41,
    BNO055_AXIS_MAP_SIGN_ADDR = 0x42,

    /* SIC registers */
    BNO055_SIC_MATRIX_0_LSB_ADDR = 0x43,
    BNO055_SIC_MATRIX_0_MSB_ADDR = 0x44,
    BNO055_SIC_MATRIX_1_LSB_ADDR = 0x45,
    BNO055_SIC_MATRIX_1_MSB_ADDR = 0x46,
    BNO055_SIC_MATRIX_2_LSB_ADDR = 0x47,
    BNO055_SIC_MATRIX_2_MSB_ADDR = 0x48,
    BNO055_SIC_MATRIX_3_LSB_ADDR = 0x49,
    BNO055_SIC_MATRIX_3_MSB_ADDR = 0x4A,
    BNO055_SIC_MATRIX_4_LSB_ADDR = 0x4B,
    BNO055_SIC_MATRIX_4_MSB_ADDR = 0x4C,
    BNO055_SIC_MATRIX_5_LSB_ADDR = 0x4D,
    BNO055_SIC_MATRIX_5_MSB_ADDR = 0x4E,
    BNO055_SIC_MATRIX_6_LSB_ADDR = 0x4F,
    BNO055_SIC_MATRIX_6_MSB_ADDR = 0x50,
    BNO055_SIC_MATRIX_7_LSB_ADDR = 0x51,
    BNO055_SIC_MATRIX_7_MSB_ADDR = 0x52,
    BNO055_SIC_MATRIX_8_LSB_ADDR = 0x53,
    BNO055_SIC_MATRIX_8_MSB_ADDR = 0x54,

    /* Accelerometer Offset registers */
    ACCEL_OFFSET_X_LSB_ADDR = 0x55,
    ACCEL_OFFSET_X_MSB_ADDR = 0x56,
    ACCEL_OFFSET_Y_LSB_ADDR = 0x57,
    ACCEL_OFFSET_Y_MSB_ADDR = 0x58,
    ACCEL_OFFSET_Z_LSB_ADDR = 0x59,
    ACCEL_OFFSET_Z_MSB_ADDR = 0x5A,

    /* Magnetometer Offset registers */
    MAG_OFFSET_X_LSB_ADDR = 0x5B,
    MAG_OFFSET_X_MSB_ADDR = 0x5C,
    MAG_OFFSET_Y_LSB_ADDR = 0x5D,
    MAG_OFFSET_Y_MSB_ADDR = 0x5E,
    MAG_OFFSET_Z_LSB_ADDR = 0x5F,
    MAG_OFFSET_Z_MSB_ADDR = 0x60,

    /* Gyroscope Offset register s*/
    GYRO_OFFSET_X_LSB_ADDR = 0x61,
    GYRO_OFFSET_X_MSB_ADDR = 0x62,
    GYRO_OFFSET_Y_LSB_ADDR = 0x63,
    GYRO_OFFSET_Y_MSB_ADDR = 0x64,
    GYRO_OFFSET_Z_LSB_ADDR = 0x65,
    GYRO_OFFSET_Z_MSB_ADDR = 0x66,

    /* Radius registers */
    ACCEL_RADIUS_LSB_ADDR = 0x67,
    ACCEL_RADIUS_MSB_ADDR = 0x68,
    MAG_RADIUS_LSB_ADDR = 0x69,
    MAG_RADIUS_MSB_ADDR = 0x6A
  } bno055_reg_t;

  typedef enum {
    POWER_MODE_NORMAL = 0x00,
    POWER_MODE_LOWPOWER = 0x01,
    POWER_MODE_SUSPEND = 0x02
  } bno055_powermode_t;

  typedef enum {
    /* Operation mode settings*/
    OPERATION_MODE_CONFIG = 0x00,
    OPERATION_MODE_ACCONLY = 0x01,
    OPERATION_MODE_MAGONLY = 0x02,
    OPERATION_MODE_GYRONLY = 0x03,
    OPERATION_MODE_ACCMAG = 0x04,
    OPERATION_MODE_ACCGYRO = 0x05,
    OPERATION_MODE_MAGGYRO = 0x06,
    OPERATION_MODE_AMG = 0x07,
    OPERATION_MODE_IMUPLUS = 0x08,
    OPERATION_MODE_COMPASS = 0x09,
    OPERATION_MODE_M4G = 0x0A,
    OPERATION_MODE_NDOF_FMC_OFF = 0x0B,
    OPERATION_MODE_NDOF = 0x0C
  } bno055_opmode_t;

  typedef struct {
    uint8_t accel_rev;
    uint8_t mag_rev;
    uint8_t gyro_rev;
    uint16_t sw_rev;
    uint8_t bl_rev;
  } bno055_rev_info_t;

  typedef enum {
    VECTOR_ACCELEROMETER = BNO055_ACCEL_DATA_X_LSB_ADDR,
    VECTOR_MAGNETOMETER = BNO055_MAG_DATA_X_LSB_ADDR,
    VECTOR_GYROSCOPE = BNO055_GYRO_DATA_X_LSB_ADDR,
    VECTOR_EULER = BNO055_EULER_H_LSB_ADDR,
    VECTOR_LINEARACCEL = BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR,
    VECTOR_GRAVITY = BNO055_GRAVITY_DATA_X_LSB_ADDR
  } vector_type_t;

  BNO055(uint8_t address, PinName sda, PinName scl);

  bool begin(bno055_opmode_t mode = OPERATION_MODE_NDOF);
  void setMode(bno055_opmode_t mode);
  void getRevInfo(bno055_rev_info_t*);
  void displayRevInfo(void);
  void setExtCrystalUse(bool usextal);
  void getSystemStatus(uint8_t* system_status, uint8_t* self_test_result, uint8_t* system_error);
  void displaySystemStatus(void);
  void getCalibration(uint8_t* system, uint8_t* gyro, uint8_t* accel, uint8_t* mag);

  imu::Vector<3> getVector(vector_type_t vector_type);
  imu::Quaternion getQuat(void);
  int8_t getTemp(void);

 private:
  char read8(bno055_reg_t);
  bool readLen(bno055_reg_t, char* buffer, int len);
  bool write8(bno055_reg_t, char value);

  uint8_t _address;
  bno055_opmode_t _mode;
  I2C i2c;
};

#endif
