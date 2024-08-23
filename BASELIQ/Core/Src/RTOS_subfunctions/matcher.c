/*
 * matcher.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include "RTOS_subfunctions/matcher.h"

/************************ TASK ************************/
void matcher(void){
	GNSSRequestQ_t gnssRequest;
	UBXMessageQ_t gnssMsg_container;
	xQueueReceive(GNSS_RequestHandle, &gnssRequest, osWaitForever);

	UART_Transmit_With_Color( "\r\n...[INFO] Semaphore in MATCHER...--TAKE--\t\t", ANSI_COLOR_RESET);
	UART_Transmit_With_Color(gnssRequest.applicantName, ANSI_COLOR_RESET);
	//Parcourir UBXQueue
	//Matcher avec Class et ID --> sortir le payload
	//UBXMessage_parsed* ubxFromQueueMatching = findItemQueueWithoutCopy(UBXQueueHandle, gnssRequest);
	//UBXMessage_parsed* ubxFromQueueMatching = findAndRemoveItemFromQueue(UBXQueueHandle, gnssRequest);

	if (xQueueReceive(UBXQueueHandle,&gnssMsg_container,osWaitForever)==pdPASS){
		//itemFromUBX est pvPortMalloc dans UartByte il faut liberer l'espace un moment
		///////
		//GNSSReturnQ_t* gnssReturn = (GNSSReturnQ_t*)pvPortMalloc(sizeof(GNSSReturnQ_t));
		//if (gnssReturn == NULL) Error_Handler();

		GNSSReturnQ_t gnssReturn = (GNSSReturnQ_t){ //Probleme de reecriture puisque non protégée
			.Request_TIME = gnssRequest.Request_TIME,
					.Return_TIME = xTaskGetTickCount(),
					.statut = OK,
					.CLASS = gnssRequest.CLASS,
					.ID = gnssRequest.ID,
					.UBXMessage = gnssMsg_container.UBXMessage,
					.bufferReturn = gnssMsg_container.UBXMessage->brute,
					//bufferReturn est pvPortMalloc dans UartByte il faut liberer l'espace un moment
					.applicantName = gnssRequest.applicantName
		};

		char* TIME_delta = (char*)pvPortMalloc(sizeof(TickType_t) * sizeof(char));
		if(TIME_delta == NULL) Error_Handler();

		UART_Transmit_With_Color( "\n\r...[INFO] Semaphore in MATCHER...--RELEASE--\t\t", ANSI_COLOR_RESET);
		UART_Transmit_With_Color( gnssRequest.applicantName, ANSI_COLOR_RESET);

		sprintf(TIME_delta, "%lu",((gnssReturn.Return_TIME-gnssReturn.Request_TIME)*(uint32_t)(1000.000/configTICK_RATE_HZ)));
		UART_Transmit_With_Color( "\n\r...[INFO] Delta Time: Request & Return (ms)...\t\t", ANSI_COLOR_RESET);
		UART_Transmit_With_Color(TIME_delta,ANSI_COLOR_GREEN);

		vPortFree(TIME_delta); //UART Transmit color stocke toutes les info messages ainsi que couleur

		if(xQueueSendToBack(GNSS_ReturnHandle,&gnssReturn,portMAX_DELAY)!=pdTRUE) Error_Handler();

		osSemaphoreRelease(gnssRequest.applicantSemaphore);
	}else
	{		UART_Transmit_With_Color( "\n\r...[INFO] ERROR...\t\t", ANSI_COLOR_RED);}
}
/************************ ---- ************************/
/************************ FUNCTIONS ************************/
UBXMessage_parsed* findItemQueueWithoutCopy(osMessageQId xQueue, GNSSRequestQ_t gnssRequest)
{
	UBXMessage_parsed* foundItem = NULL;
	UBXMessageQ_t item ;
	UBaseType_t queueSize = uxQueueMessagesWaiting(xQueue);
	for (BaseType_t index = 0; index < queueSize; index++){
		// Peek at the current item in the queue
		if (xQueueReceive(xQueue, &item, 100) == pdPASS) {
			if (isItemMatching(&item, gnssRequest)) {
				// Copy the found item
				foundItem = pvPortMalloc(sizeof(UBXMessageQ_t));
				if (foundItem != NULL) {
					foundItem = item.UBXMessage;
				}
				break;
			}
		}
	}
	return foundItem;
}

UBXMessage_parsed* findAndRemoveItemFromQueue(osMessageQId xQueue, GNSSRequestQ_t gnssRequest){
	UBXMessage_parsed* foundItem = NULL;
	UBXMessageQ_t currentItem ;
	UBaseType_t queueSize = uxQueueMessagesWaiting(xQueue);

	for (BaseType_t index = 0; index < queueSize; index++){
		if (xQueueReceive(xQueue, &currentItem, 100) == pdPASS) {
			if (isItemMatching(&currentItem, gnssRequest) && foundItem == NULL) {
				// Copy the found item
				foundItem = pvPortMalloc(sizeof(UBXMessageQ_t));
				foundItem = currentItem.UBXMessage;
			}
			else {
				xQueueSendToFront(xQueue, &currentItem, 0);
			}
		}
	}
	return foundItem;
}

int isItemMatching(UBXMessageQ_t* item,  GNSSRequestQ_t itemTarget){
	UBXMessage_parsed* messageinQueue = item->UBXMessage;
	return messageinQueue->CLASS == itemTarget.CLASS && messageinQueue->ID == itemTarget.ID;
}
/************************ -------- ************************/
