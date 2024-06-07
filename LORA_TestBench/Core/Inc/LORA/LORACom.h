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

#endif /* INC_LORA_LORACOM_H_ */
