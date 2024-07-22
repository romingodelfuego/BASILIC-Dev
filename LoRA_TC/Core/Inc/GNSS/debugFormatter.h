/*
 * traductor.h
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#ifndef INC_TRADUCTOR_H_
#define INC_TRADUCTOR_H_

#include <GNSS/traductor.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>  // Pour size_t
#include <stdlib.h>
#include <string.h> // Pour memcpy
#include <stdarg.h>
#include <stdbool.h>
#include "UBX_types.h"

void debug_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate);
void debug_UBX_NAV_TIMEUTC(UBXMessage_parsed* UBXMessage,UBX_NAV_TIMEUTC*);
void debug_SetVal(UBXMessage_parsed* UBXMessage,UBX_CFG_SETVAL*);
void debug_PollMessage(UBXMessage_parsed* UBXMessage,UBX_CFG_MSG* structAssociate);

unsigned int bytes_to_endian(uint8_t attr[],size_t length,char type_endian);
char* array_to_hex_string(const uint8_t* array, size_t length);
void fill_unuse_memory(UBXMessage_parsed* UBXMessage,int len_use);
char* uint8ArrayToString(uint8_t* array, size_t size);
void formattedString(char* buffer, size_t* offset, int numArgs, bool freeMemory, const char* format,...);
#endif /* INC_TRADUCTOR_H_ */
