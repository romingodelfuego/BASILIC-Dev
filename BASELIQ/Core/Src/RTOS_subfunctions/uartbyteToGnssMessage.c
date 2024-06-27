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
static UBXMessage_parsed* messageUBX = NULL;

void uartbyteToGnssMessage(void){
	if (xQueueReceive(UARTbyteHandle, &uartMsg, portMAX_DELAY) == pdTRUE) {
		uint8_t receivedByte = uartMsg.data;
		// Machine à états pour traiter les messages
		switch (state) {
		case WAIT_FOR_SYNC_1:
			if (receivedByte == HEADER_UBX_1) {
				ITM_Port32(31)=1111;
				osSemaphoreWait(GNSS_UART_AccessHandle, osWaitForever);
				state = WAIT_FOR_SYNC_2;
			}
			break;

		case WAIT_FOR_SYNC_2:
			if (receivedByte == HEADER_UBX_2)
			{state = WAIT_FOR_CLASS;}
			else
			{state = WAIT_FOR_SYNC_1;}
			break;

		case WAIT_FOR_CLASS:
			if (messageUBX ==NULL){
				messageUBX = (UBXMessage_parsed*) pvPortMalloc(sizeof(UBXMessage_parsed)) ;
			}
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
			if (messageUBX->len_payload > 0 && messageUBX->len_payload <= UART_MAX_BUFFER_SIZE) {
				messageUBX->load = initializeBuffer(messageUBX->len_payload);
				messageUBX->brute = initializeBuffer(messageUBX->len_payload+8);
				if (messageUBX->load == NULL) {
					// Erreur d'allocation de mémoire
					UART_Transmit_With_Color(hGNSSCom.huartDebug,"\r\t\t\n...UARTByte --ALLOCATION-- FAILED...\r\n",ANSI_COLOR_RED);
					state = WAIT_FOR_SYNC_1;
				} else {
					payloadIndex = 0;
					messageUBX->brute->buffer[0]=0xb5;
					messageUBX->brute->buffer[1]=0x62;
					messageUBX->brute->buffer[2]=messageUBX->CLASS;
					messageUBX->brute->buffer[3]=messageUBX->ID;
					messageUBX->brute->buffer[4]=messageUBX->len_payload & 0x00FF;
					messageUBX->brute->buffer[5]=messageUBX->len_payload >> 8;
					state = RECEIVE_MESSAGE;
				}
			} else {
				// Longueur invalide
				UART_Transmit_With_Color(hGNSSCom.huartDebug,"\r\t\t\n...UARTByte --INVALID LENGTH-- FAILED...\r\n",ANSI_COLOR_RED);
				vPortFree(messageUBX);
				state = WAIT_FOR_SYNC_1;
			}
			break;
		case RECEIVE_MESSAGE:
			payloadIndex++ ;
			if (payloadIndex < messageUBX->len_payload){
				messageUBX->load->buffer[payloadIndex] = receivedByte;
			}
			if (payloadIndex <= messageUBX->len_payload + 2){
				messageUBX->brute->buffer[6 + payloadIndex] = receivedByte;
			}
			if  (payloadIndex == messageUBX->len_payload + 2){ // ON perd un byte ici !
				GenericMessage* receptionGNSS = (GenericMessage*)pvPortMalloc(sizeof(GenericMessage));

				receptionGNSS->typeMessage=UBX;
				receptionGNSS->Message.UBXMessage = messageUBX;

				UBXMessageQ_t gnssMsg = { .receptionGNSS = receptionGNSS };
				if (xQueueSendToBack(UBXQueueHandle, &gnssMsg, portMAX_DELAY) != pdTRUE) {
					// Erreur d'envoi dans la queue
					UART_Transmit_With_Color(hGNSSCom.huartDebug,"\r\t\t\n...UARTByte --SendQueue-- FAILED...\r\n",ANSI_COLOR_RED);

				}else{
					UART_Transmit_With_Color(hGNSSCom.huartDebug,"\r\n...[INFO] UARTByte --SendQueue-- SUCCESS...\r\n",ANSI_COLOR_RESET);
				}
				osSemaphoreRelease(GNSS_UART_AccessHandle);
				freeBuffer(messageUBX->load);
				freeBuffer(messageUBX->brute);
				vPortFree(messageUBX);
				messageUBX = NULL;
				vPortFree(receptionGNSS);
				state = WAIT_FOR_SYNC_1;
				ITM_Port32(31)=9999;
			}
			break;
		default:
			state = WAIT_FOR_SYNC_1;
			break;
		}
	}
}
