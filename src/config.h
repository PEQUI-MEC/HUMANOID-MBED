#pragma once

/**
 * Robot flag:
 *  CFG_ROBOT_X - Uses configurations for Xablau
 *  CFG_ROBOT_F - Uses configurations for Frank
 **/
#define CFG_ROBOT_X

/**
 * Communication flag:
 *  CFG_COM_SYNC - Uses synchronous communication
 *  CFG_COM_ASYNC - Uses asynchronous communication
 **/
#define CFG_COM_ASYNC


#ifdef CFG_ROBOT_X
#include "config/xablau.h"
#endif

#ifdef CFG_ROBOT_F
#include "config/frank.h"
#endif

/**
 * Communication definitions
 **/
#ifdef CFG_COM_SYNC

#define COM_BAUD_RATE 256000
#define COM_PERIOD 20 // ms
#define COM_HEADER_BYTE 0xF1
#define COM_HEADER_SIZE 5 // HEADERB HEADERB CMDID SIZE CHECKSUM
#define COM_ERROR_ID 0x01
#define COM_UPDATE_ID 0x02
#define COM_UPDATE_DATA_SIZE ((NUM_SERVOS * 2) + 1)
#define COM_GOAL_ID 0x03
#define COM_GOAL_DATA_SIZE (NUM_SERVOS * 2)
#define COM_TIMEOUT_ID 0x04

#endif

#ifdef CFG_COM_ASYNC

#define COM_BAUD_RATE 256000
#define COM_PERIOD 20 // ms
#define COM_HEADER_BYTE 0xF1
#define COM_HEADER_SIZE 3 // HEADERB HEADERB CHECKSUM
#define COM_FOOTER_BYTE 0xF2
#define COM_FOOTER_SIZE 2 // FOOTERB FOOTERB
#define COM_TRANSMIT_DATA_SIZE ((NUM_SERVOS * 2) + 2)
#define COM_TRANSMIT_SIZE (COM_HEADER_SIZE + COM_TRANSMIT_DATA_SIZE + COM_FOOTER_SIZE)
#define COM_RECEIVE_DATA_SIZE (NUM_SERVOS * 2)
#define COM_RECEIVE_SIZE (COM_HEADER_SIZE + COM_RECEIVE_DATA_SIZE + COM_FOOTER_SIZE)

#endif
