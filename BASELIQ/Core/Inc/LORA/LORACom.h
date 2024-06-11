/*
 * LORACom.h
 *
 *  Created on: Jun 7, 2024
 *      Author: romain.pace
 */

#ifndef INC_LORA_LORACOM_H_
#define INC_LORA_LORACOM_H_



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "spi.h"
#include "usart.h"

#define UART_DEBUG_BUFFER_SIZE 200

typedef struct {
	SPI_HandleTypeDef* hspi;
	UART_HandleTypeDef* huartDebug;

	uint8_t DebugBuffer[UART_DEBUG_BUFFER_SIZE];

} LORACom_HandleTypeDef;
extern LORACom_HandleTypeDef hLORACom;

void LORACom_Init(SPI_HandleTypeDef* hspi,UART_HandleTypeDef* huartDebug);
void LORA_debug(char* flag, uint8_t* value);
void LORA_debug_val(const char* flag, uint8_t value);
void RF_TestSpi( void );
void print1(const char *text, uint8_t x);
void print(const char *text);
#endif /* INC_LORA_LORACOM_H_ */
