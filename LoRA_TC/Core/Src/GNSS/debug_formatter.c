/*
 * structAssociateuctor.c
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */
#include <GNSS/debug_formatter.h>

char* UBX_format(void* fieldToDebug, DataType type, char* (*funcSpe)(void*)){
	switch (type) {
	case TYPE_BITFIELD:
	{
		uint8_t* fieldCasted = (uint8_t*)fieldToDebug;//Si on n'a pas besoin de fonction speciale pour debug
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted,1);
		else return funcSpe(&fieldCasted);
	}
	break;
	case TYPE_U1:
	{
		U1* fieldCasted = (U1*)fieldToDebug;//Si on n'a pas besoin de fonction speciale pour debug
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_I1:
	{
		I1* fieldCasted = (I1*)fieldToDebug;
		if (funcSpe == NULL) return int8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_X1:
	{
		X1* fieldCasted = (X1*)fieldToDebug;
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_U2:
	{
		U2* fieldCasted = (U2*)fieldToDebug;//Si on n'a pas besoin de fonction speciale pour debug
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_I2:
	{
		I2* fieldCasted = (I2*)fieldToDebug;
		if (funcSpe == NULL) return int8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_X2:
	{
		X2* fieldCasted = (X2*)fieldToDebug;
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	default:
		return("Unknown type");
		break;
	}
	return "Error";
}

/*
void debug_UBX_NAV_TIMEUTC(UBXMessage_parsed* UBXMessage,UBX_NAV_TIMEUTC *structAssociate){
	size_t offset = 6;
	ITM_Port32(31)=21;
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
	offset += sizeof(structAssociate->valid);


	char bufferDebug[1024];
	//int len =
	snprintf(bufferDebug,  (size_t)1024,
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
	UART_Transmit_With_Color(bufferDebug, ANSI_COLOR_MAGENTA);
	//fill_unuse_memory(UBXMessage,len);
	ITM_Port32(31)=20;
}

void debug_SetVal(UBXMessage_parsed* UBXMessage,UBX_CFG_SETVAL* structAssociate){

	size_t offset = 6;

	memcpy(&(structAssociate->version), UBXMessage->brute->buffer + offset, sizeof(structAssociate->version));
	offset += sizeof((structAssociate->version));

	memcpy(&(structAssociate->layers), UBXMessage->brute->buffer + offset, sizeof(structAssociate->layers));
	offset += sizeof((structAssociate->layers));

	offset += sizeof((structAssociate->reserved));

	memcpy(&(structAssociate->cfgData), UBXMessage->brute->buffer + offset, sizeof(structAssociate->cfgData));
	offset += sizeof((structAssociate->cfgData));
	//Diviser ici en flag
	//
	char bufferDebug[1024];
	int len =snprintf(bufferDebug,  (size_t)1024,
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
void debug_PollMessage(UBXMessage_parsed* UBXMessage,UBX_CFG_MSG* structAssociate){
	size_t offset = 6;

	memcpy(&(structAssociate->msgClass), UBXMessage->brute->buffer + offset, sizeof(structAssociate->msgClass));
	offset += sizeof((structAssociate->msgClass));

	memcpy(&(structAssociate->msgID), UBXMessage->brute->buffer + offset, sizeof(structAssociate->msgID));
	offset += sizeof((structAssociate->msgID));

	memcpy(&(structAssociate->rate), UBXMessage->brute->buffer + offset, sizeof(structAssociate->rate));
	offset += sizeof((structAssociate->rate));

	char bufferDebug[1024];
	int len =snprintf(bufferDebug,  (size_t)1024,
			"\r\n__debug_PollMsg___\r\n"
			"msgClass: %u\r\n"
			"msgID: %u\r\n"
			"rate :%u\r\n",
			bytes_to_endian(structAssociate->msgClass,sizeof(structAssociate->msgClass),'l'),
			bytes_to_endian(structAssociate->msgID,sizeof(structAssociate->msgID),'l'),
			bytes_to_endian(structAssociate->rate,sizeof(structAssociate->rate),'l')
	);
	fill_unuse_memory(UBXMessage,len);

}*/



