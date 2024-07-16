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
#include "RTOS_subfunctions/RTOS_extern.h"


GNSSCom_HandleTypeDef hGNSSCom;
OutputType type = ASCII;
OutputProtocol protocol = UBX;

void GNSSCom_Init(UART_HandleTypeDef* huart,UART_HandleTypeDef* huartDebug){
	hGNSSCom.huart = huart;
	hGNSSCom.huartDebug = huartDebug;

	//hGNSSCom.Rx = initializeBuffer(UART_RX_BUFFER_SIZE);

}
DynamicBuffer* initializeBuffer(size_t initialSize) {
	DynamicBuffer *bufferDynamic = pvPortMalloc(sizeof(DynamicBuffer));
	if (bufferDynamic == NULL) {
		return NULL; // Échec de l'allocation mémoire
	}

	bufferDynamic->buffer = pvPortMalloc(initialSize);
	if (bufferDynamic->buffer == NULL) {
		vPortFree(bufferDynamic); // Libérer la mémoire allouée pour la structure
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
	vPortFree(bufferDynamic->buffer);
	vPortFree(bufferDynamic);
}

GenericMessage* GNSSCom_Receive(uint8_t* buffer,size_t size){
	GenericMessage* genericMessage=(GenericMessage*) malloc(sizeof(GenericMessage));

	for (int i = 0; i < size; i++) {
		if (buffer[i] == HEADER_UBX_1 &&
				buffer[i +1] == HEADER_UBX_2 ){
			genericMessage->typeMessage=UBX;
			UBXMessage_parsed* UbxMessage =(UBXMessage_parsed*) malloc(sizeof(UBXMessage_parsed));
			UbxMessage->CLASS = buffer[i + 2];
			UbxMessage->ID = buffer[i + 3];
			UbxMessage->len_payload= (buffer[i+5] << 8) |buffer[i+4];


			UbxMessage->brute=initializeBuffer((size_t)UbxMessage->len_payload + 8);
			memcpy(UbxMessage->brute->buffer, buffer + i, UbxMessage->brute->size);

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
