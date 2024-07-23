/*
 * debug_utils.c
 *
 *  Created on: Jul 23, 2024
 *      Author: romain.pace
 */
#include "GNSS/debug_utils.h"

char* uint8ArrayToString(uint8_t* array, size_t size) {
	// Calculer la valeur totale en interprétant le tableau comme un entier
	uint64_t totalValue = 0;
	for (size_t i = 0; i < size; i++) {
		totalValue |= ((uint64_t)array[i]) << (i * 8);
	}

	// Calculer la taille nécessaire pour stocker la représentation en chaîne de la valeur totale
	// La taille maximale en caractères pour un uint64_t en base 10 est 20, plus le null terminator
	char* str = pvPortMalloc(21);

	// Convertir la valeur totale en chaîne de caractères
	//snprintf(str, 21, "%llu", totalValue);
	itoa(totalValue, str, 10); // Base 10 pour les décimales
	return str;
}
char* int8ArrayToString(int8_t* array, size_t size){
	// Calculer la valeur totale en interprétant le tableau comme un entier
	uint64_t totalValue = 0;
	for (size_t i = 0; i < size; i++) {
		totalValue |= ((uint64_t)array[i]) << (i * 8);
	}

	// Calculer la taille nécessaire pour stocker la représentation en chaîne de la valeur totale
	// La taille maximale en caractères pour un uint64_t en base 10 est 20, plus le null terminator
	char* str = pvPortMalloc(21);

	// Convertir la valeur totale en chaîne de caractères
	//snprintf(str, 21, "%llu", totalValue);
	itoa(totalValue, str, 10); // Base 10 pour les décimales
	return str;
}


void formattedString(char* buffer, size_t* offset, int numArgs, bool freeMemory,bool isThereSpecFormat,  const char* format,...){
	va_list args;
	va_start(args, format);
	*offset += vsnprintf(buffer + *offset, MAX_BUFFER_SIZE - *offset, format, args);
	va_end(args);

	if (freeMemory){
		va_start(args, format);
		for (int i = 0; i < numArgs; i++) {
			void* arg = va_arg(args, void*);
			if (isThereSpecFormat){
				if (i%2) vPortFree(arg); // SUPPRIME LE pvPortMalloc de uint8ArrayToString
			}
			else{
				vPortFree(arg);
			}
			updateMemoryUsage();
		}
		va_end(args);
	}
}





