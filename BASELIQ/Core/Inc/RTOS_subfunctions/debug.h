/*
 * debug.h
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#ifndef INC_RTOS_SUBFUNCTIONS_DEBUG_H_
#define INC_RTOS_SUBFUNCTIONS_DEBUG_H_

#include "RTOS_extern.h"

#define ANSI_COLOR_RED     "\033[31m"
#define ANSI_COLOR_GREEN   "\033[32m"
#define ANSI_COLOR_YELLOW  "\033[33m"
#define ANSI_COLOR_BLUE    "\033[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN    "\033[36m"
#define ANSI_COLOR_RESET   "\033[0m"

void debug(void);

void UART_Transmit_With_Color(char *data, char *color);
void uint8_array_to_hex_string(char* hexString, uint8_t* array, size_t len);
void logMemoryUsage(char* phase);
void updateMemoryUsage(void);
#endif /* INC_RTOS_SUBFUNCTIONS_DEBUG_H_ */
