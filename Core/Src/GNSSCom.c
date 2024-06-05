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

	memcpy(hGNSSCom.TxBuffer, NULL, UART_TX_BUFFER_SIZE);
	memcpy(hGNSSCom.DebugBuffer, NULL, UART_DEBUG_BUFFER_SIZE);
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
    char *newData = realloc(bufferDynamic->buffer, newSize);
    if (newData == NULL) {
        return 0; // Échec de la réallocation mémoire
    }

    bufferDynamic->buffer = newData;
    bufferDynamic->size = newSize;
    return 1; // Réallocation réussie
}
void freeBuffer(DynamicBuffer *bufferDynamic) {
    free(bufferDynamic->buffer);
    free(bufferDynamic);
}
void GNSSCom_UartActivate(GNSSCom_HandleTypeDef* hGNSS){
	HAL_UART_Receive_IT(hGNSS->huart, hGNSS->Rx->buffer, hGNSS->Rx->size);
}

void GNSSCom_Send_SetVal(void){
	const char message1[] = "\r\t\t\n...Message1...\r\n";
	const char message2[] = "\r\t\t\n...Message2...\r\n";
	const char message3[] = "\r\t\t...Message3...\r\n";
	const char message4[] = "\r\t\t...Message4...\r\n";
	//const char message5[] = "\r\t\t...Message5...\r\n";
	const char messagetest[] = "\r\t\t...Message TEST...\r\n";
	//const char messageEnd[] = "\r\t\t...END...\r\n";
	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message1,sizeof(message1),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandSetGNSS_Config,sizeof(commandSetGNSS_Config),HAL_MAX_DELAY);
	memcpy(hGNSSCom.Rx->buffer,commandSetGNSS_Config,sizeof(commandSetGNSS_Config));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message2,sizeof(message2),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandUart1Ouput,sizeof(commandUart1Ouput),HAL_MAX_DELAY);
	memcpy(hGNSSCom.Rx->buffer,commandUart1Ouput,sizeof(commandUart1Ouput));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message3,sizeof(message3),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandUBXTimeUTC,sizeof(commandUBXTimeUTC),HAL_MAX_DELAY);
	memcpy(hGNSSCom.Rx->buffer,commandUBXTimeUTC,sizeof(commandUBXTimeUTC));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message3,sizeof(message3),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandSetTP,sizeof(commandSetTP),HAL_MAX_DELAY);
	memcpy(hGNSSCom.Rx->buffer,commandSetTP,sizeof(commandSetTP));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message4,sizeof(message4),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandMeasureRate,sizeof(commandMeasureRate),HAL_MAX_DELAY);
	memcpy(hGNSSCom.Rx->buffer,commandMeasureRate,sizeof(commandMeasureRate));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)messagetest,sizeof(messagetest),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, test_UBXNav_TIMEUTC,sizeof(test_UBXNav_TIMEUTC),HAL_MAX_DELAY);
	memcpy(hGNSSCom.Rx->buffer,test_UBXNav_TIMEUTC,sizeof(test_UBXNav_TIMEUTC));
	GNSSCom_ReceiveDebug();

}

void GNSSCom_ReceiveDebug(){
	// Initialiser la chaîne de sortie à une chaîne vide
	char output_string[UART_DEBUG_BUFFER_SIZE];
	int isUBX=0;
	for (int i = 0; i < UART_RX_BUFFER_SIZE; i++) {

		if (hGNSSCom.Rx->buffer[i] == HEADER_UBX_1 &&
			hGNSSCom.Rx->buffer[i +1] == HEADER_UBX_2 ){
				//On est sur un message UBX
				isUBX=1;
				int len = (hGNSSCom.Rx->buffer[i+5] << 8) |hGNSSCom.Rx->buffer[i+4];
				UBXMessage_parsed* UbxMessage =(UBXMessage_parsed*) malloc(sizeof(UBXMessage_parsed)) ;

				UbxMessage->msgClass = hGNSSCom.Rx->buffer[i + 2];
				UbxMessage->msgID = hGNSSCom.Rx->buffer[i + 3];
				UbxMessage->len = len;
				memcpy(UbxMessage->load, hGNSSCom.Rx->buffer + i + 6, len);
				create_message_debug(UbxMessage); //On obtient l'adresse de la structure qui correspond au message
				//Maintenant pour pouvoir utilsier ici la structure il nous faut savoir quelle type de structure est elle
				HAL_UART_Transmit(hGNSSCom.huartDebug, UbxMessage->bufferDebug, sizeof(UbxMessage->bufferDebug),HAL_MAX_DELAY);
				i+=6+len;
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


