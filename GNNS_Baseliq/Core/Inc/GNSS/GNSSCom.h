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
#include "constants.h"

typedef struct {
    uint8_t *buffer;
    size_t size;
} DynamicBuffer;

typedef struct {
	UART_HandleTypeDef* huart;
	UART_HandleTypeDef* huartDebug;

	DynamicBuffer* Rx;

	uint8_t TxBuffer[UART_TX_BUFFER_SIZE];

	uint8_t DebugBuffer[UART_DEBUG_BUFFER_SIZE];

} GNSSCom_HandleTypeDef;
extern GNSSCom_HandleTypeDef hGNSSCom;

typedef struct {
    const uint8_t *command;
    size_t size;
} CommandnSize;

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
DynamicBuffer* initializeBuffer(size_t initialSize);
void GNSSCom_UartActivate(GNSSCom_HandleTypeDef* hGNSS);
void GNSSCom_Send_SetVal(void);
void GNSSCom_ReceiveDebug(void);
void resizeBuffer(DynamicBuffer *buffer, size_t newSize);
void freeBuffer(DynamicBuffer *buffer);
void remove_spaces(const char* str);

#endif /* INC_GNSSCOM_H_ */
