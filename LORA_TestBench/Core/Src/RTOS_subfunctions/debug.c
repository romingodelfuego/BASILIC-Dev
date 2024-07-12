/*
 * debug.c
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#include "RTOS_subfunctions/debug.h"
HeapStats_t heapStats;
/************************ TASK ************************/
void debug(void){
	char buffer[512];
	UARTdebugQ_t UARTdebug;

	xQueueReceive(UARTdebugHandle, &UARTdebug, osWaitForever);

	snprintf(buffer, sizeof(buffer), "%s%s%s", UARTdebug.color, UARTdebug.message, ANSI_COLOR_RESET);
	HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer),HAL_MAX_DELAY); //Le IT ne met pas les messages dans le bon ordre
	vPortFree(UARTdebug.message);
	vPortFree(UARTdebug.color);
}
/************************ ---- ************************/
/************************ FUNCTIONS ************************/
void UART_Transmit_With_Color(char *data, char *color) {
	UARTdebugQ_t UARTdebug;

	// Allouer de la mémoire pour le message et la couleur
	UARTdebug.message = pvPortMalloc(strlen(data) + 1);
	UARTdebug.color = pvPortMalloc(strlen(color) + 1);

	if (UARTdebug.message == NULL || UARTdebug.color == NULL) Error_Handler();

	// Copier les données
	strcpy(UARTdebug.message, data);
	strcpy(UARTdebug.color, color);
	xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);

}
void uint8_array_to_hex_string(char* hexString, uint8_t* array, size_t len) {
	for (size_t i = 0; i < len; i++) {	// Parcourir le tableau et convertir chaque octet en hexadécimal
		snprintf(hexString + (i * 2), 3, "%02X", array[i]);
	}
	hexString[len * 2+1] = '\0';	// Ajouter le caractère de fin de chaîne
}

int __io_putchar(int ch) {
	ITM_SendChar(ch);
	return ch;
}

void updateMemoryUsage(void) {
	vPortGetHeapStats(&heapStats);
}
void logMemoryUsage(char*phase) {
	// Obtenir les statistiques de la mémoire
	vPortGetHeapStats(&heapStats);
	 TickType_t systemTime = xTaskGetTickCount() * portTICK_PERIOD_MS;
	// Imprimer les statistiques de mémoire
	printf("\n-----%s-----\tTIME: %lu\n", phase,systemTime);
	printf("Available Heap Space: %u bytes\n", (unsigned int)heapStats.xAvailableHeapSpaceInBytes);
	printf("Largest Free Block: %u bytes\n", (unsigned int)heapStats.xSizeOfLargestFreeBlockInBytes);
	printf("Smallest Free Block: %u bytes\n", (unsigned int)heapStats.xSizeOfSmallestFreeBlockInBytes);
	printf("Number of Free Blocks: %u\n", (unsigned int)heapStats.xNumberOfFreeBlocks);
	printf("Minimum Ever Free Bytes Remaining: %u bytes\n", (unsigned int)heapStats.xMinimumEverFreeBytesRemaining);
	printf("Number of Successful Allocations: %u\n", (unsigned int)heapStats.xNumberOfSuccessfulAllocations);
	printf("Number of Successful Frees: %u\n\n", (unsigned int)heapStats.xNumberOfSuccessfulFrees);
}
/************************ -------- ************************/
