/*
 * uartbyteToGnssMessage.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include "RTOS_subfunctions/uartbyteToGnssMessage.h"

UARTMessageQ_t uartMsg;
UARTState state;
uint16_t payloadIndex = 0;

UBXMessage_parsed* messageUBX = NULL;

/************************ TASK ************************/
void uartbyteToGnssMessage(void){
	if (xQueueReceive(UARTbyteHandle, &uartMsg, portMAX_DELAY) == pdTRUE) { //On recoit un byte d'un ISR de lhuart du GNSS
		uint8_t receivedByte = uartMsg.data;

		// Machine à états pour traiter les messages
		switch (state) {
		case WAIT_FOR_SYNC_1:
			ITM_Port32(31)=1111;
			if (receivedByte == HEADER_UBX_1) {
				osSemaphoreWait(GNSS_UART_AccessHandle, osWaitForever); //On empeche l'utilisation du TX
				state = WAIT_FOR_SYNC_2;
			}
			break;

		case WAIT_FOR_SYNC_2:
			if (receivedByte == HEADER_UBX_2) state = WAIT_FOR_CLASS;
			else{
			state = WAIT_FOR_SYNC_1;
			osSemaphoreRelease(GNSS_UART_AccessHandle);}
			break;

		case WAIT_FOR_CLASS:
			if(messageUBX != NULL) Error_Handler();
			messageUBX = (UBXMessage_parsed*) pvPortMalloc(sizeof(UBXMessage_parsed)); //messageUBX utilise une partie de la memoire HEAP
			if (messageUBX == NULL) Error_Handler();
			messageUBX->CLASS = receivedByte;
			state = WAIT_FOR_ID;
			break;

		case WAIT_FOR_ID:
			messageUBX->ID = receivedByte;
			state = WAIT_FOR_LENGTH_1;
			break;

		case WAIT_FOR_LENGTH_1:
			messageUBX->len_payload = receivedByte;
			state = WAIT_FOR_LENGTH_2;
			break;

		case WAIT_FOR_LENGTH_2:
			ITM_Port32(31)=6666;
			messageUBX->len_payload |= receivedByte << 8;
			if (messageUBX->len_payload == 0 || messageUBX->len_payload > UART_MAX_BUFFER_SIZE) Error_Handler(); //On checke que le message ne dépasse pas une certaine longueur

			messageUBX->load = initializeBuffer(messageUBX->len_payload); //On fait pointer un buffer sur une partie protege de la mémoire HEAP
			messageUBX->brute = initializeBuffer(messageUBX->len_payload + 8); //On fait pointer un buffer sur une partie protege de la mémoire HEAP
			if (messageUBX->load == NULL || messageUBX->brute == NULL) Error_Handler();

			//On rempli un de nos buffer
			messageUBX->brute->buffer[0]=0xb5;
			messageUBX->brute->buffer[1]=0x62;
			messageUBX->brute->buffer[2]=messageUBX->CLASS;
			messageUBX->brute->buffer[3]=messageUBX->ID;
			messageUBX->brute->buffer[4]=messageUBX->len_payload & 0x00FF;
			messageUBX->brute->buffer[5]=messageUBX->len_payload >> 8;
			//----//
			payloadIndex = 0;
			state = RECEIVE_MESSAGE;
			break;

		case RECEIVE_MESSAGE:
			ITM_Port32(31)=777;
			if (payloadIndex < messageUBX->len_payload) messageUBX->load->buffer[payloadIndex] = receivedByte;
			if (payloadIndex <= messageUBX->len_payload + 2) messageUBX->brute->buffer[6 + payloadIndex] = receivedByte;
			payloadIndex++ ;

			if  (payloadIndex == messageUBX->len_payload + 2){
				UBXMessageQ_t gnssMsg = { .UBXMessage = messageUBX };
				if (xQueueSendToBack(UBXQueueHandle, &gnssMsg, portMAX_DELAY) != pdTRUE) Error_Handler();

				UART_Transmit_With_Color("\r\n...[INFO] UARTByte --SendQueue-- SUCCESS...\r\n",ANSI_COLOR_RESET);
				ITM_Port32(31)=9999;

				//Reinitisalition de la machine a état
				messageUBX = NULL;
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

