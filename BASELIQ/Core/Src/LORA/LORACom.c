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
}

void LORA_Send(LORA_Header* header, uint8_t* payload){
	uint8_t* buffer = (uint8_t*)pvPortMalloc(RFM9x_FIFO_SIZE * sizeof(uint8_t));
	if(buffer == NULL) Error_Handler();
    // Ajouter l'en-tête
    buffer[0] = header->recipient;
    buffer[1] = header->sender;
    buffer[2] = header->type;
    buffer[3] = header->len_payload;

    // Ajouter la charge utile
    memcpy(buffer + 4 , payload, header->len_payload);
    RFM9x_Send(buffer, header->len_payload + 4);
    vPortFree(buffer);
    RFM9x_SetMode_Receive();
}

