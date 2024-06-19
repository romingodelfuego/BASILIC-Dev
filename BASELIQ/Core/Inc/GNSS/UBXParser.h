/*
 * UBXParser.h
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#ifndef INC_UBXPARSER_H_
#define INC_UBXPARSER_H_

#include <stdio.h>
#include <stdint.h>
#include "shared.h"

typedef struct {
    uint8_t *buffer;
    size_t size;
} DynamicBuffer;

typedef enum {
	INIT,
    NMEA,
    UBX,
} OutputProtocol;

extern OutputProtocol protocol;

typedef struct {
	uint8_t msgClass;
	uint8_t msgID;
	uint16_t len;
	char bufferDebug[UART_DEBUG_BUFFER_SIZE];
	DynamicBuffer* load; // Pointeur de tableau
	DynamicBuffer* UBX_Brute; // Pointeur de tableau de longeur variable
}UBXMessage_parsed ;

typedef struct {
	char* NMEA_Brute;
}NMEAMessage_parsed;

typedef union {
	NMEAMessage_parsed* NMEAMessage;
	UBXMessage_parsed* UBXMessage;
}MessageStruct;

typedef struct{
	OutputProtocol typeMessage;
	MessageStruct Message;
}GenericMessage;


typedef struct {
    uint8_t msgClass;
    uint8_t msgID;
    void (*get_func)(UBXMessage_parsed*, void*);
    void* structAssociate;
} MessageMapping;
extern MessageMapping message_mappings[];

void create_message_debug(UBXMessage_parsed*);

#endif /* INC_UBXPARSER_H_ */
