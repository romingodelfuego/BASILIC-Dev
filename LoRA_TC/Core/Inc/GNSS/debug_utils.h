/*
 * debug_utils.h
 *
 *  Created on: Jul 23, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSS_DEBUG_UTILS_H_
#define INC_GNSS_DEBUG_UTILS_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>  // Pour size_t
#include <stdlib.h>
#include <string.h> // Pour memcpy
#include <stdarg.h>
#include <stdbool.h>

#include <GNSS/traductor.h>

#define MAX_BUFFER_SIZE 512

unsigned int bytes_to_endian(uint8_t attr[],size_t length,char type_endian);
char* array_to_hex_string(const uint8_t* array, size_t length);
void fill_unuse_memory(UBXMessage_parsed* UBXMessage,int len_use);
char* uint8ArrayToString(uint8_t* array, size_t size);
char* int8ArrayToString(int8_t* array, size_t size);
void formattedString(char* buffer, size_t* offset, int numArgs, bool freeMemory,bool isThereSpecFormat, const char* format,...);


#endif /* INC_GNSS_DEBUG_UTILS_H_ */
