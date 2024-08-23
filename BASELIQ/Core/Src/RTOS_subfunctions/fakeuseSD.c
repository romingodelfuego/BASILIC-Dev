/*
 * fakeuseSD.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */
#include "RTOS_subfunctions/fakeuseSD.h"

GNSSReturnQ_t gnssReturn;
GNSStoPollQ_t pollTimeUTC = {pollUBXTimeUTC, sizeof(pollUBXTimeUTC),"SD_POLLING"};
/************************ TASK ************************/
void fakeuseSD(void){
	logMemoryUsage("START - fakeUseSD");

	GNSSRequestQ_t requestFromSD = {
			.Request_TIME = xTaskGetTickCount(),
			.CLASS = 0x01,
			.ID = 0x21,
			.applicantSemaphore = SD_Access_GNSS_ReturnHandle,
			.applicantName = "SD_REQUEST"
	};

	xQueueSendToBack(GNSS_RequestHandle,&requestFromSD,osWaitForever);			// Sending the request for accessing to GNSS uart
	osSemaphoreWait(SD_Access_GNSS_ReturnHandle, osWaitForever); 				//Locking the semaphore
	UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...\r\n",ANSI_COLOR_YELLOW);

	// Sending the request for GNSS
	request_commandToGNSS(pollTimeUTC);

	osStatus eventSD = osSemaphoreWait(SD_Access_GNSS_ReturnHandle, osWaitForever); 	//Waiting, the sempahore is free by matcher.c
	if (eventSD != osOK){
		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...",ANSI_COLOR_YELLOW);
		UART_Transmit_With_Color("\t---SEMAPHORE ISSUE---\r\n\n",ANSI_COLOR_RED);
		return;
	}
	xQueueReceive(GNSS_ReturnHandle, &gnssReturn, osWaitForever);						// Reception of the response from the GNSS

	if (gnssReturn.statut != OK)
	{
		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- SD Polling...",ANSI_COLOR_YELLOW);
		UART_Transmit_With_Color("\t---NOT FOUND---\r\n",ANSI_COLOR_RED);
	}


	if (gnssReturn.bufferReturn->size<= 512){										// Checking for debugging the message or not
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

		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --SEND-- SD Polling --TOO LONG FOR DEBUG...\t",ANSI_COLOR_YELLOW);
		sprintf(len, "%u",gnssReturn.bufferReturn->size);
		UART_Transmit_With_Color(len, ANSI_COLOR_RED);
		vPortFree(len);
	}

	// Reinitialisation de la trame
	freeBuffer(gnssReturn.bufferReturn);
	vPortFree(gnssReturn.UBXMessage);
	logMemoryUsage("END - fakeUseSD");

	osSemaphoreRelease(SD_Access_GNSS_ReturnHandle);	//Unlocking the semaphore

}
/************************ ---- ************************/
