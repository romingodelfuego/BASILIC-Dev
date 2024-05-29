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
	//const char messageEnd[] = "\r\t\t...END...\r\n";
	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message1,sizeof(message1),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandSetTP,sizeof(commandSetTP),HAL_MAX_DELAY);
	memcpy(hGNSSCom.DebugBuffer,commandSetTP,sizeof(commandSetTP));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message2,sizeof(message2),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandUart1Ouput,sizeof(commandUart1Ouput),HAL_MAX_DELAY);
	memcpy(hGNSSCom.DebugBuffer,commandUart1Ouput,sizeof(commandUart1Ouput));
	GNSSCom_ReceiveDebug();

	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message3,sizeof(message3),HAL_MAX_DELAY);

	HAL_UART_Transmit(hGNSSCom.huart, commandUBXTimeUTC,sizeof(commandUBXTimeUTC),HAL_MAX_DELAY);
	memcpy(hGNSSCom.DebugBuffer,commandUBXTimeUTC,sizeof(commandUBXTimeUTC));
	GNSSCom_ReceiveDebug();

}

void GNSSCom_ReceiveDebug(){
	// Initialiser la chaîne de sortie à une chaîne vide
	char output_string[UART_DEBUG_BUFFER_SIZE];
	int bool_UBXflag = 0;
	for (int i = 0; i < sizeof(hGNSSCom.DebugBuffer); i++) {


		if (hGNSSCom.DebugBuffer[i] == HEADER_CheckValue1 || bool_UBXflag ){
			bool_UBXflag = 1;
			if (hGNSSCom.DebugBuffer[i] == HEADER_CheckValue2 ){
				//On est sur un message UBX
				bool_UBXflag =0;
				UBXMessage_parsed UbxMessage;
				UbxMessage.msgClass = hGNSSCom.DebugBuffer[i+1];
				UbxMessage.msgID = hGNSSCom.DebugBuffer[i+2];
				UbxMessage.len = (hGNSSCom.DebugBuffer[i+4] << 8) |hGNSSCom.DebugBuffer[i+3];

				memcpy(UbxMessage.load, hGNSSCom.DebugBuffer + 5 ,UbxMessage.len);
				void *structure_ptr = create_message_struct(UbxMessage); //On obtient l'adresse de la structure qui correspond au message
				//Maintenant pour pouvoir utilsier ici la structure il nous faut savoir quelle type de structure est elle


				HAL_Delay(5);
			}
		}

		switch (hGNSSCom.DebugBuffer[i]) {
		case '\n': // Nouvelle ligne détectée
			strcat(output_string, "\n"); // Ajout d'un saut de ligne à la chaîne de sortie
			break;
		case '\r': // Retour de chariot détecté
			strcat(output_string, "\r");
			break; // Ignorer le caractère de retour de chariot
		default:
			switch (type) {
			case RAW:
				snprintf(output_string + strlen(output_string), sizeof(output_string) - strlen(output_string), "%d", hGNSSCom.DebugBuffer[i]);
				break;

			case HEX:
				snprintf(output_string + strlen(output_string), sizeof(output_string) - strlen(output_string), "%02X", hGNSSCom.DebugBuffer[i]);
				break;

			case ASCII:
				snprintf(output_string + strlen(output_string), sizeof(output_string) - strlen(output_string), "%c",
						(hGNSSCom.DebugBuffer[i] >= 32 && hGNSSCom.DebugBuffer[i] <= 126) ? hGNSSCom.DebugBuffer[i] : '.');
				break;
			}

			if (i < sizeof(hGNSSCom.DebugBuffer) - 1) {
				strncat(output_string, " ", sizeof(output_string) - strlen(output_string) - 1);
			}
		}
	}
	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)output_string, strlen(output_string),HAL_MAX_DELAY);
}


