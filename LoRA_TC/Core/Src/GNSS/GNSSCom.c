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
	/*uint8_t *newData = realloc(bufferDynamic->buffer, newSize);
	if (newData != NULL) {
		bufferDynamic->buffer = newData;
		bufferDynamic->size = newSize;
	}*/
}
void freeBuffer(DynamicBuffer *bufferDynamic) {
	vPortFree(bufferDynamic->buffer);
	vPortFree(bufferDynamic);
}

void GNSSCom_MessageAdapter(uint8_t* buffer,size_t* size, GenericMessage* genericMessage){
		if (buffer[0] == HEADER_UBX_1 &&
				buffer[1] == HEADER_UBX_2 ){
			genericMessage->typeMessage=UBX;
			genericMessage->Message.UBXMessage =(UBXMessage_parsed*) pvPortMalloc(sizeof(UBXMessage_parsed));
			updateMemoryUsage();

			genericMessage->Message.UBXMessage->CLASS = buffer[2];
			genericMessage->Message.UBXMessage->ID = buffer[3];
			genericMessage->Message.UBXMessage->len_payload= (buffer[5] << 8) |buffer[4];
			//genericMessage->Message.UBXMessage->brute = (DynamicBuffer*)initializeBuffer(genericMessage->Message.UBXMessage->len_payload +8);
			genericMessage->Message.UBXMessage->brute = (DynamicBuffer*)pvPortMalloc(sizeof(DynamicBuffer));
			*(DynamicBuffer*) genericMessage->Message.UBXMessage->brute=(DynamicBuffer){.buffer = buffer,.size=*size};
			updateMemoryUsage();
			//memcpy(genericMessage->Message.UBXMessage->brute->buffer, buffer , genericMessage->Message.UBXMessage->len_payload +8);
			updateMemoryUsage();
		}

		else if(buffer[0] == HEADER_NMEA) {
			NMEAMessage_parsed* NMEAMessage =(NMEAMessage_parsed*) pvPortMalloc(sizeof(NMEAMessage_parsed));
			genericMessage->typeMessage= NMEA;
			genericMessage->Message.NMEAMessage = NMEAMessage;
		}
}
