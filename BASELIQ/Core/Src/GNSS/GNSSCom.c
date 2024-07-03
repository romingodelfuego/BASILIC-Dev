/*
 * GNSSCom.c
 *
 *  Created on: May 23, 2024
 *      Author: romain.pace
 */
#include <GNSS/GNSSCom.h>
#include "LORA/RFM9x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "RTOS_subfunctions/RTOS_extern.h"
#include "RTOS_subfunctions/commandToGNSS.h"

GNSSCom_HandleTypeDef hGNSSCom;
OutputType type = ASCII;
OutputProtocol protocol = UBX;

void GNSSCom_Init(UART_HandleTypeDef* huart,UART_HandleTypeDef* huartDebug){
	hGNSSCom.huart = huart;
	hGNSSCom.huartDebug = huartDebug;
	hGNSSCom.Rx = initializeBuffer(UART_RX_BUFFER_SIZE);
	GNSSCom_UartActivate(&hGNSSCom);
}

DynamicBuffer* initializeBuffer(size_t initialSize) {
	DynamicBuffer *bufferDynamic = (DynamicBuffer *)pvPortMalloc(sizeof(DynamicBuffer));
	if (bufferDynamic == NULL) Error_Handler();

	bufferDynamic->buffer = (uint8_t*)pvPortMalloc(initialSize);
	if (bufferDynamic->buffer == NULL) Error_Handler();

	bufferDynamic->size = initialSize;
	return bufferDynamic;
}

void freeBuffer(DynamicBuffer *bufferDynamic) {
	vPortFree(bufferDynamic->buffer);
	vPortFree(bufferDynamic);
}
void GNSSCom_Send_SetVal(CommandnSize toTransmit){
	while (hGNSSCom.huart->gState != HAL_UART_STATE_READY){ITM_Port32(30)=99999999;}
	HAL_StatusTypeDef statut = HAL_UART_Transmit(hGNSSCom.huart, toTransmit.command, toTransmit.size,HAL_MAX_DELAY);
	if (statut!= HAL_OK) Error_Handler();
}
void GNSSCom_SetUp_Init(void){
	GNSStoPollQ_t commands[] = {

			{commandSetGNSS_Config, sizeof(commandSetGNSS_Config)},
			{commandSetTP1_atNVTRate,sizeof(commandSetTP1_atNVTRate)},
			{commandSetTP2, sizeof(commandSetTP2)},
			{commandMeasureRate, sizeof(commandMeasureRate)},
			{commandUart1Ouput, sizeof(commandUart1Ouput)},
			//{commandUBXTimeUTC, sizeof(commandUBXTimeUTC)}
	};


	char message[50];
	for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i){
		sprintf(message, "\r\t\t\n %s%s%d...%s \r\n",ANSI_COLOR_RESET,"...UBXMessage", i + 1,ANSI_COLOR_RESET);
		HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message,strlen(message),HAL_MAX_DELAY);

		HAL_UART_Transmit(hGNSSCom.huart,commands[i].command,commands[i].size,HAL_MAX_DELAY);

		sprintf(message, "\r\t\t\n %s%s%s \r\n",ANSI_COLOR_RESET,"...UBXMessage --FROM-- INIT...",ANSI_COLOR_RESET);
		HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message,strlen(message),HAL_MAX_DELAY);

		sprintf(message, "\r\t\t\n %s%s%s \r\n",ANSI_COLOR_GREEN,"\t---SUCCESS---\r\n",ANSI_COLOR_RESET);
		HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message,strlen(message),HAL_MAX_DELAY);

		HAL_Delay(500);
		ITM_Port32(31)=1;
	}
}

void GNSSCom_UartActivate(GNSSCom_HandleTypeDef* hGNSS){
	HAL_UART_Receive_IT(hGNSS->huart, hGNSS->Rx->buffer, hGNSS->Rx->size);
}
/*

*/
