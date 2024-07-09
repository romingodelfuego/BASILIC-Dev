/*
 * RTOS_extern.h
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#ifndef INC_RTOS_SUBFUNCTIONS_RTOS_EXTERN_H_
#define INC_RTOS_SUBFUNCTIONS_RTOS_EXTERN_H_

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "debug.h"
#include "shared.h"



extern osMessageQId UARTbyteHandle;
extern osMessageQId UBXQueueHandle;
extern osMessageQId GNSS_RequestHandle;
extern osMessageQId GNSS_ReturnHandle;
extern osMessageQId UARTdebugHandle;
extern osMessageQId GNSS_toPollHandle;
extern osMessageQId LoRA_toSendHandle;

extern osSemaphoreId xSem_LORAReceive_startHandle;
extern osSemaphoreId SD_Access_GNSS_ReturnHandle;
extern osSemaphoreId LORA_Access_GNSS_ReturnHandle;
extern osSemaphoreId GNSS_UART_AccessHandle;
extern osSemaphoreId xSem_GNSS_InitHandle;


typedef enum{
	OK,
	Error
}GNSSReturnStatut;
typedef enum {
    WAIT_FOR_SYNC_1,
    WAIT_FOR_SYNC_2,
	WAIT_FOR_CLASS,
	WAIT_FOR_ID,
    WAIT_FOR_LENGTH_1,
    WAIT_FOR_LENGTH_2,
    RECEIVE_MESSAGE
} UARTState;

typedef struct {
	TickType_t Request_TIME;
	uint8_t CLASS;
	uint8_t ID;
	osSemaphoreId applicantSemaphore;
	char* applicantName; // DEBUG PURPOSE: Assuming pointer to string
} GNSSRequestQ_t;

typedef struct {
	TickType_t Request_TIME;
	TickType_t Return_TIME;
	GNSSReturnStatut statut;
	uint8_t CLASS;
	uint8_t ID;
	UBXMessage_parsed* UBXMessage;
	DynamicBuffer * bufferReturn;
	char* applicantName; // DEBUG PURPOSE: Assuming pointer to string
} GNSSReturnQ_t;

typedef struct {
	char* message;
	char* color;
}UARTdebugQ_t;

typedef struct{ //Copie assumer de CommandnSize
    const uint8_t *command;
    size_t size;
	char* applicantName; // DEBUG PURPOSE: Assuming pointer to string
}GNSStoPollQ_t;


typedef struct {
	LORA_HeaderforSending* header;
	DynamicBuffer* payload;
	UBXMessageQ_t* itemFromUBXtoFree;
	UBXMessage_parsed* UBXMessage;
}LoRAtoSendQ_t;
#endif /* INC_RTOS_SUBFUNCTIONS_RTOS_EXTERN_H_ */
