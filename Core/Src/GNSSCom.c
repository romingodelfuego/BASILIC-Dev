/*
 * GNSSCom.c
 *
 *  Created on: May 23, 2024
 *      Author: romain.pace
 */
#include <GNSSCom.h>

GNSSCom_HandleTypeDef hGNSSCom;
OutputType type = ASCII;
OutputProtocol protocol = NMEA;

void GNSSCom_Init(UART_HandleTypeDef* huart,UART_HandleTypeDef* huartDebug){
	hGNSSCom.huart = huart;
	hGNSSCom.huartDebug = huartDebug;

	hGNSSCom.Rx = initializeBuffer(UART_RX_BUFFER_SIZE);
	memset(hGNSSCom.TxBuffer, 0, UART_TX_BUFFER_SIZE);
	memset(hGNSSCom.DebugBuffer, 0, UART_DEBUG_BUFFER_SIZE);

	GNSSCom_UartActivate(&hGNSSCom);
	HAL_Delay(5000); //En theorie il suffit d attendre la reception du premier msg UART pour envoyer
	GNSSCom_Send_SetVal();
}
DynamicBuffer* initializeBuffer(size_t initialSize) {
    DynamicBuffer *bufferDynamic = malloc(sizeof(DynamicBuffer));
    if (bufferDynamic == NULL) {
        return NULL; // Échec de l'allocation mémoire
    }

    bufferDynamic->buffer = malloc(initialSize);
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
void GNSSCom_UartActivate(GNSSCom_HandleTypeDef* hGNSS){
	HAL_UART_Receive_IT(hGNSS->huart, hGNSS->Rx->buffer, hGNSS->Rx->size);
}
void GNSSCom_Send_SetVal(void){

	CommandnSize commands[] = {
	    {commandSetGNSS_Config, sizeof(commandSetGNSS_Config)},
	    {commandUart1Ouput, sizeof(commandUart1Ouput)},
	    {commandUBXTimeUTC, sizeof(commandUBXTimeUTC)},
	    {commandSetTP, sizeof(commandSetTP)},
	    {commandMeasureRate, sizeof(commandMeasureRate)}
	};
	char message[50];

	for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
	    // Transmit debug message
	    sprintf(message, "\r\t\t\n...Message%d...\r\n", i + 1);
	    HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

	    // Transmit command
	    HAL_UART_Transmit(hGNSSCom.huart, commands[i].command, commands[i].size, HAL_MAX_DELAY);

	    // Receive debug
	    memcpy(hGNSSCom.Rx->buffer, commands[i].command, commands[i].size);
	    GNSSCom_ReceiveDebug();
	}
}
void GNSSCom_ReceiveDebug(){
	// Initialiser la chaîne de sortie à une chaîne vide
	char output_string[UART_DEBUG_BUFFER_SIZE];
	int isUBX = 0;
	for (int i = 0; i < hGNSSCom.Rx->size; i++) {

		if (hGNSSCom.Rx->buffer[i] == HEADER_UBX_1 &&
			hGNSSCom.Rx->buffer[i +1] == HEADER_UBX_2 ){
				//On est sur un message UBX
				isUBX=1;

				UBXMessage_parsed* UbxMessage =(UBXMessage_parsed*) malloc(sizeof(UBXMessage_parsed)) ;
				UbxMessage->msgClass = hGNSSCom.Rx->buffer[i + 2];
				UbxMessage->msgID = hGNSSCom.Rx->buffer[i + 3];
				UbxMessage->len = (hGNSSCom.Rx->buffer[i+5] << 8) |hGNSSCom.Rx->buffer[i+4];
				memcpy(UbxMessage->load, hGNSSCom.Rx->buffer + i + 6, UbxMessage->len);

				create_message_debug(UbxMessage);

				HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*) UbxMessage->bufferDebug, sizeof(UbxMessage->bufferDebug),HAL_MAX_DELAY);
				free(UbxMessage);
		}

		else if (!isUBX){
		switch (hGNSSCom.Rx->buffer[i]) {
		case '\n': // Nouvelle ligne détectée
			strcat(output_string, "\n"); // Ajout d'un saut de ligne à la chaîne de sortie
			break;
		case '\r': // Retour de chariot détecté
			strcat(output_string, "\r");
			break;
		default:
			switch (type) {
			case RAW:
				snprintf(output_string + i, sizeof(output_string) - i, "%d", hGNSSCom.Rx->buffer[i]);
				break;

			case HEX:
				snprintf(output_string +i, sizeof(output_string) - i, "%02X", hGNSSCom.Rx->buffer[i]);
				break;

			case ASCII:
				snprintf(output_string +i, sizeof(output_string) - i, "%c",
						((hGNSSCom.Rx->buffer[i] >= 32 && hGNSSCom.Rx->buffer[i] <= 126)||hGNSSCom.Rx->buffer[i] >= 192) ? hGNSSCom.Rx->buffer[i] : '.');
				break;
			}
			strncat(output_string, " ", sizeof(output_string) - i - 1);

		}
	}
	}
	if (!isUBX){
	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)output_string, strlen(output_string),HAL_MAX_DELAY);
	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)"\r\n", 4,HAL_MAX_DELAY);
	}
}


