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

typedef enum {
    WAIT_FOR_SYNC_1,
    WAIT_FOR_SYNC_2,
	WAIT_FOR_CLASS,
	WAIT_FOR_ID,
    WAIT_FOR_LENGTH_1,
    WAIT_FOR_LENGTH_2,
    RECEIVE_MESSAGE
} UARTState;


typedef struct {
    uint8_t class;
    uint8_t ID;
    void (*get_func)(UBXMessage_parsed*, void*);
    void* structAssociate;
} MessageMapping;
extern MessageMapping message_mappings[];

void create_message_debug(UBXMessage_parsed*);

#endif /* INC_UBXPARSER_H_ */
