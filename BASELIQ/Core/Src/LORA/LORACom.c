/*
 * LORACom.c
 *
 *  Created on: Jun 7, 2024
 *      Author: romain.pace
 */


#include "LORA/LORACom.h"

LORACom_HandleTypeDef hLORACom;

void LORACom_Init(SPI_HandleTypeDef* hspi,UART_HandleTypeDef* huartDebug)
{
	hLORACom.hspi = hspi;
	hLORACom.huartDebug = huartDebug;
}

void LORA_debug(char* flag, uint8_t* value)
{
	char message[50];
	if (value != NULL){
		snprintf(message, sizeof(message),"%s: %s\r\n",flag,value);
	}
	else {
		snprintf(message,sizeof(message), "%s\r\n",flag);
	}
    HAL_UART_Transmit(hLORACom.huartDebug, (uint8_t*)message, strlen(message),HAL_MAX_DELAY);
}
