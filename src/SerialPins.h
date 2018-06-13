#pragma once
#include "mbed.h"

class SerialPins {
public:
  const static PinName TX2 = PD_5;
  const static PinName RX2 = PD_6;

  // Conflito com a serial stdio
  const static PinName TX3 = PB_10;
  const static PinName RX3 = PB_11;

  const static PinName TX4 = PA_0;
  const static PinName RX4 = PC_11;

  const static PinName TX5 = PC_12;
  const static PinName RX5 = PD_2;

  const static PinName TX6 = PC_6;
  const static PinName RX6 = PC_7;

  const static PinName TX7 = PE_8;
  const static PinName RX7 = PE_7;
};
