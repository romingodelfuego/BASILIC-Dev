/*
 * traductor.h
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#ifndef INC_TRADUCTOR_H_
#define INC_TRADUCTOR_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>  // Pour size_t
#include <stdlib.h>
#include <string.h> // Pour memcpy
#include "UBXParser.h"

typedef struct {
	uint8_t iTOW[4];
	uint8_t tAcc[4];
	uint8_t nano[4];
	uint8_t year[2];
	uint8_t month[1];
	uint8_t day[1];
	uint8_t hour[1];
	uint8_t min[1];
	uint8_t sec[1];
	uint8_t valid[1];
}UBX_NAV_TIMEUTC;
extern UBX_NAV_TIMEUTC UBX_NAV_TIMEUTC_instance;

typedef struct {
	uint8_t version[1];
	uint8_t layers[1];
	uint8_t reserved[2];
	uint8_t cfgData[UART_RX_BUFFER_SIZE];
}UBX_CFG_SETVAL;
extern UBX_CFG_SETVAL UBX_CFG_SETVAL_instance;

typedef struct {
	uint8_t msgClass[1];
	uint8_t msgID[1];
	uint8_t rate[1];
}UBX_CFG_MSG;
extern UBX_CFG_MSG UBX_CFG_MSG_instance;


void debug_UBX_NAV_TIMEUTC(UBXMessage_parsed* UBXMessage,UBX_NAV_TIMEUTC*);
void debug_SetVal(UBXMessage_parsed* UBXMessage,UBX_CFG_SETVAL*);
void debug_PollMessage(UBXMessage_parsed* UBXMessage,UBX_CFG_MSG* structAssociate);

unsigned int bytes_to_endian(uint8_t attr[],size_t length,char type_endian);
char* array_to_hex_string(const uint8_t* array, size_t length);
void fill_unuse_memory(UBXMessage_parsed* UBXMessage,int len_use);
#endif /* INC_TRADUCTOR_H_ */
