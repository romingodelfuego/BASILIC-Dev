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


extern osThreadId InitTaskHandle;
extern osThreadId ReceivedLORAHandle;
extern osThreadId UARTbytet_to_GNHandle;
extern osThreadId MatcherHandle;
extern osThreadId Fake_SDuseHandle;
extern osMessageQId UARTbyteHandle;
extern osMessageQId UBXQueueHandle;
extern osMessageQId GNSS_RequestHandle;
extern osMessageQId GNSS_ReturnHandle;
extern osSemaphoreId xSem_LORAReceive_startHandle;
extern osSemaphoreId SD_Access_GNSS_ReturnHandle;
extern osSemaphoreId LORA_Access_GNSS_ReturnHandle;

typedef enum{
	OK,
	Error
}GNSSReturnStatut;
typedef struct {
	uint8_t CLASS;
	uint8_t ID;
	osSemaphoreId applicantSemaphore;
	char* applicantName; // DEBUG PURPOSE: Assuming pointer to string
} GNSSRequestQ_t;

typedef struct {
	GNSSReturnStatut statut;
	uint8_t CLASS;
	uint8_t ID;
	DynamicBuffer * bufferReturn;
	char* applicantName; // DEBUG PURPOSE: Assuming pointer to string
} GNSSReturnQ_t;

#endif /* INC_RTOS_SUBFUNCTIONS_RTOS_EXTERN_H_ */
