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
	xQueueReceive(LoRA_toSendHandle, &LoRAtoSend, osWaitForever);
	NotifyForRFM_IRQ = (NotifyForRFM_IRQ_t){.task=SenderLoRaHandle,.name="SenderLoRaHandle"};

	logMemoryUsage("START - Lora Sender");
	LoRAtoSend.header->nbOf_packet = LoRAtoSend.payload->size / (size_t)(RFM9x_FIFO_SIZE - sizeof(LORA_HeaderforSending)) +1;
	LoRAtoSend.header->identifier= (uint8_t)(rand() % 256); // A generer aleatoirement mais de facon unique pour chaque payload

	/*	We are slicing long message into sub-message
	 * sub-message are contains with LoRA header which allows the recipient
	 * to rebuild the whole initial message
	*/
	for (int i = 0; i < LoRAtoSend.header->nbOf_packet; i++){
		LoRAtoSend.header->num_packet = i+1;

		size_t remaining_size  = LoRAtoSend.payload->size - (size_t)(i * (RFM9x_FIFO_SIZE - sizeof(LORA_HeaderforSending)));
		size_t len_payload = remaining_size  > RFM9x_FIFO_SIZE - sizeof(LORA_HeaderforSending) ?
				RFM9x_FIFO_SIZE - sizeof(LORA_HeaderforSending) :
				remaining_size  ;

		LoRAtoSend.header->len_payload = len_payload; // 0 - RFM9x_FIFO_SIZE = 255
		updateMemoryUsage();
		uint8_t* buffer = (uint8_t*)pvPortMalloc(sizeof(uint8_t)*(len_payload +sizeof(LORA_HeaderforSending)));
		if(buffer == NULL) Error_Handler();
		updateMemoryUsage();

		//Filling the buffer with the LoRA header

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
		updateMemoryUsage();

		vTaskDelay(500); //Ce delai depend de la distance avec la cible
		RFM9x_Send(buffer, len_payload + sizeof(LORA_HeaderforSending));
		xTaskNotifyWait(0x00,        // Ne pas effacer de bits à l'entrée
				                        0xFFFFFFFF,  // Effacer tous les bits à la sortie
				                        NULL,        // Stocker la valeur des bits notifiés
				                        portMAX_DELAY); // Attendre indéfiniment*/

		char* hexString_LORA = (char*)pvPortMalloc((len_payload + sizeof(LORA_HeaderforSending))*2 + 1);
		if (hexString_LORA == NULL) Error_Handler();
		updateMemoryUsage();
		uint8_array_to_hex_string(hexString_LORA, buffer, len_payload + sizeof(LORA_HeaderforSending));
		UART_Transmit_With_Color("\r\nMESSAGE RFM9x Send: \r\n",ANSI_COLOR_RESET);
		UART_Transmit_With_Color(hexString_LORA, ANSI_COLOR_GREEN);
		vPortFree(hexString_LORA);

		vPortFree(buffer);
		updateMemoryUsage();

	}
	UART_Transmit_With_Color("\r\nMESSAGE RFM9x Send: TERMINATED\r\n",ANSI_COLOR_MAGENTA);

	//Liberer mémoire
	freeBuffer(LoRAtoSend.payload);
	vPortFree(LoRAtoSend.UBXMessage);
	vPortFree(LoRAtoSend.header);
	logMemoryUsage("END - Lora Sender");
	RFM9x_SetMode_Receive();
}
/************************ ---- ************************/
