// Copyright (C) Pololu Corporation.  See LICENSE.txt for details.

#pragma once

#include "stdint.h"
#include "mbed.h"
#include "BufferSerial.h"

/// The possible communication errors that can happen when reading the
/// acknowledgment packet from a servo.
enum class XYZrobotServoError {
  /// No error.
  None = 0,

  /// There was a timeout waiting to receive the 7-byte acknowledgment header.
  HeaderTimeout = 1,

  /// The first byte of received header was not 0xFF.
  HeaderByte1Wrong = 2,

  /// The second byte of the received header was not 0xFF.
  HeaderByte2Wrong = 3,

  /// The ID byte in the received header was wrong.
  IdWrong = 4,

  /// The CMD bytes in the received header was wrong.
  CmdWrong = 5,

  /// The size byte in the received header was wrong.
  SizeWrong = 6,

  /// There was a timeout reading the first expected block of data in the
  /// acknowledgment.
  Data1Timeout = 7,

  /// There was a timeout reading the second expected block of data in the
  /// acknowledgment.
  Data2Timeout = 8,

  /// The first byte of the checksum was wrong.
  Checksum1Wrong = 9,

  /// The second byte of the checksum was wrong.
  Checksum2Wrong = 10,

  /// The offset byte returned by an EEPROM Read or RAM Read command was wrong.
  ReadOffsetWrong = 16,

  /// The length byte returned by an EEPROM Read or RAM Read command was wrong.
  ReadLengthWrong = 17,
};

enum class XYZrobotServoBaudRate {
  B9600 = 1,
  B19200 = 2,
  B57600 = 6,
  B115200 = 12,
};

static inline uint32_t XYZrobotServoBaudRateToInt(XYZrobotServoBaudRate baud) {
  switch (baud) {
    case XYZrobotServoBaudRate::B9600:
      return 9600;
    case XYZrobotServoBaudRate::B19200:
      return 19200;
    case XYZrobotServoBaudRate::B57600:
      return 57600;
    case XYZrobotServoBaudRate::B115200:
      return 115200;
    default:
      return 0;
  }
}

/// The possible values for the ACK_Policy parameter stored in the servo's
/// EEPROM and RAM.  This parameter determins which commands the servo will send
/// an acknowledgment response for.
enum class XYZrobotServoAckPolicy {
  // The servo only responds to STAT commands.
  OnlyStat = 0,

  // The servo only responds to STAT, EEPROM Read, and RAM Read commands.
  OnlyReadAndStat = 1,

  // The servo responds to all commands.
  All = 2,
};

enum class XYZrobotServoSpdctrlPolicy {
  OpenLoop = 0,
  CloseLoop = 1,
};

/// This struct represents the data returned by a STAT command.
struct XYZrobotServoStatus {
  uint8_t statusError;
  uint8_t statusDetail;
  uint16_t pwm;
  uint16_t posRef;
  uint16_t position;
  uint16_t iBus;
} __attribute__((packed));

class XYZrobotServo {
public:
  XYZrobotServo(uint8_t id, PinName tx, PinName rx, uint32_t baud);
  XYZrobotServo(uint8_t id, BufferSerial &, uint32_t baud);

  /// Writes data from the specified buffer to the servo's EEPROM.
  ///
  /// After running this command, we recommend delaying for 10 ms per data byte
  /// before sending the next command to this servo, since writing to EEPROM
  /// takes some time and the servo cannot receive more commands until it is
  /// done.
  void eepromWrite(uint8_t startAddress, const uint8_t *, uint8_t dataSize);

  /// Reads data from the servo's EEPROM and stores it in the specified buffer.
  ///
  /// The data size should be 35 or less: otherwise the A1-16 seems to return a
  /// response with an invalid CRC.
  void eepromRead(uint8_t startAddress, uint8_t *data, uint8_t dataSize);

  /// Writes data from the specified buffer to the servo's RAM.
  void ramWrite(uint8_t startAddress, const uint8_t *, uint8_t dataSize);

  /// Reads data from the servo's RAM and stores it in the specified buffer.
  ///
  /// The data size should be 35 or less: otherwise the A1-16 seems to return a
  /// response with an invalid CRC.
  void ramRead(uint8_t startAddress, uint8_t *data, uint8_t dataSize);

  /// Write the Baud_Rate parameter byte in EEPROM, which determines which baud
  /// rate the servo uses on its serial interface.
  ///
  /// After running this command, we recommend delaying for 10 ms before sending
  /// the next command to this servo, since writing to EEPROM takes some time
  /// and the servo cannot receive more commands until it is done.
  void writeBaudRateEeprom(XYZrobotServoBaudRate baudRate);

  /// Reads the Baud_Rate parameter byte in EEPROM, which determines which baud
  /// rate the servo uses on its serial interface.
  XYZrobotServoBaudRate readBaudRateEeprom();

  /// Write the sID parameter byte in EEPROM, which determines which ID the
  /// servo uses on its serial interface.
  ///
  /// After running this command, we recommend delaying for 10 ms before sending
  /// the next command to this servo, since writing to EEPROM takes some time
  /// and the servo cannot receive more commands until it is done.
  void writeIdEeprom(uint8_t id);

  /// Reads the sID parameter byte in EEPROM, which determines which ID the
  /// servo uses on its serial interface.
  uint8_t readIdEeprom();

  /// Write the sID parameter byte in RAM, which determines which ID the
  /// servo uses on its serial interface.
  ///
  /// Write the ACK_Policy parameter byte in RAM.
  void writeIdRam(uint8_t id);

  /// Write the ACK_Policy parameter byte in EEPROM.
  ///
  /// After running this command, we recommend delaying for 10 ms before sending
  /// the next command to this servo, since writing to EEPROM takes some time
  /// and the servo cannot receive more commands until it is done.
  void writeAckPolicyEeprom(XYZrobotServoAckPolicy);

  /// Read the ACK_Policy parameter byte in EEPROM.
  XYZrobotServoAckPolicy readAckPolicyEeprom();

  /// Write the ACK_Policy parameter byte in RAM.
  void writeAckPolicyRam(XYZrobotServoAckPolicy);

  /// Write the Alarm_LED_Policy byte in RAM.  This controls which LEDs on the
  /// servo are controlled by the user and which are controlled by the system.
  ///
  /// A 0 bit means the LED is controlled by the system, and a 1 bit means the
  /// LED is controlled by the user.
  ///
  /// - Bit 0: White LED
  /// - Bit 1: Blue LED
  /// - Bit 2: Green LED
  /// - Bit 3: Red LED
  ///
  /// To control user LEDs, see writeLedControl().
  void writeAlarmLedPolicyRam(uint8_t);

  /// Sets the SPDctrl_Policy variable in RAM.
  void writeSpdctrlPolicyRam(XYZrobotServoSpdctrlPolicy policy);

  /// Sets the maximum PWM value in RAM.
  ///
  /// This should be a number between 0 and 1023 that indicates how strong the
  /// servo should be allowed to drive its motor, with 1023 corresponding to
  /// 100%.
  void writeMaxPwmRam(uint16_t value);

  /// After calling writeAlarmLedPolicyRam(), you can use this to control any
  /// LEDs that are configured as user LED.
  ///
  /// - Bit 0: White LED
  /// - Bit 1: Blue LED
  /// - Bit 2: Green LED
  /// - Bit 3: Red LED
  void writeLedControl(uint8_t);

  /// Read the ACK_Policy parameter byte in RAM.
  XYZrobotServoAckPolicy readAckPolicyRam();

  /// Sends a STAT command to the servo and returns the results.
  XYZrobotServoStatus readStatus();

  /// Uses a STAT command to read the current PWM duty cycle.
  ///
  /// See readStatus().
  uint16_t readPwm() { return readStatus().pwm; }

  /// Uses a STAT command to read the servo position.
  ///
  /// See readStatus().
  uint16_t readPosition() { return readStatus().position; }

  /// Uses a STAT command to read the servo position goal.
  ///
  /// If the servo has no position goal, this is just its current measured
  /// position.
  ///
  /// See readStatus().
  uint16_t readPosRef() { return readStatus().posRef; }

  /// Uses a STAT command to read the bus current.
  ///
  /// See readStatus().
  uint16_t readIBus() { return readStatus().iBus; }

  /// Sends an I-JOG command to set the target/goal position for this servo.
  ///
  /// The position should be a number between 0 and 1023.
  ///
  /// The playtime should the desired time for the movement to take, in units of
  /// 10 ms.  For example, a playtime of 50 would correspond to 500 ms or 0.5
  /// seconds.
  void setPosition(uint16_t position, uint8_t playtime = 0);

  /// Sends an I-JOG command to set the speed for this servo.
  ///
  /// The speed should be a number between -1023 and 1023.
  ///
  /// A value of 0 causes an abrupt stop.  Non-zero values generally cause the
  /// servo to smoothly ramp its speed up or down to the specified value.
  ///
  /// The playtime specifies how long this command will last, in units of 10 ms.
  /// A value of 0 makes it last indefinitely.
  ///
  /// See writeSpeedControlPolicyRam().
  void setSpeed(int16_t speed, uint8_t playtime = 0);

  /// Sends an I-JOG command to turn off the servo's motor.
  ///
  /// Note that this command interacts badly with the A1-16 servo's speed
  /// ramping feature.  If you are in speed control mode and then send this
  /// command, the servo will still remember what speed it was using before it
  /// received the torqueOff() command.  If you later send a setSpeed() command
  /// with a non-zero speed, it will ramp up or down to that speed, starting
  /// from the remembered speed instead of starting from zero.  If you encounter
  /// this issue, you can call `setSpeed(0)` immediately before the next
  /// non-zero setSpeed() command to solve it.
  void torqueOff();

  // Resets all parameters in EEPROM to their default values.
  //
  // After running this command, we recommend delaying for 2500 ms before
  // sending the next command to this servo, since it takes the servo a while to
  // change its parameters.
  void rollback();

  // Resets the servo.
  //
  // After running this command, we recommend delaying for 2500 ms before
  // sending the next command to this servo, since it takes the servo a while to
  // restart.
  void reboot();

  /// Returns the communication error from the last command.  The return value
  /// will be 0 if there was no error and non-zero if there was an error.  The
  /// return value will be one of the values of the XYZrobotServoError enum.
  uint8_t getLastError() const { return (uint8_t)lastError; }

  /// Get the servo ID assigned to this object.
  uint8_t getId() const { return id; }

private:
  void flushRead();

  int readBytes(uint8_t *data, uint8_t size, int timeout);

  void writeBytes(const uint8_t *data, uint8_t size);

  void sendRequest(uint8_t cmd,
    const uint8_t *data1, uint8_t data1Size,
    const uint8_t *data2 = NULL, uint8_t data2Size = 0);

  void readAck(uint8_t cmd,
    uint8_t *data1, uint8_t data1Size,
    uint8_t *data2 = NULL, uint8_t data2Size = 0);

  void memoryWrite(uint8_t cmd, uint8_t startAddress, const uint8_t *data, uint8_t dataSize);

  void memoryRead(uint8_t cmd, uint8_t startAddress, uint8_t *data, uint8_t dataSize);

  void sendIJog(uint16_t goal, uint8_t type, uint8_t playtime);

  XYZrobotServoError lastError;

  uint8_t id;

  uint32_t baud;

  BufferSerial* serial;
};
