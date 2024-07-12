/*
 * senderLoRA.c
 *
 *  Created on: Jul 8, 2024
 *      Author: romain.pace
 */
#include "RTOS_subfunctions/senderLoRA.h"

/************************ TASK ************************/
void senderLoRA(){
	LoRAtoSendQ_t LoRAtoSend;
	logMemoryUsage("START - Lora Sender");
	//RECEVOIR D UNE QUEUE : format :{Header, DynamicBuffer}
	//Allouer memoire ou pas
	xQueueReceive(LoRA_toSendHandle, &LoRAtoSend, osWaitForever);

	updateMemoryUsage();
	uint8_t* buffer = (uint8_t*)pvPortMalloc(sizeof(uint8_t)*(LoRAtoSend.header->len_payload + sizeof(LORA_HeaderforReception)));
	if(buffer == NULL) Error_Handler();
	updateMemoryUsage();


	//On rempli la premiere partie du buffer

	buffer[0] = LoRAtoSend.header->recipient;
	buffer[1] = LoRAtoSend.header->sender;
	buffer[2] = LoRAtoSend.header->type;
	buffer[3] = (uint8_t)LoRAtoSend.header->len_payload;

	memcpy(buffer + sizeof(LORA_HeaderforReception),
			LoRAtoSend.payload->buffer ,
			LoRAtoSend.payload->size );
	updateMemoryUsage();

	//SEMAPHORE D ATTENTE DE RFM SEND
	RFM9x_Send(buffer, LoRAtoSend.payload->size + sizeof(LORA_HeaderforReception)); //Pour l'instant on s'oblige a faire comme cela,

	vPortFree(buffer);
	updateMemoryUsage();



//Liberer memoire
logMemoryUsage("END - Lora Sender");
RFM9x_SetMode_Receive();
}
/************************ ---- ************************/
