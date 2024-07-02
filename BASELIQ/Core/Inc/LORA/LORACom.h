/*
 * LORACom.h
 *
 *  Created on: Jun 7, 2024
 *      Author: romain.pace
 */

#ifndef INC_LORA_LORACOM_H_
#define INC_LORA_LORACOM_H_



#include <shared.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h> // Incluez les bibliothèques standard nécessaires
#include "spi.h"
#include "usart.h"
#include "shared.h"

//WHO AM I
#define MODULE_SOURCE_ADDRESS 0x01
#define MODULE_BROADCAST_ADDRESS 0xFF
// PAQUE TYPE
#define PACKET_TYPE_DATA 0x01
#define PACKET_TYPE_ACK  0x02
#define PACKET_TYPE_POLL 0x03

#define MAX_SPI_SIZE 300

typedef struct {
	SPI_HandleTypeDef* hspi;
	UART_HandleTypeDef* huartDebug;

} LORACom_HandleTypeDef;
extern LORACom_HandleTypeDef hLORACom;


void LORACom_Init(SPI_HandleTypeDef* hspi,UART_HandleTypeDef* huartDebug);
void LORACom_SPIActivate(LORACom_HandleTypeDef* hLORA);
void LORA_Send(LORA_Header* header, uint8_t* payload);
void LORA_debug(char* flag, uint8_t* value);
void LORA_debug_val(const char* flag, uint8_t value);
void LORA_debug_hexa(char* flag, uint8_t* value, uint8_t len);
void RF_TestSpi( void );
void print1(const char *text, uint8_t x);
void print(const char *text);
#endif /* INC_LORA_LORACOM_H_ */
