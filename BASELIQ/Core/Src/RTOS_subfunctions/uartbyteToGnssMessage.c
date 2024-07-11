/*
 * uartbyteToGnssMessage.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include "RTOS_subfunctions/uartbyteToGnssMessage.h"

UARTMessageQ_t uartMsg;
UARTState state = WAIT_FOR_SYNC_1;
uint16_t payloadIndex = 0;

UBXMessage_parsed* UBXMessage = NULL;

/************************ TASK ************************/
void uartbyteToGnssMessage(void){
	if (xQueueReceive(UARTbyteHandle, &uartMsg, portMAX_DELAY) == pdTRUE) { //On recoit un byte d'un ISR de lhuart du GNSS
		uint8_t receivedByte = uartMsg.data;
		/*//Eviter les problemes de state qui sont malheureusement present
	    if (state < WAIT_FOR_SYNC_1 || state > RECEIVE_MESSAGE) state = WAIT_FOR_SYNC_1;*/
		// Machine à états pour traiter les messages
		switch (state) {
		case WAIT_FOR_SYNC_1:
			if (receivedByte == HEADER_UBX_1) {
				osSemaphoreWait(GNSS_UART_AccessHandle, osWaitForever); //On empeche l'utilisation du TX
				state = WAIT_FOR_SYNC_2;
			}
			break;

		case WAIT_FOR_SYNC_2:
			ITM_Port32(31)=1111;
			if (receivedByte == HEADER_UBX_2) state = WAIT_FOR_CLASS;
			else{
				state = WAIT_FOR_SYNC_1;
				osSemaphoreRelease(GNSS_UART_AccessHandle);}
			break;

		case WAIT_FOR_CLASS:
			if(UBXMessage != NULL) Error_Handler();
			logMemoryUsage("BEFORE - UBXMessage - PortMalloc");
			UBXMessage = (UBXMessage_parsed*) pvPortMalloc(sizeof(UBXMessage_parsed)); //UBXMessage utilise une partie de la memoire HEAP
			if (UBXMessage == NULL) Error_Handler();
			logMemoryUsage("AFTER - UBXMessage - PortMalloc");
			UBXMessage->CLASS = receivedByte;
			state = WAIT_FOR_ID;
			break;

		case WAIT_FOR_ID:
			UBXMessage->ID = receivedByte;
			state = WAIT_FOR_LENGTH_1;
			break;

		case WAIT_FOR_LENGTH_1:
			UBXMessage->len_payload = receivedByte;
			state = WAIT_FOR_LENGTH_2;
			break;

		case WAIT_FOR_LENGTH_2:
			ITM_Port32(31)=6666;
			UBXMessage->len_payload |= receivedByte << 8;
			if (UBXMessage->len_payload == 0 || UBXMessage->len_payload > UART_MAX_BUFFER_SIZE) Error_Handler(); //On checke que le message ne dépasse pas une certaine longueur
			ITM_Port32(31)=UBXMessage->len_payload;
			logMemoryUsage("BEFORE - UBXMessage->brute - PortMalloc");
			UBXMessage->brute = initializeBuffer(UBXMessage->len_payload + 8); //On fait pointer un buffer sur une partie protege de la mémoire HEAP
			if (UBXMessage->brute == NULL) Error_Handler();
			logMemoryUsage("AFTER - UBXMessage->brute - PortMalloc");
			//On rempli un de nos buffer
			UBXMessage->brute->buffer[0]=0xb5;
			UBXMessage->brute->buffer[1]=0x62;
			UBXMessage->brute->buffer[2]=UBXMessage->CLASS;
			UBXMessage->brute->buffer[3]=UBXMessage->ID;
			UBXMessage->brute->buffer[4]=UBXMessage->len_payload & 0x00FF;
			UBXMessage->brute->buffer[5]=UBXMessage->len_payload >> 8;
			//----//
			payloadIndex = 0;
			state = RECEIVE_MESSAGE;
			break;

		case RECEIVE_MESSAGE:
			//ITM_Port32(31)=payloadIndex;

			if (payloadIndex <= UBXMessage->len_payload + 2) UBXMessage->brute->buffer[6 + payloadIndex] = receivedByte;
			payloadIndex++ ;
			if  (payloadIndex == UBXMessage->len_payload + 2){

				UBXMessageQ_t gnssMsg_container = (UBXMessageQ_t){ .UBXMessage = UBXMessage };
				if (xQueueSendToBack(UBXQueueHandle, &gnssMsg_container, portMAX_DELAY) != pdTRUE) Error_Handler();

				UART_Transmit_With_Color("\r\n...[INFO] UARTByte --SendQueue-- SUCCESS...\r\n",ANSI_COLOR_RESET);
				ITM_Port32(31)=9999;

				//Reinitisalition de la machine a état
				UBXMessage = NULL;
				state = WAIT_FOR_SYNC_1;
				osSemaphoreRelease(GNSS_UART_AccessHandle); //On reautorise le TX
			}
			break;
		default:
			state = WAIT_FOR_SYNC_1;
			break;
		}
	}
}
/************************ ---- ************************/

