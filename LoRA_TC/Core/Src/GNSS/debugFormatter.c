/*
 * structAssociateuctor.c
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */
#include <GNSS/debugFormatter.h>
#include "RTOS_subfunctions/RTOS_extern.h"
#define SEPARATON "\r\n-----------------------------\r\n"
#define MAX_BUFFER_SIZE 512

void debug_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate){
	size_t offsetDebug = 0;
	char bufferDebug[MAX_BUFFER_SIZE];

	const int MAX_COLUMNS = 18;
	static char *columnNames[18] = {"Signal","gnssId","svId","sigId","freqId","prRes","cno",
			"qualityInd","corrSource","ionoModel","health","prSmoothed","prUsed","crUsed",
			"doUsed","prCorrUsed","crCorrUsed","doCorrUsed"};


	formattedString(bufferDebug, &offsetDebug, 3, true,
			"\r\n___debug_UBX_NAV_SIG__\r\n"
			"iTOW [ms]: %s\r\n"
			"Version: %s\r\n"
			"NumSigs: %s\r\n",
			uint8ArrayToString(structAssociate->iTOW,sizeof(structAssociate->iTOW)),
			uint8ArrayToString(structAssociate->version,sizeof(structAssociate->version)),
			uint8ArrayToString(structAssociate->numSigs,sizeof(structAssociate->numSigs))
	);

	formattedString(bufferDebug, &offsetDebug,1, false, SEPARATON);
	for (int i = 0; i < MAX_COLUMNS; ++i) {
		formattedString(bufferDebug, &offsetDebug, 1, false,
				"| %-5s |",
				columnNames[i]);
	}

	formattedString(bufferDebug, &offsetDebug, 1, false, SEPARATON);

	UARTdebugQ_t UARTdebug; char *color = ANSI_COLOR_RESET;
	UARTdebug.message = pvPortMalloc(offsetDebug+1);
	UARTdebug.color = pvPortMalloc(strlen(color) + 1);
	updateMemoryUsage();
	strncpy(UARTdebug.message, bufferDebug, offsetDebug+1);
	strcpy(UARTdebug.color, color);
	offsetDebug = 0 ;

	xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
	updateMemoryUsage();

	for (int i = 0; i < structAssociate->numSigs[0]; i++) {
		formattedString(bufferDebug, &offsetDebug, 1, false,
				"| %-5i |",i+1);
		formattedString(bufferDebug, &offsetDebug, MAX_COLUMNS - 1,true,
				"| %-5s || %-5s || %-5s || %-5s || %-5s || %-5s || %-5s |"
				"| %-5s || %-5s || %-5s || %-5s || %-5s || %-5s || %-5s || %-5s |"
				"| %-5s || %-5s |\n",
				uint8ArrayToString(structAssociate->sig[i].gnssId,sizeof(structAssociate->sig[i].gnssId)),
				uint8ArrayToString(structAssociate->sig[i].svId,sizeof(structAssociate->sig[i].svId)),
				uint8ArrayToString(structAssociate->sig[i].sigId,sizeof(structAssociate->sig[i].sigId)),
				uint8ArrayToString(structAssociate->sig[i].freqId,sizeof(structAssociate->sig[i].freqId)),
				uint8ArrayToString(structAssociate->sig[i].prRes,sizeof(structAssociate->sig[i].prRes)),
				uint8ArrayToString(structAssociate->sig[i].cno,sizeof(structAssociate->sig[i].cno)),
				uint8ArrayToString(structAssociate->sig[i].qualityInd,sizeof(structAssociate->sig[i].qualityInd)),
				uint8ArrayToString(structAssociate->sig[i].corrSource,sizeof(structAssociate->sig[i].corrSource)),
				uint8ArrayToString(structAssociate->sig[i].ionoModel,sizeof(structAssociate->sig[i].ionoModel)),
				uint8ArrayToString(structAssociate->sig[i].health,sizeof(structAssociate->sig[i].health)),
				uint8ArrayToString(structAssociate->sig[i].prSmoothed,sizeof(structAssociate->sig[i].prSmoothed)),
				uint8ArrayToString(structAssociate->sig[i].prUsed,sizeof(structAssociate->sig[i].prUsed)),
				uint8ArrayToString(structAssociate->sig[i].crUsed,sizeof(structAssociate->sig[i].crUsed)),
				uint8ArrayToString(structAssociate->sig[i].doUsed,sizeof(structAssociate->sig[i].doUsed)),
				uint8ArrayToString(structAssociate->sig[i].prCorrUsed,sizeof(structAssociate->sig[i].prCorrUsed)),
				uint8ArrayToString(structAssociate->sig[i].crCorrUsed,sizeof(structAssociate->sig[i].crCorrUsed)),
				uint8ArrayToString(structAssociate->sig[i].doCorrUsed,sizeof(structAssociate->sig[i].doCorrUsed))
		);
		formattedString(bufferDebug, &offsetDebug, 1, false, SEPARATON);

		UARTdebugQ_t UARTdebug; char *color = ANSI_COLOR_RESET;

		UARTdebug.message = pvPortMalloc(offsetDebug+1);
		UARTdebug.color = pvPortMalloc(strlen(color) + 1);
		updateMemoryUsage();
		strncpy(UARTdebug.message, bufferDebug, offsetDebug+1);
		strcpy(UARTdebug.color, color);
		offsetDebug=0;

		xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
		updateMemoryUsage();
		}
}
char* uint8ArrayToString(uint8_t* array, size_t size) {
	// Allocate enough space for the string
	char* str = pvPortMalloc(size * 3 + 1); // 2 characters per byte + 1 for space or null terminator
	char* ptr = str;
	for (size_t i = 0; i < size; i++) {
		ptr += sprintf(ptr, "%02X", array[i]);
		if (i < size - 1) {
			*ptr++ = ' ';
		}
	}
	return str;
}

void formattedString(char* buffer, size_t* offset, int numArgs, bool freeMemory, const char* format,...){
	va_list args;
	va_start(args, format);
	*offset += vsnprintf(buffer + *offset, MAX_BUFFER_SIZE - *offset, format, args);
	va_end(args);

	if (freeMemory){
		va_start(args, format);
		for (int i = 0; i < numArgs; i++) {
			void* arg = va_arg(args, void*);
			vPortFree(arg);
		}
		va_end(args);
	}
}
/*void UBX_format(UBX_data_type dataType, UBX_field_name fieldName, void* valueOfField, size_t size){
	if (true){
		switch (fieldName){
		case gnssID:
			valueOfField

		}
	}
}*/
/*
void debug_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate){
	size_t offsetDebug = 0;
	char bufferDebug[MAX_BUFFER_SIZE];

	const int MAX_COLUMNS = 18;
	static char *columnNames[18] = {"Signal","gnssId","svId","sigId","freqId","prRes","cno",
			"qualityInd","corrSource","ionoModel","health","prSmoothed","prUsed","crUsed",
			"doUsed","prCorrUsed","crCorrUsed","doCorrUsed"};


	offsetDebug +=snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug,
			"\r\n___debug_UBX_NAV_SIG__\r\n"
			"iTOW [ms]: %s\r\n"
			"Version: %s\r\n"
			"NumSigs: %s\r\n",
			uint8ArrayToString(structAssociate->iTOW,sizeof(structAssociate->iTOW)),
			uint8ArrayToString(structAssociate->version,sizeof(structAssociate->version)),
			uint8ArrayToString(structAssociate->numSigs,sizeof(structAssociate->numSigs))
	);

	offsetDebug +=snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug, SEPARATON);
	for (int i = 0; i < MAX_COLUMNS; ++i) {
		offsetDebug += snprintf(bufferDebug + offsetDebug,
				MAX_BUFFER_SIZE - offsetDebug,
				"| %-5s |", columnNames[i]);
	}

	offsetDebug += snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug, SEPARATON);
	UARTdebugQ_t UARTdebug; char *color = ANSI_COLOR_RESET;

	UARTdebug.message = pvPortMalloc(offsetDebug);
	UARTdebug.color = pvPortMalloc(strlen(color) + 1);
	updateMemoryUsage();
	strncpy(UARTdebug.message, bufferDebug, offsetDebug);
	strcpy(UARTdebug.color, color);
	offsetDebug = 0 ;

	xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
	updateMemoryUsage();


	for (int i = 0; i < structAssociate->numSigs[0]; i++) {
		offsetDebug += snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug,
				"| %-5i || %-5s || %-5s || %-5s || %-5s || %-5s || %-5s || %-5s |"
				"| %-5s || %-5s || %-5s || %-5s || %-5s || %-5s || %-5s || %-5s |"
				"| %-5s || %-5s |\n",
				i,
				uint8ArrayToString(structAssociate->sig[i].gnssId,sizeof(structAssociate->sig[i].gnssId)),
				uint8ArrayToString(structAssociate->sig[i].svId,sizeof(structAssociate->sig[i].svId)),
				uint8ArrayToString(structAssociate->sig[i].sigId,sizeof(structAssociate->sig[i].sigId)),
				uint8ArrayToString(structAssociate->sig[i].freqId,sizeof(structAssociate->sig[i].freqId)),
				uint8ArrayToString(structAssociate->sig[i].prRes,sizeof(structAssociate->sig[i].prRes)),
				uint8ArrayToString(structAssociate->sig[i].cno,sizeof(structAssociate->sig[i].cno)),
				uint8ArrayToString(structAssociate->sig[i].qualityInd,sizeof(structAssociate->sig[i].qualityInd)),
				uint8ArrayToString(structAssociate->sig[i].corrSource,sizeof(structAssociate->sig[i].corrSource)),
				uint8ArrayToString(structAssociate->sig[i].ionoModel,sizeof(structAssociate->sig[i].ionoModel)),
				uint8ArrayToString(structAssociate->sig[i].health,sizeof(structAssociate->sig[i].health)),
				uint8ArrayToString(structAssociate->sig[i].prSmoothed,sizeof(structAssociate->sig[i].prSmoothed)),
				uint8ArrayToString(structAssociate->sig[i].prUsed,sizeof(structAssociate->sig[i].prUsed)),
				uint8ArrayToString(structAssociate->sig[i].crUsed,sizeof(structAssociate->sig[i].crUsed)),
				uint8ArrayToString(structAssociate->sig[i].doUsed,sizeof(structAssociate->sig[i].doUsed)),
				uint8ArrayToString(structAssociate->sig[i].prCorrUsed,sizeof(structAssociate->sig[i].prCorrUsed)),
				uint8ArrayToString(structAssociate->sig[i].crCorrUsed,sizeof(structAssociate->sig[i].crCorrUsed)),
				uint8ArrayToString(structAssociate->sig[i].doCorrUsed,sizeof(structAssociate->sig[i].doCorrUsed))
		);
		offsetDebug += snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug, SEPARATON);
		UARTdebugQ_t UARTdebug; char *color = ANSI_COLOR_RESET;

		UARTdebug.message = pvPortMalloc(offsetDebug);
		UARTdebug.color = pvPortMalloc(strlen(color) + 1);
		updateMemoryUsage();
		strncpy(UARTdebug.message, bufferDebug, offsetDebug);
		strcpy(UARTdebug.color, color);
		offsetDebug=0;

		xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
		updateMemoryUsage();



	}
}*/
/*
void debug_UBX_NAV_SIG1(UBX_NAV_SIG *structAssociate){
	size_t offsetDebug = 0;
	char bufferDebug[MAX_BUFFER_SIZE];

	const int MAX_COLUMNS = 18;
	static char *columnNames[18] = {"Signal","gnssId","svId","sigId","freqId","prRes","cno",
			"qualityInd","corrSource","ionoModel","health","prSmoothed","prUsed","crUsed",
			"doUsed","prCorrUsed","crCorrUsed","doCorrUsed"};


	offsetDebug += snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug,
			"\r\n___debug_UBX_NAV_SIG__\r\n"
			"iTOW [ms]: %s\r\n"
			"Version: %s\r\n"
			"NumSigs: %s\r\n",
			uint8ArrayToString(structAssociate->iTOW,sizeof(structAssociate->iTOW)),
			uint8ArrayToString(structAssociate->version,sizeof(structAssociate->version)),
			uint8ArrayToString(structAssociate->numSigs,sizeof(structAssociate->numSigs))
	);

	offsetDebug += snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug, SEPARATON);
	for (int i = 0; i < MAX_COLUMNS; ++i) {
		offsetDebug += snprintf(bufferDebug + offsetDebug,
				MAX_BUFFER_SIZE - offsetDebug,
				"| %-10s |", columnNames[i]);
	}

	offsetDebug += snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug, SEPARATON);

	for (int i = 0; i < structAssociate->numSigs[0]; i++) {
		offsetDebug += snprintf(bufferDebug + offsetDebug, MAX_BUFFER_SIZE - offsetDebug,
				"| %-10i || %-10s || %-10s || %-10s || %-10s || %-10s || %-10s || %-10s |"
				"| %-10s || %-10s || %-10s || %-10s || %-10s || %-10s || %-10s || %-10s |"
				"| %-10s || %-10s |\n",
				i,
				uint8ArrayToString(structAssociate->sig[i].gnssId,sizeof(structAssociate->sig[i].gnssId)),
				uint8ArrayToString(structAssociate->sig[i].svId,sizeof(structAssociate->sig[i].svId)),
				uint8ArrayToString(structAssociate->sig[i].sigId,sizeof(structAssociate->sig[i].sigId)),
				uint8ArrayToString(structAssociate->sig[i].freqId,sizeof(structAssociate->sig[i].freqId)),
				uint8ArrayToString(structAssociate->sig[i].prRes,sizeof(structAssociate->sig[i].prRes)),
				uint8ArrayToString(structAssociate->sig[i].cno,sizeof(structAssociate->sig[i].cno)),
				uint8ArrayToString(structAssociate->sig[i].qualityInd,sizeof(structAssociate->sig[i].qualityInd)),
				uint8ArrayToString(structAssociate->sig[i].corrSource,sizeof(structAssociate->sig[i].corrSource)),
				uint8ArrayToString(structAssociate->sig[i].ionoModel,sizeof(structAssociate->sig[i].ionoModel)),
				uint8ArrayToString(structAssociate->sig[i].health,sizeof(structAssociate->sig[i].health)),
				uint8ArrayToString(structAssociate->sig[i].prSmoothed,sizeof(structAssociate->sig[i].prSmoothed)),
				uint8ArrayToString(structAssociate->sig[i].prUsed,sizeof(structAssociate->sig[i].prUsed)),
				uint8ArrayToString(structAssociate->sig[i].crUsed,sizeof(structAssociate->sig[i].crUsed)),
				uint8ArrayToString(structAssociate->sig[i].doUsed,sizeof(structAssociate->sig[i].doUsed)),
				uint8ArrayToString(structAssociate->sig[i].prCorrUsed,sizeof(structAssociate->sig[i].prCorrUsed)),
				uint8ArrayToString(structAssociate->sig[i].crCorrUsed,sizeof(structAssociate->sig[i].crCorrUsed)),
				uint8ArrayToString(structAssociate->sig[i].doCorrUsed,sizeof(structAssociate->sig[i].doCorrUsed))
		);
	}
	UARTdebugQ_t UARTdebug; char *color = ANSI_COLOR_RESET;

	UARTdebug.message = pvPortMalloc(offsetDebug);
	UARTdebug.color = pvPortMalloc(strlen(color) + 1);
	updateMemoryUsage();
	strcpy(UARTdebug.message, bufferDebug);
	strcpy(UARTdebug.color, color);

	xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
}






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
}*/


