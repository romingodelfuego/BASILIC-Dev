/*
 * GNSSCom.h
 *
 *  Created on: May 23, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSSCOM_H_
#define INC_GNSSCOM_H_

#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "command.h"
#include "traductor.h"
#include "UBXParser.h"

#define UART_RX_BUFFER_SIZE 50
#define UART_TX_BUFFER_SIZE 50
#define UART_DEBUG_BUFFER_SIZE 200

#define HEADER_CheckValue1 0xB5
#define HEADER_CheckValue2 0x62

typedef struct {
	UART_HandleTypeDef* huart;
	UART_HandleTypeDef* huartDebug;

	uint8_t RxBuffer[UART_RX_BUFFER_SIZE];
	uint8_t TxBuffer[UART_TX_BUFFER_SIZE];

	uint8_t DebugBuffer[UART_DEBUG_BUFFER_SIZE];
	uint8_t uart_RX_error;
} GNSSCom_HandleTypeDef;
extern GNSSCom_HandleTypeDef hGNSSCom;

typedef enum {
    RAW,
    HEX,
    ASCII
} OutputType;
extern OutputType type; //=ASCII
typedef enum {
    NMEA,
    UBX,
} OutputProtocol;

extern OutputProtocol protocol;// = NMEA;

void GNSSCom_Init(UART_HandleTypeDef* huart,UART_HandleTypeDef* huartDebug);
void GNSSCom_UartActivate(GNSSCom_HandleTypeDef* hGNSS);
void GNSSCom_ReceiveDebug(void);

void GNSSCom_Send_SetVal(void);
void remove_spaces(const char* str);

#endif /* INC_GNSSCOM_H_ */
