#pragma once

/**
 * Configuration flag:
 *  CFG_X - Uses configurations for Xablau
 *  CFG_F - Uses configurations for Frank
 **/
#define CFG_X

#ifdef CFG_X
#include "config/xablau.h"
#endif

#ifdef CFG_F
#include "config/frank.h"
#endif

/**
 * Communication definitions
 **/
#define COM_BAUD_RATE 256000
#define COM_PERIOD 20 // ms
#define COM_HEADER_BYTE 0xF1
#define COM_HEADER_SIZE 5 // HEADERB CMDID SIZE CHECKSUM
#define COM_ERROR_ID 0x01
#define COM_UPDATE_ID 0x02
#define COM_UPDATE_DATA_SIZE ((NUM_SERVOS * 2) + 1)
#define COM_GOAL_ID 0x03
#define COM_GOAL_DATA_SIZE (NUM_SERVOS * 2)
#define COM_TIMEOUT_ID 0x04
