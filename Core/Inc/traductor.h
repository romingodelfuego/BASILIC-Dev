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
	int8_t nano[4];
	uint8_t year[2];
	uint8_t month[2];
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
	uint8_t cfgData[MAX_SIZE_LOAD];
}UBX_CFG_SETVAL;
extern UBX_CFG_SETVAL UBX_CFG_SETVAL_instance;


void get_UBX_NAV_TIMEUTC(UBXMessage_parsed* UBXMessage,UBX_NAV_TIMEUTC*);
void get_SetVal(UBXMessage_parsed* UBXMessage,UBX_CFG_SETVAL*);
unsigned int bytes_to_endian(uint8_t attr[],char type_endian);
#endif /* INC_TRADUCTOR_H_ */
