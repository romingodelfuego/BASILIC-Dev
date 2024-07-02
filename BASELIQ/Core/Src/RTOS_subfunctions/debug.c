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
	HAL_UART_Transmit_IT(hGNSSCom.huartDebug, (uint8_t*)buffer, strlen(buffer));
	vPortFree(UARTdebug.message);
	vPortFree(UARTdebug.color);
	vTaskDelay(1);

}

void UART_Transmit_With_Color(char *data, char *color) {
	UARTdebugQ_t UARTdebug;

	// Allouer de la mémoire pour le message et la couleur
	UARTdebug.message = pvPortMalloc(strlen(data) + 1);
	UARTdebug.color = pvPortMalloc(strlen(color) + 1);

	if (UARTdebug.message != NULL && UARTdebug.color != NULL) {
		// Copier les données
		strcpy(UARTdebug.message, data);
		strcpy(UARTdebug.color, color);

		xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
	} else {
		// Gérer l'erreur d'allocation de mémoire
		if (UARTdebug.message == NULL) {
			vPortFree(UARTdebug.message);
			Error_Handler();
		}
		if (UARTdebug.color == NULL) {
			vPortFree(UARTdebug.color);
			Error_Handler();
		}
	}
}
void uint8_array_to_hex_string(char* hexString, uint8_t* array, size_t len) {
	// Allouer de la mémoire pour la chaîne hexadécimale (2 caractères par octet + 1 pour le '\0')

	// Parcourir le tableau et convertir chaque octet en hexadécimal
	for (size_t i = 0; i < len; i++) {
		snprintf(hexString + (i * 2), 3, "%02X", array[i]);
	}

	// Ajouter le caractère de fin de chaîne
	hexString[len * 2] = '\0';

}
