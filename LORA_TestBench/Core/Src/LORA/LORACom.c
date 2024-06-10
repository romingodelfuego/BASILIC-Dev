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

	if (value != NULL || *value==0){
		snprintf(message, sizeof(message),"%s: %02X\r\n",
				flag,
				(*value == '\0') ? 0 : *value);
	}
	else {
		snprintf(message,sizeof(message), "%s\r\n",flag);
	}
    HAL_UART_Transmit(hLORACom.huartDebug, (uint8_t*)message, strlen(message),HAL_MAX_DELAY);
}
void LORA_debug_val(const char* flag, uint8_t value)
{
    char message[50];
    snprintf(message, sizeof(message), "%s: %02X\r\n", flag, (value == '\0') ? 0 : value);
    HAL_UART_Transmit(hLORACom.huartDebug, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}
