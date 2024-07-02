/*
 * fakeuseSD.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */
#include "RTOS_subfunctions/fakeuseSD.h"

GNSSReturnQ_t gnssReturn;
GNSStoPollQ_t pollTimeUTC = {pollUBXTimeUTC, sizeof(pollUBXTimeUTC)};

void fakeuseSD(void){
	GNSSRequestQ_t requestFromSD = {
			.Request_TIME = xTaskGetTickCount(),
			.CLASS = 0x01,
			.ID = 0x21,
			.applicantSemaphore = SD_Access_GNSS_ReturnHandle,
			.applicantName = "SD_REQUEST"
	};


	xQueueSendToBack(GNSS_RequestHandle,&requestFromSD,osWaitForever);
	UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...\r\n",ANSI_COLOR_YELLOW);

	ITM_Port32(29)=111;

	request_commandToGNSS(pollTimeUTC);

	ITM_Port32(29)=444;

	int32_t eventSD = osSemaphoreWait(SD_Access_GNSS_ReturnHandle, 100);

	if (eventSD == osOK){
		xQueueReceive(GNSS_ReturnHandle, &gnssReturn, portMAX_DELAY);
		ITM_Port32(29)=555;
		char* hexString_SD = (char*)pvPortMalloc(gnssReturn.bufferReturn->size * 2 + 1);
		if (hexString_SD == NULL) Error_Handler();

		if (gnssReturn.statut == OK)
		{
			UART_Transmit_With_Color("\n\r\t\t...UBXMessage --SEND-- SD Polling...",ANSI_COLOR_YELLOW);
			UART_Transmit_With_Color("\t---SUCCESS---\r\n",ANSI_COLOR_GREEN);
			uint8_array_to_hex_string(hexString_SD, gnssReturn.bufferReturn->buffer, gnssReturn.bufferReturn->size);
			UART_Transmit_With_Color(hexString_SD,ANSI_COLOR_YELLOW);
		}
		else
		{
			UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...",ANSI_COLOR_YELLOW);
			UART_Transmit_With_Color("\t---NOT FOUND---\r\n",ANSI_COLOR_RED);
		}
		vPortFree(hexString_SD);
		freeBuffer(gnssReturn.itemFromUBX_Q.UBXMessage->load);
		freeBuffer(gnssReturn.itemFromUBX_Q.UBXMessage->brute);
		vPortFree(gnssReturn.itemFromUBX_Q.UBXMessage);
	}
	else
	{
		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...",ANSI_COLOR_YELLOW);
		UART_Transmit_With_Color("\t---SEMAPHORE ISSUE---\r\n\n",ANSI_COLOR_RED);
	}
}

