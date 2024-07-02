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

	GNSSCom_SetUp_Init();
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
	GNSSReturnQ_t gnssReturn;

	GNSStoPollQ_t commands[] = {
			{commandUart1Ouput, sizeof(commandUart1Ouput)},
			{commandSetGNSS_Config, sizeof(commandSetGNSS_Config)},
			{commandSetTP1_atNVTRate,sizeof(commandSetTP1_atNVTRate)},
			{commandSetTP2, sizeof(commandSetTP2)},
			{commandMeasureRate, sizeof(commandMeasureRate)},

			//{commandUBXTimeUTC, sizeof(commandUBXTimeUTC)}
	};
	GNSSRequestQ_t requestFromSD = {
			.Request_TIME = xTaskGetTickCount(),
			.CLASS = 0x05,
			.ID = 0x01,
			.applicantSemaphore = xSem_GNSS_InitHandle,
			.applicantName = "GNSS_INIT"
	};
	__enable_irq();
	osDelay(100);
	xQueueSendToBack(GNSS_RequestHandle,&requestFromSD,osWaitForever);
	char message[50];
	for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i){
		sprintf(message, "\r\t\t\n...UBXMessage%d...\r\n", i + 1);
		UART_Transmit_With_Color(message,ANSI_COLOR_RESET);
		request_commandToGNSS(commands[i]);
		osSemaphoreWait(xSem_GNSS_InitHandle, osWaitForever);
		xQueueReceive(GNSS_ReturnHandle, &gnssReturn, osWaitForever);
		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- INIT...",ANSI_COLOR_RESET);
		UART_Transmit_With_Color("\t---SUCCESS---\r\n",ANSI_COLOR_GREEN);
	}
}


