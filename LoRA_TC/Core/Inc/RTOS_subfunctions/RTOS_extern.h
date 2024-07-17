/*
 * RTOS_extern.h
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#ifndef INC_RTOS_SUBFUNCTIONS_RTOS_EXTERN_H_
#define INC_RTOS_SUBFUNCTIONS_RTOS_EXTERN_H_

//WHO AM I
#define MODULE_SOURCE_ADDRESS 0xFE
#define MODULE_BROADCAST_ADDRESS 0xFF
// PAQUET TYPE
#define PACKET_TYPE_DATA 0x01
#define PACKET_TYPE_ACK  0x02
#define PACKET_TYPE_POLL 0x03


#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "debug.h"
#include "shared.h"
#include "GNSS/GNSSCom.h"

extern osThreadId StartInitHandle;
extern osThreadId SenderLoRAHandle;
extern osThreadId UartDebugHandle;
extern osThreadId ReceiverLoRAHandle;

extern osMessageQId LoRA_toSendHandle;
extern osMessageQId UARTdebugHandle;
extern osMessageQId LoRA_inReceptionHandle;

extern osSemaphoreId xSem_LORAReceive_startHandleHandle;


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
	LORA_HeaderforReception* header;
	DynamicBuffer* payload;
}LoRAtoSendQ_t;

typedef struct {
	LORA_HeaderforSending* header;
	uint8_t* payload;
}LoRAinReceptionQ_t;
#endif /* INC_RTOS_SUBFUNCTIONS_RTOS_EXTERN_H_ */
