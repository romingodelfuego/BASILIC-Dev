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
			.CLASS = 0x01,
			.ID = 0x21,
			.applicantSemaphore = SD_Access_GNSS_ReturnHandle,
			.applicantName = "SD_REQUEST"};
	if (uxQueueMessagesWaiting(GNSS_RequestHandle)>0){
		ITM_Port32(31)=3000001; //Debug Purpose
	}
	xQueueSendToBack(GNSS_RequestHandle,&requestFromSD,osWaitForever);
	UART_Transmit_With_Color(hGNSSCom.huartDebug,"\r\t\t\n...UBXMessage --FROM-- SD Polling...\r\n",ANSI_COLOR_YELLOW);
	ITM_Port32(29)=111;

	request_commandToGNSS(pollTimeUTC);
	ITM_Port32(29)=444;
	int32_t eventSD = osSemaphoreWait(SD_Access_GNSS_ReturnHandle, 100);
	if (eventSD == osOK){
		xQueueReceive(GNSS_ReturnHandle, &gnssReturn, portMAX_DELAY);
		ITM_Port32(29)=555;

		if (gnssReturn.statut == OK){
			UART_Transmit_With_Color(hGNSSCom.huartDebug,"\n\r\t\t...UBXMessage --SEND-- SD Polling...",ANSI_COLOR_YELLOW);
			UART_Transmit_With_Color(hGNSSCom.huartDebug,"\t---SUCCESS---\r\n",ANSI_COLOR_GREEN);
			char* hexString = uint8_array_to_hex_string(gnssReturn.bufferReturn->buffer, gnssReturn.bufferReturn->size);
			UART_Transmit_With_Color(hGNSSCom.huartDebug,hexString,ANSI_COLOR_YELLOW);
			vPortFree(hexString);
		}
		else{
			UART_Transmit_With_Color(hGNSSCom.huartDebug,"\r\t\t\n...UBXMessage --FROM-- SD Polling...",ANSI_COLOR_YELLOW);
			UART_Transmit_With_Color(hGNSSCom.huartDebug,"\t---NOT FOUND---\r\n",ANSI_COLOR_RED);
		}

	}
	else {
		UART_Transmit_With_Color(hGNSSCom.huartDebug,"\r\t\t\n...UBXMessage --FROM-- SD Polling...",ANSI_COLOR_YELLOW);
		UART_Transmit_With_Color(hGNSSCom.huartDebug,"\t---SEMAPHORE ISSUE---\r\n\n",ANSI_COLOR_RED);
	}
	osDelay(1000);

}

