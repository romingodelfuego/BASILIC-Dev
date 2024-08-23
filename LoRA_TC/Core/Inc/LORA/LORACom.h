/*
 * LORACom.h
 *
 *  Created on: Jun 7, 2024
 *      Author: romain.pace
 */

#ifndef INC_LORA_LORACOM_H_
#define INC_LORA_LORACOM_H_



#include <shared.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h> // Incluez les bibliothèques standard nécessaires
#include "spi.h"
#include "usart.h"
#include "shared.h"


typedef struct {
	SPI_HandleTypeDef* hspi;
	UART_HandleTypeDef* huartDebug;
} LORACom_HandleTypeDef;
extern LORACom_HandleTypeDef hLORACom;


void LORACom_Init(SPI_HandleTypeDef* hspi,UART_HandleTypeDef* huartDebug);

void RF_TestSpi( void );
void print1(const char *text, uint8_t x);
void print(const char *text);
#endif /* INC_LORA_LORACOM_H_ */
