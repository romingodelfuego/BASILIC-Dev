/*
 * senderLoRA.c
 *
 *  Created on: Jul 8, 2024
 *      Author: romain.pace
 */
#include "RTOS_subfunctions/senderLoRA.h"
#include "GNSS/debug_utils.h"

/************************ TASK ************************/
void senderLoRA(){
	LoRAtoSendQ_t LoRAtoSend;
	logMemoryUsage("START - Lora Sender");

	/* RECEIVE FROM QUEUE : format :{Header, DynamicBuffer} */
	xQueueReceive(LoRA_toSendHandle, &LoRAtoSend, osWaitForever);

	UART_Transmit_With_Color("\n\r--- SEND MESSAGE ---",ANSI_COLOR_MAGENTA);

	uint8_t* buffer = (uint8_t*)pvPortMalloc(LoRAtoSend.header->len_payload + sizeof(LORA_HeaderforReception));
	if(buffer == NULL) Error_Handler();
	updateMemoryUsage();

	//On rempli la premiere partie du buffer
	buffer[0] = LoRAtoSend.header->recipient;
	buffer[1] = LoRAtoSend.header->sender;
	buffer[2] = LoRAtoSend.header->type;
	buffer[3] = LoRAtoSend.header->len_payload;

	memcpy(buffer + sizeof(LORA_HeaderforReception),
			LoRAtoSend.payload->buffer ,
			LoRAtoSend.header->len_payload);

	RFM9x_Send(buffer, LoRAtoSend.header->len_payload + sizeof(LORA_HeaderforReception));
	char* hexString = (char*)pvPortMalloc(sizeof(LoRAtoSend.header->recipient));
	if (hexString == NULL) Error_Handler();
	uint8_array_to_hex_string(hexString,&LoRAtoSend.header->recipient,sizeof(LoRAtoSend.header->recipient));

	UART_Transmit_With_Color(hexString,ANSI_COLOR_MAGENTA);

	vPortFree(hexString);
	vPortFree(buffer);
	logMemoryUsage("END - Lora Sender");
}
/************************ ---- ************************/
