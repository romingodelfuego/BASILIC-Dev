/*
 * fakeuseSD.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */
#include "RTOS_subfunctions/fakeuseSD.h"

GNSSReturnQ_t gnssReturn;
GNSStoPollQ_t pollTimeUTC = {pollUBXTimeUTC, sizeof(pollUBXTimeUTC),"SD_POLLING"};

void fakeuseSD(void){
	GNSSRequestQ_t requestFromSD = {
			.Request_TIME = xTaskGetTickCount(),
			.CLASS = 0x01,
			.ID = 0x21,
			.applicantSemaphore = SD_Access_GNSS_ReturnHandle,
			.applicantName = "SD_REQUEST"
	};

	xQueueSendToBack(GNSS_RequestHandle,&requestFromSD,osWaitForever);
	osSemaphoreWait(SD_Access_GNSS_ReturnHandle, osWaitForever); //On verrouille la semaphore
	UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...\r\n",ANSI_COLOR_YELLOW);
	ITM_Port32(29)=111;

	request_commandToGNSS(pollTimeUTC);

	ITM_Port32(29)=444;

	osStatus eventSD = osSemaphoreWait(SD_Access_GNSS_ReturnHandle, osWaitForever);
	ITM_Port32(29)=555;

	if (eventSD != osOK){
		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...",ANSI_COLOR_YELLOW);
		UART_Transmit_With_Color("\t---SEMAPHORE ISSUE---\r\n\n",ANSI_COLOR_RED);
		return;
	}
	xQueueReceive(GNSS_ReturnHandle, &gnssReturn, osWaitForever);
	ITM_Port32(29)=666;

	if (gnssReturn.statut != OK)
	{
		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...",ANSI_COLOR_YELLOW);
		UART_Transmit_With_Color("\t---NOT FOUND---\r\n",ANSI_COLOR_RED);
	}


	if (gnssReturn.bufferReturn->size<= 512){
		char* hexString_SD = (char*)pvPortMalloc(gnssReturn.bufferReturn->size * 2 + 1);
		if (hexString_SD == NULL) Error_Handler();

		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --SEND-- SD Polling...",ANSI_COLOR_YELLOW);
		UART_Transmit_With_Color("\t---SUCCESS---\r\n",ANSI_COLOR_GREEN);
		uint8_array_to_hex_string(hexString_SD, gnssReturn.bufferReturn->buffer, gnssReturn.bufferReturn->size);
		UART_Transmit_With_Color(hexString_SD,ANSI_COLOR_YELLOW);

		vPortFree(hexString_SD);
	}

	else{
		char* len = (char*)pvPortMalloc(sizeof(TickType_t) * sizeof(char));
		if (len == NULL) Error_Handler();

		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --SEND-- SD Polling --TOO LONG FOR DEBUG...\t",ANSI_COLOR_MAGENTA);
		sprintf(len, "%u",gnssReturn.bufferReturn->size);
		UART_Transmit_With_Color(len, ANSI_COLOR_RED);
		vPortFree(len);
	}
	freeBuffer(gnssReturn.itemFromUBX_Q.UBXMessage->load);
	freeBuffer(gnssReturn.itemFromUBX_Q.UBXMessage->brute);
	vPortFree(gnssReturn.itemFromUBX_Q.UBXMessage);
	//vPortFree(&gnssReturn);

	osSemaphoreRelease(SD_Access_GNSS_ReturnHandle);

}

