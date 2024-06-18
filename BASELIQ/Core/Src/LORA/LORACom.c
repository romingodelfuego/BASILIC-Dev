/*
 * LORACom.c
 *
 *  Created on: Jun 7, 2024
 *      Author: romain.pace
 */


#include "LORA/LORACom.h"
#include "LORA/RFM9x.h"
LORACom_HandleTypeDef hLORACom;

void LORACom_Init(SPI_HandleTypeDef* hspi,UART_HandleTypeDef* huartDebug)
{
	hLORACom.hspi = hspi;
	hLORACom.huartDebug = huartDebug;

	LORACom_SPIActivate(&hLORACom);
}
void LORACom_SPIActivate(LORACom_HandleTypeDef* hLORA){
	HAL_SPI_Receive_IT(hLORA->hspi, hLORA->RxBuffer, MAX_SPI_SIZE);
}
void LORA_Send(Header* header, uint8_t* payload){
    uint8_t buffer[RFM9x_FIFO_SIZE];

    // Ajouter l'en-tête
    buffer[0] = header->recipient;
    buffer[1] = header->sender;
    buffer[2] = header->type;
    buffer[3] = header->len_payload;

    // Ajouter la charge utile
    memcpy(buffer + 4 , payload, header->len_payload);
    RFM9x_Send(buffer, header->len_payload + 4);
    RFM9x_SetMode_Receive();
}


void LORA_debug(char* flag, uint8_t* value)
{
	char message[200];
	if (value != NULL){
		snprintf(message, sizeof(message),"%s: %s\r\n",flag,value);
	}
	else {
		snprintf(message,sizeof(message), "%s\r\n",flag);
	}
    HAL_UART_Transmit(hLORACom.huartDebug, (uint8_t*)message, strlen(message),HAL_MAX_DELAY);
}
void LORA_debug_hexa(char* flag, uint8_t* value, uint8_t len)
{
	char message[50];
    char hexString[len*2+1]; // Buffer to hold the hex string (2 characters per byte)
	if (value != NULL){
		for (int i = 0; i < len; i++) {
		            snprintf(hexString + (i * 2), sizeof(hexString) - (i * 2), "%02X", value[i]);
		        }
		snprintf(message, sizeof(message),"%s: %s\r\n",flag,hexString);
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
