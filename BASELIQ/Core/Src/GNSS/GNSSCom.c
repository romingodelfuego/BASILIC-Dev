/*
 * GNSSCom.c
 *
 *  Created on: May 23, 2024
 *      Author: romain.pace
 */
#include <GNSS/GNSSCom.h>
#include "LORA/RFM9x.h"


GNSSCom_HandleTypeDef hGNSSCom;
OutputType type = ASCII;
OutputProtocol protocol = UBX;

void GNSSCom_Init(UART_HandleTypeDef* huart,UART_HandleTypeDef* huartDebug){
	hGNSSCom.huart = huart;
	hGNSSCom.huartDebug = huartDebug;

	hGNSSCom.Rx = initializeBuffer(UART_RX_BUFFER_SIZE);
	memset(hGNSSCom.TxBuffer, 0, UART_TX_BUFFER_SIZE);
	memset(hGNSSCom.DebugBuffer, 0, UART_DEBUG_BUFFER_SIZE);

	GNSSCom_UartActivate(&hGNSSCom);
	HAL_Delay(5000); //En theorie il suffit d attendre la reception du premier msg UART pour envoyer
	GNSSCom_Send_SetVal();
}
DynamicBuffer* initializeBuffer(size_t initialSize) {
	DynamicBuffer *bufferDynamic = malloc(sizeof(DynamicBuffer));
	if (bufferDynamic == NULL) {
		return NULL; // Échec de l'allocation mémoire
	}

	bufferDynamic->buffer = malloc(initialSize);
	if (bufferDynamic->buffer == NULL) {
		free(bufferDynamic); // Libérer la mémoire allouée pour la structure
		return NULL; // Échec de l'allocation mémoire
	}

	bufferDynamic->size = initialSize;
	return bufferDynamic;
}
void resizeBuffer(DynamicBuffer *bufferDynamic, size_t newSize) {
	uint8_t *newData = realloc(bufferDynamic->buffer, newSize);
	if (newData != NULL) {
		bufferDynamic->buffer = newData;
		bufferDynamic->size = newSize;
	}
}
void freeBuffer(DynamicBuffer *bufferDynamic) {
	free(bufferDynamic->buffer);
	free(bufferDynamic);
}
void GNSSCom_UartActivate(GNSSCom_HandleTypeDef* hGNSS){
	HAL_UART_Receive_IT(hGNSS->huart, hGNSS->Rx->buffer, hGNSS->Rx->size);
}
void GNSSCom_Send_SetVal(void){

	CommandnSize commands[] = {
			{commandSetGNSS_Config, sizeof(commandSetGNSS_Config)},
			{commandUart1Ouput, sizeof(commandUart1Ouput)},
			{commandUBXTimeUTC, sizeof(commandUBXTimeUTC)},
			{commandSetTP1_atNVTRate,sizeof(commandSetTP1_atNVTRate)},
			{commandSetTP2, sizeof(commandSetTP2)},
			{commandMeasureRate, sizeof(commandMeasureRate)}
	};
	char message[50];

	for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
		// Transmit debug message
		sprintf(message, "\r\t\t\n...UBXMessage%d...\r\n", i + 1);
		HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

		// Transmit command
		HAL_UART_Transmit(hGNSSCom.huart, commands[i].command, commands[i].size, HAL_MAX_DELAY);

		// Print UBX debug
		memcpy(hGNSSCom.Rx->buffer, commands[i].command, commands[i].size);

		GenericMessage* message = GNSSCom_Receive();
		UBXMessage_parsed* messageUBX=(UBXMessage_parsed*) message->Message.UBXMessage;
		create_message_debug(messageUBX);
		HAL_UART_Transmit(hGNSSCom.huartDebug,(uint8_t*) messageUBX->bufferDebug, sizeof(messageUBX->bufferDebug), HAL_MAX_DELAY);
		freeBuffer(message->Message.UBXMessage->UBX_Brute);
		freeBuffer(message->Message.UBXMessage->load);
		free(message->Message.UBXMessage);
		free(message);

	}
}
GenericMessage* GNSSCom_Receive(){
	GenericMessage* genericMessage=(GenericMessage*) malloc(sizeof(GenericMessage));

	for (int i = 0; i < hGNSSCom.Rx->size; i++) {
		if (hGNSSCom.Rx->buffer[i] == HEADER_UBX_1 &&
				hGNSSCom.Rx->buffer[i +1] == HEADER_UBX_2 ){
			genericMessage->typeMessage=UBX;
			UBXMessage_parsed* UbxMessage =(UBXMessage_parsed*) malloc(sizeof(UBXMessage_parsed));
			UbxMessage->msgClass = hGNSSCom.Rx->buffer[i + 2];
			UbxMessage->msgID = hGNSSCom.Rx->buffer[i + 3];
			UbxMessage->len = (hGNSSCom.Rx->buffer[i+5] << 8) |hGNSSCom.Rx->buffer[i+4];
			UbxMessage->load=initializeBuffer((size_t)UbxMessage->len);
			memcpy(UbxMessage->load->buffer, hGNSSCom.Rx->buffer + i + 6, UbxMessage->load->size);

			UbxMessage->UBX_Brute=initializeBuffer((size_t)UbxMessage->len + 8);
			memcpy(UbxMessage->UBX_Brute->buffer, hGNSSCom.Rx->buffer + i, UbxMessage->UBX_Brute->size);

			genericMessage->Message.UBXMessage = UbxMessage;
			return genericMessage;
		}

		else if(hGNSSCom.Rx->buffer[i] == HEADER_NMEA) {
			NMEAMessage_parsed* NMEAMessage =(NMEAMessage_parsed*) malloc(sizeof(NMEAMessage_parsed));
			genericMessage->typeMessage= NMEA;
			genericMessage->Message.NMEAMessage = NMEAMessage;
			return genericMessage; //Temporaire
		}
	}

	return genericMessage;
}
