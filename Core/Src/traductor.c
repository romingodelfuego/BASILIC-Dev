/*
 * structAssociateuctor.c
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#include "traductor.h"


void debug_UBX_NAV_TIMEUTC(UBXMessage_parsed* UBXMessage,UBX_NAV_TIMEUTC *structAssociate){
	size_t offset = 0;
	ITM_Port32(31)=21;
	memcpy(&(structAssociate->iTOW), UBXMessage->load + offset, sizeof(structAssociate->iTOW));
	offset += sizeof(structAssociate->iTOW);

	memcpy(&(structAssociate->tAcc), UBXMessage->load + offset, sizeof(structAssociate->tAcc));
	offset += sizeof(structAssociate->tAcc);

	memcpy(&(structAssociate->nano), UBXMessage->load + offset, sizeof(structAssociate->nano));
	offset += sizeof(structAssociate->nano);

	memcpy(&(structAssociate->year), UBXMessage->load + offset, sizeof(structAssociate->year));
	offset += sizeof(structAssociate->year);

	memcpy(&(structAssociate->month), UBXMessage->load + offset, sizeof(structAssociate->month));
	offset += sizeof(structAssociate->month);

	memcpy(&(structAssociate->day), UBXMessage->load + offset, sizeof(structAssociate->day));
	offset += sizeof(structAssociate->day);

	memcpy(&(structAssociate->hour), UBXMessage->load + offset, sizeof(structAssociate->hour));
	offset += sizeof(structAssociate->hour);

	memcpy(&(structAssociate->min), UBXMessage->load + offset, sizeof(structAssociate->min));
	offset += sizeof(structAssociate->min);

	memcpy(&(structAssociate->sec), UBXMessage->load + offset, sizeof(structAssociate->sec));
	offset += sizeof(structAssociate->sec);

	memcpy(&(structAssociate->valid), UBXMessage->load + offset, sizeof(structAssociate->valid));
	offset += sizeof(structAssociate->valid);

	int len =sprintf(UBXMessage->bufferDebug,
			"\r\n___debug_UBX_NAV_TIMEUTC__\r\n"
			"iTOW [ms]: %u\r\n"
			"tAcc [ns]: %u\r\n"
			"Date [YYYY-MM-DD]: %u-%u-%u\r\n"
			"Time [HH:MM:SS]: %u:%u:%u - %u [ns]\r\n"
			"valid: %u\r\n",
			bytes_to_endian(structAssociate->iTOW,sizeof(structAssociate->iTOW),'l'),
			bytes_to_endian(structAssociate->tAcc,sizeof(structAssociate->tAcc),'l'),
			bytes_to_endian(structAssociate->year,sizeof(structAssociate->year),'l'),
			bytes_to_endian(structAssociate->month,sizeof(structAssociate->month),'l'),
			bytes_to_endian(structAssociate->day,sizeof(structAssociate->day),'l'),
			bytes_to_endian(structAssociate->hour,sizeof(structAssociate->hour),'l'),
			bytes_to_endian(structAssociate->min,sizeof(structAssociate->min),'l'),
			bytes_to_endian(structAssociate->sec,sizeof(structAssociate->sec),'l'),
			bytes_to_endian(structAssociate->nano,sizeof(structAssociate->nano),'l'),
			bytes_to_endian(structAssociate->valid,sizeof(structAssociate->valid),'l')
	);

	fill_unuse_memory(UBXMessage,len);
	ITM_Port32(31)=20;
}

void debug_SetVal(UBXMessage_parsed* UBXMessage,UBX_CFG_SETVAL* structAssociate){

	size_t offset = 0;

	memcpy(&(structAssociate->version), UBXMessage->load + offset, sizeof(structAssociate->version));
	offset += sizeof((structAssociate->version));

	memcpy(&(structAssociate->layers), UBXMessage->load + offset, sizeof(structAssociate->layers));
	offset += sizeof((structAssociate->layers));

	offset += sizeof((structAssociate->reserved));

	memcpy(&(structAssociate->cfgData), UBXMessage->load + offset, sizeof(structAssociate->cfgData));
	offset += sizeof((structAssociate->cfgData));
	//Diviser ici en flag
	//

	int len = sprintf(UBXMessage->bufferDebug,
			"\r\n__debug_SetVal___\r\n"
			"version: %u\r\n"
			"layers: %u\r\n"
			"KeyId|Value: %s\r\n",
			bytes_to_endian(structAssociate->version,sizeof(structAssociate->version),'b'),
			bytes_to_endian(structAssociate->layers,sizeof(structAssociate->layers),'l'),
			array_to_hex_string(structAssociate->cfgData,sizeof(structAssociate->cfgData))
	);
	fill_unuse_memory(UBXMessage,len);
}

unsigned int bytes_to_endian(uint8_t attr[], size_t length, char type_endian) {
	uint64_t result = 0;
	if (type_endian == 'l') { // little-endian
		for (size_t i = 0; i < length; ++i) {
			result |= ((uint64_t)attr[i]) << (i * 8);
		}
	} else if (type_endian == 'b') { // big-endian
		for (size_t i = 0; i < length; ++i) {
			result |= ((uint64_t)attr[i]) << ((sizeof(uint64_t) - i - 1) * 8);
		}
	}
	else if (type_endian == '2'){ //little-endiand 2's complements
		if (attr[length - 1] & 0x80) { // Check if the most significant bit (MSB) is 1 (indicating negative number)
			result = -1; // Initialize result to all 1s for negative number
		}
		for (size_t i = 0; i < length; ++i) {
			result |= ((uint64_t)attr[i]) << (i * 8);
		}
	}
	return result;
}

void fill_unuse_memory(UBXMessage_parsed* UBXMessage,int len_use){
	if (len_use < sizeof(UBXMessage->bufferDebug)) {
		memset(UBXMessage->bufferDebug + len_use, '/', sizeof(UBXMessage->bufferDebug) - len_use);
	}
}

char* array_to_hex_string(const uint8_t* array, size_t length) {
	// Taille maximale pour le buffer
	static char hex_string[UART_RX_BUFFER_SIZE * 2 + 1];
	for (size_t i = 0; i < length; ++i) {
		sprintf(hex_string + (i * 2), "%02x", array[i]);
	}
	hex_string[length * 2] = '\0';
	return hex_string;
}
