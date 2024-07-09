/*
 * receivedLora.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include <RTOS_subfunctions/receiverLoRA.h>

/************************ TASK ************************/
void receivedLora(void){

	LORA_MessageReception* LORA_Receive_Message = (LORA_MessageReception*)pvPortMalloc(sizeof(LORA_MessageReception)); // On pointe vers une partie de la memoire HEAP protégée
	if (LORA_Receive_Message == NULL) Error_Handler();
	ITM_Port32(30)=357951;
	RFM9x_Receive(LORA_Receive_Message);

	if (!LORA_Receive_Message->RxNbrBytes){
		ITM_Port32(30)=66; //Si on recoit du bruit
	}

	else if (LORA_Receive_Message->header->recipient == MODULE_BROADCAST_ADDRESS
			||LORA_Receive_Message->header->recipient == MODULE_SOURCE_ADDRESS){

		switch (LORA_Receive_Message->header->type){

		/*case PACKET_TYPE_ACK:
			PACKET_TYPE_ACK_fct();
			break;*/

		case PACKET_TYPE_POLL:
			PACKET_TYPE_POLL_fct(LORA_Receive_Message);
			break;

		default:
			break;
		}
	}
	ITM_Port32(30)=159753;
	vPortFree(LORA_Receive_Message); //contient des attribut aui sont pvPortMalloc et qui sont free de facon asynchrone
}
/************************ ---- ************************/
/************************ FUNCTIONS ************************/
void PACKET_TYPE_POLL_fct(LORA_MessageReception* LORA_Receive_Message){
	GNSSReturnQ_t gnssReturn;
	GNSStoPollQ_t poll = {
			(const uint8_t*) LORA_Receive_Message->payload,
			(size_t) LORA_Receive_Message->header->len_payload,
			"LORA_POLLING"
	};
	GNSSRequestQ_t requestFromLora = {
			.Request_TIME= xTaskGetTickCount(),
			.CLASS = LORA_Receive_Message->payload[2],
			.ID = LORA_Receive_Message->payload[3],
			.applicantSemaphore = LORA_Access_GNSS_ReturnHandle,
			.applicantName = "LORAPolling_REQUEST"
	};
	/*--------------- COMMAND PART ---------------*/
	xQueueSendToBack(GNSS_RequestHandle,&requestFromLora,osWaitForever);

	osSemaphoreWait(LORA_Access_GNSS_ReturnHandle,osWaitForever);

	UART_Transmit_With_Color("\r\t\t\n...UBXMessage --FROM-- LORA Polling...\r\n",ANSI_COLOR_MAGENTA);
	ITM_Port32(30)=111;
	request_commandToGNSS(poll); //On envoie un message vers GNSS
	ITM_Port32(30)=444;
	osStatus eventLORA = osSemaphoreWait(LORA_Access_GNSS_ReturnHandle, osWaitForever);

	if (eventLORA !=osOK){
		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --SEND-- LORA Polling...",ANSI_COLOR_MAGENTA);
		UART_Transmit_With_Color("\t---ISSUE SEMAPHORE--\r\n",ANSI_COLOR_RED);
		return;
	}

	xQueueReceive(GNSS_ReturnHandle, &gnssReturn, osWaitForever);
	/*--------------- SEND PART ---------------*/
	ITM_Port32(30)=555;

	if (gnssReturn.statut!=OK){
		ITM_Port32(31)=99;
		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --SEND-- LORA Polling...",ANSI_COLOR_MAGENTA);
		UART_Transmit_With_Color("\t---NOT FOUND--\r\n",ANSI_COLOR_RED);
		return;
	}
	LORA_HeaderforSending* headerSend =(LORA_HeaderforSending*) pvPortMalloc(sizeof(LORA_HeaderforSending));
	if (headerSend == NULL) Error_Handler();

	*headerSend = (LORA_HeaderforSending){
		.recipient = 0xFE,
				.sender = MODULE_SOURCE_ADDRESS,
				.type = PACKET_TYPE_POLL,
	};
	LoRAtoSendQ_t LoRAtoSend= {.header = headerSend,
			.payload = gnssReturn.bufferReturn,
			.UBXMessage=gnssReturn.UBXMessage,
	};
	xQueueSendToBack(LoRA_toSendHandle,&LoRAtoSend,osWaitForever);
	/*--------------- ________ ---------------*/
	UART_Transmit_With_Color("\r\t\t\n...UBXMessage --SEND-- LORA Polling...",ANSI_COLOR_MAGENTA);
	UART_Transmit_With_Color("\t---SUCCESS---\r\n",ANSI_COLOR_GREEN);

	if (gnssReturn.bufferReturn->size<= 512){
		char* hexString_LORA = (char*)pvPortMalloc(gnssReturn.bufferReturn->size * 2 + 1);
		if (hexString_LORA == NULL) Error_Handler();

		uint8_array_to_hex_string(hexString_LORA, gnssReturn.bufferReturn->buffer, gnssReturn.bufferReturn->size);
		UART_Transmit_With_Color(hexString_LORA,ANSI_COLOR_MAGENTA);
		vPortFree(hexString_LORA);

	}else{
		char* len = (char*)pvPortMalloc(sizeof(TickType_t) * sizeof(char));
		if (len == NULL) Error_Handler();

		UART_Transmit_With_Color("\r\t\t\n...UBXMessage --SEND-- LORA Polling --TOO LONG FOR DEBUG...\t",ANSI_COLOR_MAGENTA);
		sprintf(len, "%u",gnssReturn.bufferReturn->size);
		UART_Transmit_With_Color(len, ANSI_COLOR_RED);
		vPortFree(len);
	}

	// Reinitialisation de la trame
	ITM_Port32(30)=666;
	osSemaphoreRelease(LORA_Access_GNSS_ReturnHandle);
	// --- //
}

/*void PACKET_TYPE_ACK_fct(void){
	//Lora send un messsage vide
	LORA_Header* headerSend =(LORA_Header*) pvPortMalloc(sizeof(LORA_Header));

 *headerSend = (LORA_Header){
		.recipient = 0xFE,
				.sender = MODULE_SOURCE_ADDRESS,
				.type = PACKET_TYPE_ACK,
				.len_payload = sizeof(NULL)
	};
	LoRAtoSendQ_t LoRAtoSend= {.header = headerSend,
			.payload = gnssReturn.bufferReturn,
			.itemFromUBXtoFree = gnssReturn.itemFromUBXtoFree,
			.gnssReturntoFree=NULL};
	xQueueSendToBack(headerSend, NULL);
	vPortFree(headerSend);
}*/
/************************ -------- ************************/
