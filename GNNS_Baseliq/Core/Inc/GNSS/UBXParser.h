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
#include "constants.h"

typedef struct {
	uint8_t msgClass;
	uint8_t msgID;
	uint16_t len;
	uint8_t load[UART_DEBUG_BUFFER_SIZE]; // Pointeur de tableau
	char bufferDebug[UART_DEBUG_BUFFER_SIZE];
}UBXMessage_parsed ;

typedef struct {
    uint8_t msgClass;
    uint8_t msgID;
    void (*get_func)(UBXMessage_parsed*, void*);
    void* structAssociate;
} MessageMapping;
extern MessageMapping message_mappings[];

void create_message_debug(UBXMessage_parsed*);

#endif /* INC_UBXPARSER_H_ */
