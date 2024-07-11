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

	//RECEVOIR D UNE QUEUE : format :{Header, DynamicBuffer}
	//Allouer memoire ou pas
	xQueueReceive(LoRA_toSendHandle, &LoRAtoSend, osWaitForever);
/*
	LoRAtoSend.header->nbOf_packet = LoRAtoSend.payload->size / (size_t)(RFM9x_FIFO_SIZE - sizeof(LORA_HeaderforSending)) +1;
	LoRAtoSend.header->identifier= 42; // A generer aleatoirement mais de facon unique pour chaque payload
	ITM_Port32(30)=LoRAtoSend.header->nbOf_packet;
	ITM_Port32(30)=LoRAtoSend.payload->size;
	//On decoupe de message en plusieurs ssmessage
	for (int i = 0; i < LoRAtoSend.header->nbOf_packet; i++){
		LoRAtoSend.header->num_packet = i+1; //Permet de faciliter le debug

		size_t remaining_size  = LoRAtoSend.payload->size - (size_t)(i * (RFM9x_FIFO_SIZE - sizeof(LORA_HeaderforSending)));//On oublie aussi la taille du header
		size_t len_payload = remaining_size  > RFM9x_FIFO_SIZE - sizeof(LORA_HeaderforSending) ?
				RFM9x_FIFO_SIZE - sizeof(LORA_HeaderforSending) :
				remaining_size  ;

		LoRAtoSend.header->len_payload = len_payload;
		logMemoryUsage("BEFORE - Lora-buffer - PortMalloc ");
		uint8_t* buffer = (uint8_t*)pvPortMalloc(sizeof(uint8_t)*(len_payload +sizeof(LORA_HeaderforSending)));
		if(buffer == NULL) Error_Handler();


		//On rempli la premiere partie du buffer

		buffer[0] = LoRAtoSend.header->recipient;
		buffer[1] = LoRAtoSend.header->sender;
		buffer[2] = LoRAtoSend.header->type;
		buffer[3] = LoRAtoSend.header->identifier;
		buffer[4] = LoRAtoSend.header->nbOf_packet;
		buffer[5] = LoRAtoSend.header->num_packet;
		buffer[6] = (uint8_t)LoRAtoSend.header->len_payload;

		memcpy(buffer + sizeof(LORA_HeaderforSending),
				LoRAtoSend.payload->buffer + i * (RFM9x_FIFO_SIZE-sizeof(LORA_HeaderforSending)),
				len_payload );

		//SEMAPHORE D ATTENTE DE RFM SEND
		RFM9x_Send(buffer, len_payload); //Pour l'instant on s'oblige a faire comme cela,
		ITM_Port32(30)=len_payload;

		vPortFree(buffer);
		logMemoryUsage("AFTER - Lora-buffer - PortFree ");
	}
*/
	//Liberer memoire
	freeBuffer(LoRAtoSend.payload);
	vPortFree(LoRAtoSend.UBXMessage);
	vPortFree(LoRAtoSend.header);
	logMemoryUsage("AFTER - Lora-payload x UBXMessage x header- PortFree ");
	RFM9x_SetMode_Receive();
}
/************************ ---- ************************/
