/*
 * structAssociateuctor.c
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#include "traductor.h"


void* get_UBX_NAV_TIMEUTC(UBXMessage_parsed UBXMessage,UBX_NAV_TIMEUTC* structAssociate){
	size_t offset = 0;

	memcpy(&(structAssociate->iTOW), UBXMessage.load + offset, sizeof(structAssociate->iTOW));
	offset += sizeof(&(structAssociate->iTOW));

	memcpy(&(structAssociate->tAcc), UBXMessage.load + offset, sizeof(structAssociate->tAcc));
	offset += sizeof(&(structAssociate->tAcc));

	memcpy(&(structAssociate->nano), UBXMessage.load + offset, sizeof(structAssociate->nano));
	offset += sizeof(&(structAssociate->nano));

	memcpy(&(structAssociate->year), UBXMessage.load + offset, sizeof(structAssociate->year));
	offset += sizeof(&(structAssociate->year));

	memcpy(&(structAssociate->month), UBXMessage.load + offset, sizeof(structAssociate->month));
	offset += sizeof(&(structAssociate->month));

	memcpy(&(structAssociate->day), UBXMessage.load + offset, sizeof(structAssociate->day));
	offset += sizeof(&(structAssociate->day));

	memcpy(&(structAssociate->hour), UBXMessage.load + offset, sizeof(structAssociate->hour));
	offset += sizeof(&(structAssociate->hour));

	memcpy(&(structAssociate->min), UBXMessage.load + offset, sizeof(structAssociate->min));
	offset += sizeof(&(structAssociate->min));

	memcpy(&(structAssociate->sec), UBXMessage.load + offset, sizeof(structAssociate->sec));
	offset += sizeof(&(structAssociate->sec));

	memcpy(&(structAssociate->valid), UBXMessage.load + offset, sizeof(structAssociate->valid));
	offset += sizeof(&(structAssociate->valid));

	return(structAssociate);
}

void* get_SetVal(UBXMessage_parsed UBXMessage,UBX_CFG_SETVAL* structAssociate){

	size_t offset = 0;

	memcpy(&(structAssociate->version), UBXMessage.load + offset, sizeof(structAssociate->version));
	offset += sizeof(&(structAssociate->version));

	memcpy(&(structAssociate->layers), UBXMessage.load + offset, sizeof(structAssociate->layers));
	offset += sizeof(&(structAssociate->layers));

	return(structAssociate);

}
