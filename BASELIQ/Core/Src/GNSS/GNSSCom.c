/*
 * GNSSCom.c
 *
 *  Created on: May 23, 2024
 *      Author: romain.pace
 */
#include <GNSS/GNSSCom.h>
#include "LORA/RFM9x.h"
#include "FreeRTOS.h"
#include "task.h"

	GNSSCom_HandleTypeDef hGNSSCom;
	OutputType type = ASCII;
	OutputProtocol protocol = UBX;

void GNSSCom_Init(UART_HandleTypeDef* huart,UART_HandleTypeDef* huartDebug){
	hGNSSCom.huart = huart;
	hGNSSCom.huartDebug = huartDebug;

	hGNSSCom.Rx = initializeBuffer(UART_RX_BUFFER_SIZE);
	memset(hGNSSCom.DebugBuffer, 0, UART_DEBUG_BUFFER_SIZE);

	GNSSCom_UartActivate(&hGNSSCom);
	HAL_Delay(5000); //En theorie il suffit d attendre la reception du premier msg UART pour envoyer
	GNSSCom_SetUp_Init();


}
void GNSSCom_UartActivate(GNSSCom_HandleTypeDef* hGNSS){
	HAL_UART_Receive_IT(hGNSS->huart, hGNSS->Rx->buffer, hGNSS->Rx->size);
}

DynamicBuffer* initializeBuffer(size_t initialSize) {
	DynamicBuffer *bufferDynamic = (DynamicBuffer *)malloc(sizeof(DynamicBuffer));
	if (bufferDynamic == NULL) {
		return NULL; // Échec de l'allocation mémoire
	}

	bufferDynamic->buffer = (uint8_t*)malloc(initialSize);
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
void GNSSCom_Send_SetVal(CommandnSize toTransmit){
	HAL_UART_Transmit(hGNSSCom.huart, toTransmit.command, toTransmit.size, HAL_MAX_DELAY);
}
void GNSSCom_SetUp_Init(void){

	CommandnSize commands[] = {
			{commandSetGNSS_Config, sizeof(commandSetGNSS_Config)},

			{commandSetTP1_atNVTRate,sizeof(commandSetTP1_atNVTRate)},
			{commandSetTP2, sizeof(commandSetTP2)},
			{commandMeasureRate, sizeof(commandMeasureRate)},
			{commandUart1Ouput, sizeof(commandUart1Ouput)},
			{commandUBXTimeUTC, sizeof(commandUBXTimeUTC)}
	};
	char message[50];

	for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
		// Transmit debug message
		sprintf(message, "\r\t\t\n...UBXMessage%d...\r\n", i + 1);
		HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

		// Transmit command
		GNSSCom_Send_SetVal(commands[i]);

		// On fais croire que la commande a ete recu par le RX buffer : TIPS pour print en debug la commande
		memcpy(hGNSSCom.Rx->buffer, commands[i].command, commands[i].size);

		GenericMessage* command_debug = GNSSCom_Receive(hGNSSCom.Rx->buffer,hGNSSCom.Rx->size);
		if (command_debug->typeMessage == UBX){
			UBXMessage_parsed* messageUBX=(UBXMessage_parsed*) command_debug->Message.UBXMessage;
			create_message_debug(messageUBX);
			HAL_UART_Transmit(hGNSSCom.huartDebug,(uint8_t*) messageUBX->bufferDebug, sizeof(messageUBX->bufferDebug), HAL_MAX_DELAY);
			freeBuffer(command_debug->Message.UBXMessage->UBX_Brute);
			freeBuffer(command_debug->Message.UBXMessage->load);
			free(command_debug->Message.UBXMessage);
			free(command_debug);
		}
		else{
			sprintf(message, "\r\t\t\n...UBXMessage%d - FAILED...\r\n", i + 1);
			HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
			if (command_debug->typeMessage == NMEA){
				free(command_debug->Message.NMEAMessage);
				free(command_debug);
			}
		}
	}
}
GenericMessage* GNSSCom_Receive(uint8_t* buffer,size_t size){
	GenericMessage* genericMessage=(GenericMessage*) malloc(sizeof(GenericMessage));

	for (int i = 0; i < size; i++) {
		if (buffer[i] == HEADER_UBX_1 &&
				buffer[i +1] == HEADER_UBX_2 ){
			genericMessage->typeMessage=UBX;
			UBXMessage_parsed* UbxMessage =(UBXMessage_parsed*) malloc(sizeof(UBXMessage_parsed));
			UbxMessage->msgClass = buffer[i + 2];
			UbxMessage->msgID = buffer[i + 3];
			UbxMessage->len = (buffer[i+5] << 8) |buffer[i+4];
			UbxMessage->load=initializeBuffer((size_t)UbxMessage->len);

			memcpy(UbxMessage->load->buffer, buffer + i + 6, UbxMessage->load->size);

			UbxMessage->UBX_Brute=initializeBuffer((size_t)UbxMessage->len + 8);
			memcpy(UbxMessage->UBX_Brute->buffer, buffer + i, UbxMessage->UBX_Brute->size);

			genericMessage->Message.UBXMessage = UbxMessage;
			return genericMessage;
		}

		else if(buffer[i] == HEADER_NMEA) {
			NMEAMessage_parsed* NMEAMessage =(NMEAMessage_parsed*) malloc(sizeof(NMEAMessage_parsed));
			genericMessage->typeMessage= NMEA;
			genericMessage->Message.NMEAMessage = NMEAMessage;
			return genericMessage; //Temporaire
		}
	}
	return genericMessage;
}

void UART_Debug(GenericMessage* reception){
	UBXMessage_parsed* messageUBX = (UBXMessage_parsed*)reception->Message.UBXMessage;
	create_message_debug(messageUBX);
	HAL_UART_Transmit(hGNSSCom.huartDebug,(uint8_t*) messageUBX->bufferDebug, sizeof(messageUBX->bufferDebug), HAL_MAX_DELAY);
	freeBuffer(reception->Message.UBXMessage->UBX_Brute);
	freeBuffer(reception->Message.UBXMessage->load);
	free(reception->Message.UBXMessage);
}
