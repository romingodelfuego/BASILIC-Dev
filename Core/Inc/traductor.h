/*
 * traductor.h
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#ifndef INC_TRADUCTOR_H_
#define INC_TRADUCTOR_H_

#include <stdio.h>
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

UBX_NAV_TIMEUTC get_UBX_NAV_TIMEUTC(UBXMessage_parsed load);

#endif /* INC_TRADUCTOR_H_ */
