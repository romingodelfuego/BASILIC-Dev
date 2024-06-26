/*
 * debug.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include "RTOS_subfunctions/debug.h"

void UART_Transmit_With_Color(UART_HandleTypeDef *huart, const char *data, const char *color) {
	char buffer[256];

	snprintf(buffer, sizeof(buffer), "%s%s%s", color, data, ANSI_COLOR_RESET);
	HAL_UART_Transmit(huart, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

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
