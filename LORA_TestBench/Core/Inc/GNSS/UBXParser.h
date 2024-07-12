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
#include "debug.h"
#include "shared.h"

extern OutputProtocol protocol;

/*typedef struct {
	uint8_t msgClass;
	uint8_t msgID;
	uint16_t len;
	char bufferDebug[UART_DEBUG_BUFFER_SIZE];
	DynamicBuffer* load; // Pointeur de tableau
	DynamicBuffer* UBX_Brute; // Pointeur de tableau de longeur variable

}UBXMessage_parsed ;*/


typedef struct {
    uint8_t CLASS;
    uint8_t ID;
    void (*get_func)(UBXMessage_parsed*, void*);
    void* structAssociate;
} MessageMapping;
extern MessageMapping message_mappings[];

void create_message_debug(UBXMessage_parsed*);

#endif /* INC_UBXPARSER_H_ */
