/*
 * receivedLora.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include <RTOS_subfunctions/receiverLoRA.h>

/************************ TASK ************************/
void receivedLora(void){
	logMemoryUsage("START - LoRA Reception");
	UART_Transmit_With_Color("\n\r--- RECEIVED MESSAGE ---",ANSI_COLOR_GREEN);
	LORA_MessageReception* LORA_Receive_Message = (LORA_MessageReception*)pvPortMalloc(sizeof(LORA_MessageReception)); // On pointe vers une partie de la memoire HEAP protégée
	if (LORA_Receive_Message == NULL) Error_Handler();
	LORA_Receive_Message->header = (LORA_HeaderforSending*)pvPortMalloc(sizeof(LORA_HeaderforSending));
	if (LORA_Receive_Message->header == NULL) Error_Handler();

	updateMemoryUsage();

	ITM_Port32(30)=357951;
	RFM9x_Receive(LORA_Receive_Message);

	if (!LORA_Receive_Message->RxNbrBytes){
		ITM_Port32(30)=66; //Si on recoit du bruit
	}

	else if (LORA_Receive_Message->header->recipient == MODULE_BROADCAST_ADDRESS
			||LORA_Receive_Message->header->recipient == MODULE_SOURCE_ADDRESS){
			vPortFree(LORA_Receive_Message->payload);

	}
	updateMemoryUsage();
	vPortFree(LORA_Receive_Message->header);
	vPortFree(LORA_Receive_Message); //contient des attribut qui sont pvPortMalloc et qui sont free de facon asynchrone
	logMemoryUsage("END -LoRA Reception");
}
/************************ ---- ************************/
/************************ FUNCTIONS ************************/

/************************ -------- ************************/
