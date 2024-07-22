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
#include "shared.h"


typedef struct {
    const uint8_t *command;
    size_t size;
} CommandnSize;

typedef enum {
    RAW,
    HEX,
    ASCII
} OutputType;
extern OutputType type;

typedef struct {
	UART_HandleTypeDef* huart;
	UART_HandleTypeDef* huartDebug;
	DynamicBuffer* Rx;
} GNSSCom_HandleTypeDef;
extern GNSSCom_HandleTypeDef hGNSSCom;

void GNSSCom_Init(UART_HandleTypeDef* huart,UART_HandleTypeDef* huartDebug);
DynamicBuffer* initializeBuffer(size_t initialSize);
void GNSSCom_ReceiveDebug(void);

void GNSSCom_MessageAdapter(uint8_t* buffer,size_t* size, GenericMessage* genericMessage);

void resizeBuffer(DynamicBuffer *buffer, size_t newSize);
void freeBuffer(DynamicBuffer *buffer);
void remove_spaces(const char* str);

#endif /* INC_GNSSCOM_H_ */
