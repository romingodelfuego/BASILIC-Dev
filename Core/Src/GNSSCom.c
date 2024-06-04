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

	memcpy(hGNSSCom.RxBuffer, NULL , UART_RX_BUFFER_SIZE);
	memcpy(hGNSSCom.TxBuffer, NULL, UART_TX_BUFFER_SIZE);
	memcpy(hGNSSCom.DebugBuffer, NULL, UART_DEBUG_BUFFER_SIZE);
	GNSSCom_UartActivate(&hGNSSCom);
	HAL_Delay(5000); //En theorie il suffit d attendre la reception du premier msg UART pour envoyer
	GNSSCom_Send_SetVal();
}

void GNSSCom_UartActivate(GNSSCom_HandleTypeDef* hGNSS){
	HAL_UART_Receive_IT(hGNSS->huart, hGNSS->RxBuffer, UART_RX_BUFFER_SIZE);
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
	memcpy(hGNSSCom.RxBuffer,commandSetGNSS_Config,sizeof(commandSetGNSS_Config));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message2,sizeof(message2),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandUart1Ouput,sizeof(commandUart1Ouput),HAL_MAX_DELAY);
	memcpy(hGNSSCom.RxBuffer,commandUart1Ouput,sizeof(commandUart1Ouput));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message3,sizeof(message3),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandUBXTimeUTC,sizeof(commandUBXTimeUTC),HAL_MAX_DELAY);
	memcpy(hGNSSCom.RxBuffer,commandUBXTimeUTC,sizeof(commandUBXTimeUTC));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message3,sizeof(message3),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandSetTP,sizeof(commandSetTP),HAL_MAX_DELAY);
	memcpy(hGNSSCom.RxBuffer,commandSetTP,sizeof(commandSetTP));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message4,sizeof(message4),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandMeasureRate,sizeof(commandMeasureRate),HAL_MAX_DELAY);
	memcpy(hGNSSCom.RxBuffer,commandMeasureRate,sizeof(commandMeasureRate));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)messagetest,sizeof(messagetest),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, test_UBXNav_TIMEUTC,sizeof(test_UBXNav_TIMEUTC),HAL_MAX_DELAY);
	memcpy(hGNSSCom.RxBuffer,test_UBXNav_TIMEUTC,sizeof(test_UBXNav_TIMEUTC));
	GNSSCom_ReceiveDebug();

}

void GNSSCom_ReceiveDebug(){
	// Initialiser la chaîne de sortie à une chaîne vide
	char output_string[UART_DEBUG_BUFFER_SIZE];
	for (int i = 0; i < UART_RX_BUFFER_SIZE; i++) {

		if (hGNSSCom.RxBuffer[i] == HEADER_CheckValue1 &&
			hGNSSCom.RxBuffer[i +1] == HEADER_CheckValue2 ){
				//On est sur un message UBX
				int len = (hGNSSCom.RxBuffer[i+5] << 8) |hGNSSCom.RxBuffer[i+4];
				UBXMessage_parsed* UbxMessage =(UBXMessage_parsed*) malloc(sizeof(UBXMessage_parsed)) ;

				UbxMessage->msgClass = hGNSSCom.RxBuffer[i + 2];
				UbxMessage->msgID = hGNSSCom.RxBuffer[i + 3];
				UbxMessage->len = len;
				memcpy(UbxMessage->load, hGNSSCom.RxBuffer + i + 6, len);
				create_message_debug(UbxMessage); //On obtient l'adresse de la structure qui correspond au message
				//Maintenant pour pouvoir utilsier ici la structure il nous faut savoir quelle type de structure est elle
				HAL_UART_Transmit(hGNSSCom.huartDebug, UbxMessage->bufferDebug, sizeof(UbxMessage->bufferDebug),HAL_MAX_DELAY);
				i+=6+len;
		}

		else{
		switch (hGNSSCom.RxBuffer[i]) {
		case '\n': // Nouvelle ligne détectée
			strcat(output_string, "\n"); // Ajout d'un saut de ligne à la chaîne de sortie
			break;
		case '\r': // Retour de chariot détecté
			strcat(output_string, "\r");
			break;
		default:
			switch (type) {
			case RAW:
				snprintf(output_string + i, sizeof(output_string) - i, "%d", hGNSSCom.RxBuffer[i]);
				break;

			case HEX:
				snprintf(output_string +i, sizeof(output_string) - i, "%02X", hGNSSCom.RxBuffer[i]);
				break;

			case ASCII:
				snprintf(output_string +i, sizeof(output_string) - i, "%c",
						((hGNSSCom.RxBuffer[i] >= 32 && hGNSSCom.RxBuffer[i] <= 126)||hGNSSCom.RxBuffer[i] >= 192) ? hGNSSCom.RxBuffer[i] : '.');
				break;
			}
			strncat(output_string, " ", sizeof(output_string) - i - 1);

		}
	}
	}
	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)output_string, strlen(output_string),HAL_MAX_DELAY);
	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)"\r\n", 4,HAL_MAX_DELAY);
}


