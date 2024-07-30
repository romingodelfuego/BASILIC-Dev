/*
 * UBX_NAV_TIMEUTC.c
 *
 *  Created on: Jul 24, 2024
 *      Author: romain.pace
 */
#include "GNSS/UBX_NAV.h"

void debug_UBX_NAV_TIMEUTC(UBX_NAV_TIMEUTC *structAssociate){
	size_t offsetDebug = 0;
	char bufferDebug[MAX_BUFFER_SIZE];

	formattedString(bufferDebug, &offsetDebug, 13, true,false,
			"\r\n___debug_UBX_NAV_SIG__\r\n"
			"iTOW [ms]: %s\r\n"
			"tAcc: %s\r\n"
			"nano: %s\r\n"
			"year: %s\r\n"
			"month: %s\r\n"
			"day: %s\r\n"
			"hour: %s\r\n"
			"min: %s\r\n"
			"sec: %s\r\n"
			"validTOW: %s\r\n"
			"validWKN: %s\r\n"
			"validUTC: %s\r\n"
			"utcStandard: %s\r\n",
			UBX_format(&(structAssociate->iTOW),TYPE_U4,NULL),
			UBX_format(&(structAssociate->tAcc),TYPE_U4,NULL),
			UBX_format(&(structAssociate->nano),TYPE_I4,NULL),
			UBX_format(&(structAssociate->year),TYPE_U2,NULL),
			UBX_format(&(structAssociate->month),TYPE_U1,NULL),
			UBX_format(&(structAssociate->day),TYPE_U1,NULL),
			UBX_format(&(structAssociate->hour),TYPE_U1,NULL),
			UBX_format(&(structAssociate->min),TYPE_U1,NULL),
			UBX_format(&(structAssociate->sec),TYPE_U1,NULL),

			UBX_format(structAssociate->valid.validTOW,TYPE_BITFIELD,NULL),
			UBX_format(structAssociate->valid.validWKN,TYPE_BITFIELD,NULL),
			UBX_format(structAssociate->valid.validUTC,TYPE_BITFIELD,NULL),
			UBX_format(structAssociate->valid.utcStandard,TYPE_BITFIELD,(char*(*)(void*))get_UTCSTANDARD)
	);
	UARTdebugQ_t UARTdebug; char *color = ANSI_COLOR_RESET;
	UARTdebug.message = pvPortMalloc(offsetDebug+1);
	UARTdebug.color = pvPortMalloc(strlen(color) + 1);
	updateMemoryUsage();
	strncpy(UARTdebug.message, bufferDebug, offsetDebug+1);
	strcpy(UARTdebug.color, color);
	offsetDebug = 0 ;

	xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
}

void create_UBX_NAV_TIMEUTC(UBXMessage_parsed* UBXMessage, UBX_NAV_TIMEUTC *structAssociate){
	size_t offset = 6; //B562...
	logMemoryUsage("START - CREATION - UBX_NAV_SIG");
	memcpy(&(structAssociate->iTOW), UBXMessage->brute->buffer + offset, sizeof(structAssociate->iTOW));
	offset += sizeof(structAssociate->iTOW);

	memcpy(&(structAssociate->tAcc), UBXMessage->brute->buffer + offset, sizeof(structAssociate->tAcc));
	offset += sizeof(structAssociate->tAcc);

	memcpy(&(structAssociate->nano), UBXMessage->brute->buffer + offset, sizeof(structAssociate->nano));
	offset += sizeof(structAssociate->nano);
	memcpy(&(structAssociate->year), UBXMessage->brute->buffer + offset, sizeof(structAssociate->year));
	offset += sizeof(structAssociate->year);

	memcpy(&(structAssociate->month), UBXMessage->brute->buffer + offset, sizeof(structAssociate->month));
	offset += sizeof(structAssociate->month);

	memcpy(&(structAssociate->day), UBXMessage->brute->buffer + offset, sizeof(structAssociate->day));
	offset += sizeof(structAssociate->day);

	memcpy(&(structAssociate->hour), UBXMessage->brute->buffer + offset, sizeof(structAssociate->hour));
	offset += sizeof(structAssociate->hour);

	memcpy(&(structAssociate->min), UBXMessage->brute->buffer + offset, sizeof(structAssociate->min));
	offset += sizeof(structAssociate->min);

	memcpy(&(structAssociate->sec), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sec));
	offset += sizeof(structAssociate->sec);

	memcpy(&(structAssociate->valid), UBXMessage->brute->buffer + offset, sizeof(structAssociate->valid));
}
void cleaner_UBX_NAV_TIMEUTC(UBX_NAV_TIMEUTC *structAssociate){
	//Nothing to clean
}


