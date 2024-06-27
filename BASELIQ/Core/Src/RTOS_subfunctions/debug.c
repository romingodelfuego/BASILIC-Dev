/*
 * debug.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include "RTOS_subfunctions/debug.h"

void debug(void){
	char buffer[1024];
	UARTdebugQ_t UARTdebug;

	xQueueReceive(UARTdebugHandle, &UARTdebug, osWaitForever);

	snprintf(buffer, sizeof(buffer), "%s%s%s", UARTdebug.color, UARTdebug.message, ANSI_COLOR_RESET);
	HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

}

void UART_Transmit_With_Color(UART_HandleTypeDef *huart, char *data, char *color) {
	UARTdebugQ_t UARTdebug = {.message = data,
			.color=color};
	xQueueSendToBack(UARTdebugHandle,&UARTdebug,100);
}
char* uint8_array_to_hex_string(uint8_t* array, size_t len) {
	// Allouer de la mémoire pour la chaîne hexadécimale (2 caractères par octet + 1 pour le '\0')
	char* hexString = (char*)pvPortMalloc(len * 2 + 1);

	if (hexString == NULL) {
		// Gérer l'échec d'allocation de mémoire
		return NULL;
	}

	// Parcourir le tableau et convertir chaque octet en hexadécimal
	for (size_t i = 0; i < len; i++) {
		snprintf(hexString + (i * 2), 3, "%02X", array[i]);
	}

	// Ajouter le caractère de fin de chaîne
	hexString[len * 2] = '\0';

	return hexString;
}
