/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "RTOS_subfunctions/RTOS_extern.h"
#include "RTOS_subfunctions/debug.h"
#include "RTOS_subfunctions/receiverLoRA.h"
#include "RTOS_subfunctions/senderLoRA.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId StartInitHandle;
osThreadId SenderLoRAHandle;
osThreadId ReceiverLoRAHandle;
osThreadId DebugHandle;
osMessageQId LoRA_toSendHandle;
osMessageQId UARTdebugHandle;
osMessageQId LoRA_inReceptionHandle;
osSemaphoreId xSem_LORAReceive_startHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInitHandle_TASK(void const * argument);
void SenderLoRA_TASK(void const * argument);
void ReceiverLoRA_TASK(void const * argument);
void Debug_TASK(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
	return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of xSem_LORAReceive_start */
  osSemaphoreDef(xSem_LORAReceive_start);
  xSem_LORAReceive_startHandle = osSemaphoreCreate(osSemaphore(xSem_LORAReceive_start), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of LoRA_toSend */
  osMessageQDef(LoRA_toSend, 4, LoRAtoSendQ_t);
  LoRA_toSendHandle = osMessageCreate(osMessageQ(LoRA_toSend), NULL);

  /* definition and creation of UARTdebug */
  osMessageQDef(UARTdebug, 16, UARTdebugQ_t);
  UARTdebugHandle = osMessageCreate(osMessageQ(UARTdebug), NULL);

  /* definition and creation of LoRA_inReception */
  osMessageQDef(LoRA_inReception, 48, LoRAinReceptionQ_t);
  LoRA_inReceptionHandle = osMessageCreate(osMessageQ(LoRA_inReception), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of StartInit */
  osThreadDef(StartInit, StartInitHandle_TASK, osPriorityRealtime, 0, 512);
  StartInitHandle = osThreadCreate(osThread(StartInit), NULL);

  /* definition and creation of SenderLoRA */
  osThreadDef(SenderLoRA, SenderLoRA_TASK, osPriorityNormal, 0, 512);
  SenderLoRAHandle = osThreadCreate(osThread(SenderLoRA), NULL);

  /* definition and creation of ReceiverLoRA */
  osThreadDef(ReceiverLoRA, ReceiverLoRA_TASK, osPriorityNormal, 0, 2048);
  ReceiverLoRAHandle = osThreadCreate(osThread(ReceiverLoRA), NULL);

  /* definition and creation of Debug */
  osThreadDef(Debug, Debug_TASK, osPriorityBelowNormal, 0, 256);
  DebugHandle = osThreadCreate(osThread(Debug), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartInitHandle_TASK */
/**
 * @brief  Function implementing the StartInit thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInitHandle_TASK */
void StartInitHandle_TASK(void const * argument)
{
  /* USER CODE BEGIN StartInitHandle_TASK */
	/* Infinite loop */
	const char startMessage[] = "\r\nStarting...\r\n";
	const char initDoneMessage[] = "\r\nInit Done\r\n\n";

	HAL_UART_Transmit(&huart2, (uint8_t *)startMessage, sizeof(startMessage), 10);

	LORACom_Init(&hspi2,&huart2);
	RFM9x_Init();

	vTaskDelay(1000);

	HAL_UART_Transmit(&huart2, (uint8_t *)initDoneMessage, sizeof(initDoneMessage), 10);

	logMemoryUsage("INITILISATION");
	osSignalSet(ReceiverLoRAHandle, 0x01);
	osSignalSet(SenderLoRAHandle, 0x01);
	osStatus event = osThreadTerminate(StartInitHandle);
	if (event != osOK)Error_Handler();
  /* USER CODE END StartInitHandle_TASK */
}

/* USER CODE BEGIN Header_SenderLoRA_TASK */
/**
 * @brief Function implementing the SenderLoRA thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_SenderLoRA_TASK */
void SenderLoRA_TASK(void const * argument)
{
  /* USER CODE BEGIN SenderLoRA_TASK */
	/* Infinite loop */
	uint8_t pollingStatutCommand [] = {0xb5, 0x62, 0x01, 0x43, 0x00, 0x00, 0x44, 0xcd};
	osEvent eventFromStart = osSignalWait(0x01, osWaitForever);
	if (eventFromStart.status == osEventSignal){
		for(;;)
		{
			logMemoryUsage("START - Lora Sender TASK");
			DynamicBuffer* payloadForPolling =(DynamicBuffer*)initializeBuffer(sizeof(pollingStatutCommand));
			LORA_HeaderforReception* headerForPolling = (LORA_HeaderforReception*)pvPortMalloc(sizeof(LORA_HeaderforReception));
			if (headerForPolling == NULL) Error_Handler();
			logMemoryUsage("MEMCPY - Lora Sender TASK");

			memcpy(payloadForPolling->buffer, pollingStatutCommand, payloadForPolling->size);

			*headerForPolling = (LORA_HeaderforReception){
				.recipient = MODULE_BROADCAST_ADDRESS,
						.sender = MODULE_SOURCE_ADDRESS,
						.type = PACKET_TYPE_POLL,
						.len_payload = (uint8_t)payloadForPolling->size
			};

			LoRAtoSendQ_t pollingStatutGNSS = (LoRAtoSendQ_t){
				.header = headerForPolling,
						.payload = payloadForPolling
			};
			xQueueSendToBack(LoRA_toSendHandle,&pollingStatutGNSS,osWaitForever);

			senderLoRA();

			freeBuffer(payloadForPolling);
			vPortFree(headerForPolling);
			logMemoryUsage("END - Lora Sender TASK");

			vTaskDelay(10000);
		}
	}
  /* USER CODE END SenderLoRA_TASK */
}

/* USER CODE BEGIN Header_ReceiverLoRA_TASK */
/**
 * @brief Function implementing the ReceiverLoRA thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ReceiverLoRA_TASK */
void ReceiverLoRA_TASK(void const * argument)
{
  /* USER CODE BEGIN ReceiverLoRA_TASK */
	/* Infinite loop */
	osEvent eventFromStart = osSignalWait(0x01, osWaitForever);
	if (eventFromStart.status == osEventSignal){
		for(;;)
		{
			osSemaphoreWait(xSem_LORAReceive_startHandle, osWaitForever);
			receivedLora();
		}
	}
  /* USER CODE END ReceiverLoRA_TASK */
}

/* USER CODE BEGIN Header_Debug_TASK */
/**
 * @brief Function implementing the Debug thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Debug_TASK */
void Debug_TASK(void const * argument)
{
  /* USER CODE BEGIN Debug_TASK */
	/* Infinite loop */
	for(;;)
	{
		debug();
	}
  /* USER CODE END Debug_TASK */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
