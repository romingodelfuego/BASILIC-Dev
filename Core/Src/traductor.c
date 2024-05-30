/*
 * structAssociateuctor.c
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#include "traductor.h"


void get_UBX_NAV_TIMEUTC(UBXMessage_parsed* UBXMessage,UBX_NAV_TIMEUTC *structAssociate){
	size_t offset = 0;

	memcpy(&(structAssociate->iTOW), UBXMessage->load + offset, sizeof(structAssociate->iTOW));
	offset += sizeof(&(structAssociate->iTOW));

	memcpy(&(structAssociate->tAcc), UBXMessage->load + offset, sizeof(structAssociate->tAcc));
	offset += sizeof(&(structAssociate->tAcc));

	memcpy(&(structAssociate->nano), UBXMessage->load + offset, sizeof(structAssociate->nano));
	offset += sizeof(&(structAssociate->nano));

	memcpy(&(structAssociate->year), UBXMessage->load + offset, sizeof(structAssociate->year));
	offset += sizeof(&(structAssociate->year));

	memcpy(&(structAssociate->month), UBXMessage->load + offset, sizeof(structAssociate->month));
	offset += sizeof(&(structAssociate->month));

	memcpy(&(structAssociate->day), UBXMessage->load + offset, sizeof(structAssociate->day));
	offset += sizeof(&(structAssociate->day));

	memcpy(&(structAssociate->hour), UBXMessage->load + offset, sizeof(structAssociate->hour));
	offset += sizeof(&(structAssociate->hour));

	memcpy(&(structAssociate->min), UBXMessage->load + offset, sizeof(structAssociate->min));
	offset += sizeof(&(structAssociate->min));

	memcpy(&(structAssociate->sec), UBXMessage->load + offset, sizeof(structAssociate->sec));
	offset += sizeof(&(structAssociate->sec));

	memcpy(&(structAssociate->valid), UBXMessage->load + offset, sizeof(structAssociate->valid));
	offset += sizeof(&(structAssociate->valid));

	sprintf(UBXMessage->bufferDebug,
			"\r\n___get_UBX_NAV_TIMEUTC__\r\n"
			"iTOW: %u\r\n"
			"tAcc: %u\r\n"
			"nano: %d\r\n"
			"year: %u\r\n"
			"month: %u\r\n"
			"day: %u\r\n"
			"hour: %u\r\n"
			"min: %u\r\n"
			"sec: %u\r\n"
			"valid: %u\r\n",
			bytes_to_endian(structAssociate->iTOW,'l'),
			bytes_to_endian(structAssociate->tAcc,'b'),
			bytes_to_endian(structAssociate->nano,'l'),
			bytes_to_endian(structAssociate->year,'l'),
			bytes_to_endian(structAssociate->month,'l'),
			bytes_to_endian(structAssociate->day,'l'),
			bytes_to_endian(structAssociate->hour,'l'),
			bytes_to_endian(structAssociate->min,'l'),
			bytes_to_endian(structAssociate->sec,'l'),
			bytes_to_endian(structAssociate->valid,'l')
			);

}

void get_SetVal(UBXMessage_parsed* UBXMessage,UBX_CFG_SETVAL* structAssociate){

	size_t offset = 0;

	memcpy(&(structAssociate->version), UBXMessage->load + offset, sizeof(structAssociate->version));
	offset += sizeof((structAssociate->version));

	memcpy(&(structAssociate->layers), UBXMessage->load + offset, sizeof(structAssociate->layers));
	offset += sizeof((structAssociate->layers));
	//Diviser ici en flag
	//
	sprintf(UBXMessage->bufferDebug,
			"\r\n___get_SetVal___\r\n"
			"version: %u\r\n"
			"layers: %u\r\n",
			bytes_to_endian(structAssociate->version,'l'),
			bytes_to_endian(structAssociate->layers,'l')
			);
}


unsigned int bytes_to_endian(uint8_t attr[],char type_endian) {
	uint64_t result = 0;
	size_t length = sizeof(attr) / sizeof(attr[0]);

	if (type_endian == 'l') { // little-endian
		for (size_t i = 0; i < length; ++i) {
			result |= ((uint64_t)attr[i]) << (i * 8);
		}
	} else if (type_endian == 'b') { // big-endian
		for (size_t i = 0; i < length; ++i) {
			result |= ((uint64_t)attr[i]) << ((sizeof(uint64_t) - i - 1) * 8);
		}
	}

	return result;
}

