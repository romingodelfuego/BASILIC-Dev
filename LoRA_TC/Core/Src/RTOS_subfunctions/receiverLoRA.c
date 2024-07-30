/*
 * receivedLora.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include <RTOS_subfunctions/receiverLoRA.h>
extern ModuleConfig_t ModuleConfig;
/************************ TASK ************************/
void receivedLora(void){
	logMemoryUsage("START - LoRA Reception");

	LORA_MessageReception* LORA_Receive_Message = (LORA_MessageReception*)pvPortMalloc(sizeof(LORA_MessageReception)); // On pointe vers une partie de la memoire HEAP protégée
	if (LORA_Receive_Message == NULL) Error_Handler();
	LORA_Receive_Message->header = (LORA_HeaderforSending*)pvPortMalloc(sizeof(LORA_HeaderforSending));
	if (LORA_Receive_Message->header == NULL) Error_Handler();
	updateMemoryUsage();

	RFM9x_Receive(LORA_Receive_Message); //PvPortMalloc LORA_Receive_Message->payload

	if (!LORA_Receive_Message->RxNbrBytes){
		// IF THE NOISE IS TRIGERRED
		vPortFree(LORA_Receive_Message->header);
		vPortFree(LORA_Receive_Message);
		updateMemoryUsage();
		return;
	}

	else if (LORA_Receive_Message->header->recipient == MODULE_BROADCAST_ADDRESS
			||LORA_Receive_Message->header->recipient == MODULE_SOURCE_ADDRESS){
		// IF THE IS ADDRESS TO THIS MODULE
		// ADRESS CAN BE MANAGED FROM THE inc/LoRA/LoraCom.h file
		UART_Transmit_With_Color("\n\r--- ADRESSED MESSAGE ---",ANSI_COLOR_GREEN);
		messageLoRATreatment(LORA_Receive_Message);
	}
	else{
		UART_Transmit_With_Color("\n\r--- INTERCEPTED MESSAGE ---",ANSI_COLOR_GREEN);
		vPortFree(LORA_Receive_Message->payload);
		vPortFree(LORA_Receive_Message->header);
		vPortFree(LORA_Receive_Message);
	}
	logMemoryUsage("END - LoRA Reception");
	RFM9x_SetMode_Receive();

}
/************************ ---- ************************/
/************************ FUNCTIONS ************************/
void messageLoRATreatment(LORA_MessageReception* LORA_Receive_Message){
	/*On verifie si le numofPacket == nbOfPacket dans ce cas on est a la fin de la reception
	//On recherche tout les messages avec le mm identifier
	//On réalise une synthèse des messages recues
	//ie concaténation dans l'ordre des payloads
	//pourquoi pas ajout des header de reception pour chaque messages ie [SNR,RSSI][][][]
	//Si non on ajoute à la queue LoRAinReception*/
	LoRAinReceptionQ_t LoRAinReceptionQ;
	LoRAinReceptionQ =(LoRAinReceptionQ_t){.LMR = LORA_Receive_Message};
	updateMemoryUsage();
/*
	char* nbpacketdebug = (char*)pvPortMalloc(sizeof(char));
	UART_Transmit_With_Color("\r\nPACKET n°",ANSI_COLOR_RESET);
	sprintf(nbpacketdebug,"%u",LoRAinReceptionQ.LMR->header->num_packet);
	UART_Transmit_With_Color(nbpacketdebug,ANSI_COLOR_RESET);
	vPortFree(nbpacketdebug);

	char* hexString_LORA = (char*)pvPortMalloc(LoRAinReceptionQ.LMR->header->len_payload * 2 + 1);
	uint8_array_to_hex_string(hexString_LORA, LoRAinReceptionQ.LMR->payload, LoRAinReceptionQ.LMR->header->len_payload);
	UART_Transmit_With_Color("\r\n-->\t",ANSI_COLOR_RESET);
	UART_Transmit_With_Color(hexString_LORA,ANSI_COLOR_RESET);
	vPortFree(hexString_LORA);
*/
	xQueueSendToBack(LoRA_inReceptionHandle,&LoRAinReceptionQ,osWaitForever);

	if (LORA_Receive_Message->header->num_packet == LORA_Receive_Message->header->nbOf_packet)
	{
		logMemoryUsage("START - Completed Reception");
		//Liste de tous les LoRAinReceptionQ_t validant le meme identifier
		LoRAinReceptionQ_t correspondingIdentifier[LORA_Receive_Message->header->nbOf_packet];
		processQueueAndStoreIdentifiers(LoRA_inReceptionHandle,
										LORA_Receive_Message->header->identifier,
										correspondingIdentifier);

		size_t total_length = 0;
		// synthesisPayload is pvPortMalloc
		uint8_t* synthesisPayload = concat_payloads(correspondingIdentifier,
								(uint8_t)LORA_Receive_Message->header->nbOf_packet,
								&total_length);

		/*char* hexString_LORA = (char*)pvPortMalloc(total_length * 2 + 1);
		if (hexString_LORA == NULL) Error_Handler();

		uint8_array_to_hex_string(hexString_LORA, synthesisPayload, total_length);
		UART_Transmit_With_Color("\r\nMESSAGE RECEIVED COMPLETLY, HERE IS THE RESULT PAYLOAD: \r\n",ANSI_COLOR_RESET);
		UART_Transmit_With_Color(hexString_LORA, ANSI_COLOR_GREEN);
		vPortFree(hexString_LORA);*/

		//Objectif afficher le message en debug avec traductor.c
		//Il faut donc creer un UBXMessage_Parsed
		GenericMessage* genericMessage=(GenericMessage*) pvPortMalloc(sizeof(GenericMessage));
		GNSSCom_MessageAdapter(synthesisPayload,&total_length,genericMessage);// GNSSCom_MessageAdapter pvPortMalloc: genericMessage->Message.UBXMessage->brute & genericMessage->Message.UBXMessage
		updateMemoryUsage();
		if (genericMessage->typeMessage==UBX){
			//traductor pvPortMalloc
			traductor(genericMessage->Message.UBXMessage, ModuleConfig);

			//freeBuffer(genericMessage->Message.UBXMessage->brute);
			vPortFree(genericMessage->Message.UBXMessage->brute);
			vPortFree(genericMessage->Message.UBXMessage);
			updateMemoryUsage();
		}
		vPortFree(genericMessage);
		vPortFree(synthesisPayload);
		logMemoryUsage("END - Completed Reception");
	}
}
/*
 * Example of Use :
    LORA_HeaderforSending header1 = {0x01, 0x02, 0x03, 3};
    uint8_t payload1[] = {0x01, 0x02, 0x03};
    LORA_HeaderforSending header2 = {0x04, 0x05, 0x06, 2};
    uint8_t payload2[] = {0x04, 0x05};
    LORA_HeaderforSending header3 = {0x07, 0x08, 0x09, 4};
    uint8_t payload3[] = {0x06, 0x07, 0x08, 0x09};

    LoRAinReceptionQ_t structsToConcatenate[] = {
        {&header1, payload1},
        {&header2, payload2},
        {&header3, payload3}
    };

    Output : should be  {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09}
 */
uint8_t* concat_payloads(LoRAinReceptionQ_t* structsToConcatenate, uint8_t nbOfstructsToConcatenate, size_t* total_length) {
    *total_length = 0;
    size_t offset = 0;
    uint8_t* result = NULL;
    uint8_t* temp = NULL;

    for (int i = 0; i < nbOfstructsToConcatenate; i++) {
        size_t new_total_length = *total_length + structsToConcatenate[i].LMR->header->len_payload;

        // Allocate new memory block
        temp = (uint8_t*)pvPortMalloc(new_total_length);
        if (temp == NULL) Error_Handler();

        // Copy the existing data to the new block
        if (result != NULL) {
            memcpy(temp, result, *total_length);
            vPortFree(result); // Free the old memory block
        }

        // Update the total length
        *total_length = new_total_length;
        result = temp; // Assign the new block to result

        // Copy the new data into the new block
        memcpy(result + offset, structsToConcatenate[i].LMR->payload, structsToConcatenate[i].LMR->header->len_payload);
        offset += (size_t)structsToConcatenate[i].LMR->header->len_payload;

        // Free the individual payload memory
        vPortFree(structsToConcatenate[i].LMR->payload);
        vPortFree(structsToConcatenate[i].LMR->header);
        vPortFree(structsToConcatenate[i].LMR);
        updateMemoryUsage();
    }
    return result;
}


void processQueueAndStoreIdentifiers(osMessageQId xQueue, uint8_t identifierToFind ,LoRAinReceptionQ_t* correspondingIdentifiers){
	LoRAinReceptionQ_t currentItem;
	UBaseType_t queueSize = uxQueueMessagesWaiting(xQueue);
	int count = 0;

	for (BaseType_t index = 0; index < queueSize; index++){
		if (xQueueReceive(xQueue, &currentItem, 100) == pdPASS) {
			if (isIdentifierMatching(&currentItem, identifierToFind)) {
				correspondingIdentifiers[count]= currentItem;
				count++;
			}
			else {
				xQueueSendToFront(xQueue, &currentItem, 100);
			}
		}
	}
}

int isIdentifierMatching(LoRAinReceptionQ_t* item, uint8_t identifier){
	return item->LMR->header->identifier == identifier;
}
/************************ -------- ************************/
