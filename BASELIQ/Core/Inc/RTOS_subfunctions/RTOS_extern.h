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
#include <stdbool.h>

extern osThreadId ReceiverLoRAHandle;
extern osThreadId SenderLoRaHandle;

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

typedef struct {
	osThreadId task;
	char* name; 						// DEBUG PURPOSE
}NotifyForRFM_IRQ_t;
extern NotifyForRFM_IRQ_t NotifyForRFM_IRQ;

typedef struct {
    bool doDebugging;
    bool doLowEnergy;
    // Ajoutez d'autres paramètres de configuration si nécessaire
} ModuleConfig_t;


 /* ------------ GNSS ------------ */
typedef enum {
    WAIT_FOR_SYNC_1,
    WAIT_FOR_SYNC_2,
	WAIT_FOR_CLASS,
	WAIT_FOR_ID,
    WAIT_FOR_LENGTH_1,
    WAIT_FOR_LENGTH_2,
    RECEIVE_MESSAGE
} UARTState;

typedef enum{
	OK,
	Error
}GNSSReturnStatut;

typedef struct {
	TickType_t Request_TIME;
	uint8_t CLASS;
	uint8_t ID;
	osSemaphoreId applicantSemaphore;
	char* applicantName;					 // DEBUG PURPOSE
} GNSSRequestQ_t;

typedef struct {
	TickType_t Request_TIME;
	TickType_t Return_TIME;
	GNSSReturnStatut statut;
	uint8_t CLASS;
	uint8_t ID;
	UBXMessage_parsed* UBXMessage;
	DynamicBuffer * bufferReturn;
	char* applicantName; 					// DEBUG PURPOSE
} GNSSReturnQ_t;


typedef struct{
    const uint8_t *command;
    size_t size;
	char* applicantName; 					// DEBUG PURPOSE
}GNSStoPollQ_t;


/* ------------ LoRA ------------*/
typedef struct {
	LORA_HeaderforSending* header;
	DynamicBuffer* payload;
	UBXMessage_parsed* UBXMessage;
}LoRAtoSendQ_t;


/* ------------ DEBUG ------------*/
typedef struct {
	char* message;
	char* color;
}UARTdebugQ_t;

#endif /* INC_RTOS_SUBFUNCTIONS_RTOS_EXTERN_H_ */
