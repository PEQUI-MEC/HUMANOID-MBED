#pragma once

#include <stdint.h>

#define DEBUG true

#define NUM_SERVOS 18
#define CLUSTER_READ_ONLY false

namespace config {
  const uint16_t defaultPos[NUM_SERVOS] = {
//  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
}
