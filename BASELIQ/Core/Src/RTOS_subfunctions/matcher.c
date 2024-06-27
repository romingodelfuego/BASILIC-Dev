/*
 * matcher.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include "RTOS_subfunctions/matcher.h"
GNSSRequestQ_t gnssRequest;

void matcher(void){
	if(uxQueueMessagesWaiting(UBXQueueHandle)>0){
		UBXMessageQ_t item;
		UBXMessage_parsed* ubxFromQueueMatching=NULL;
		xQueueReceive(GNSS_RequestHandle, &gnssRequest, osWaitForever);

		UART_Transmit_With_Color(hGNSSCom.huartDebug, "\r...[INFO] Semaphore in MATCHER...--TAKE--\t\t", ANSI_COLOR_RESET);
		UART_Transmit_With_Color(hGNSSCom.huartDebug, gnssRequest.applicantName, ANSI_COLOR_RESET);
		//Parcourir UBXQueue
		//Matcher avec Class et ID --> sortir le payload
		//UBXMessage_parsed* ubxFromQueueMatching = findItemQueueWithoutCopy(UBXQueueHandle, gnssRequest);
		//UBXMessage_parsed* ubxFromQueueMatching = findAndRemoveItemFromQueue(UBXQueueHandle, gnssRequest);
		xQueueReceive(UBXQueueHandle,&item,100);
		ubxFromQueueMatching = item.receptionGNSS->Message.UBXMessage;
		///////

		if (ubxFromQueueMatching != NULL){
			GNSSReturnQ_t gnssReturn_correct = {
					.statut = OK,
					.CLASS = gnssRequest.CLASS,
					.ID = gnssRequest.ID,
					.bufferReturn = ubxFromQueueMatching->brute,
					.applicantName = gnssRequest.applicantName
			};
			xQueueSendToBack(GNSS_ReturnHandle,&gnssReturn_correct,portMAX_DELAY);
			//vPortFree(ubxFromQueueMatching);

		}else{
			UART_Transmit_With_Color(hGNSSCom.huartDebug,"\r\t\t\n...MATCHER... ---FAILED---\t\t",ANSI_COLOR_RED);
			UART_Transmit_With_Color(hGNSSCom.huartDebug,gnssRequest.applicantName,ANSI_COLOR_RED);

			GNSSReturnQ_t gnssReturn_erreur = {
					.statut = Error,
					.CLASS = 0,
					.ID = 0,
					.bufferReturn = (DynamicBuffer*)NULL,
					.applicantName = gnssRequest.applicantName};
			xQueueSendToBack(GNSS_ReturnHandle,&gnssReturn_erreur,portMAX_DELAY);
		}
		osSemaphoreRelease(gnssRequest.applicantSemaphore);
		UART_Transmit_With_Color(hGNSSCom.huartDebug, "\n\r...[INFO] Semaphore in MATCHER...--RELEASE--\t\t", ANSI_COLOR_RESET);
		UART_Transmit_With_Color(hGNSSCom.huartDebug, gnssRequest.applicantName, ANSI_COLOR_RESET);
	}

}
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
					foundItem = item.receptionGNSS->Message.UBXMessage;
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
				foundItem = currentItem.receptionGNSS->Message.UBXMessage;
			}
			else {
				xQueueSendToFront(xQueue, &currentItem, 0);
			}
		}
	}
	return foundItem;
}
int isItemMatching(UBXMessageQ_t* item,  GNSSRequestQ_t itemTarget){
	UBXMessage_parsed* messageinQueue = item->receptionGNSS->Message.UBXMessage;
	return messageinQueue->CLASS == itemTarget.CLASS && messageinQueue->ID == itemTarget.ID;
}
