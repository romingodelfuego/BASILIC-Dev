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
GenericMessage* receptionGNSS;
/* USER CODE END Variables */
osThreadId InitTaskHandle;
osThreadId ReceivedGNSSHandle;
osThreadId ReceivedLORAHandle;
osSemaphoreId xSem_GNSSReceive_startHandle;
osSemaphoreId xSem_LORAReceive_startHandle;
osSemaphoreId xSem_GNSSReceive_endHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInitTask(void const * argument);
void ReceivedGNSSTask(void const * argument);
void ReceivedLORATask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

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

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
	*ppxTimerTaskStackBuffer = &xTimerStack[0];
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
	/* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

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
	/* definition and creation of xSem_GNSSReceive_start */
	osSemaphoreDef(xSem_GNSSReceive_start);
	xSem_GNSSReceive_startHandle = osSemaphoreCreate(osSemaphore(xSem_GNSSReceive_start), 1);

	/* definition and creation of xSem_LORAReceive_start */
	osSemaphoreDef(xSem_LORAReceive_start);
	xSem_LORAReceive_startHandle = osSemaphoreCreate(osSemaphore(xSem_LORAReceive_start), 1);

	/* definition and creation of xSem_GNSSReceive_end */
	osSemaphoreDef(xSem_GNSSReceive_end);
	xSem_GNSSReceive_endHandle = osSemaphoreCreate(osSemaphore(xSem_GNSSReceive_end), 1);

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of InitTask */
	osThreadDef(InitTask, StartInitTask, osPriorityHigh, 0, 1024);
	InitTaskHandle = osThreadCreate(osThread(InitTask), NULL);

	/* definition and creation of ReceivedGNSS */
	osThreadDef(ReceivedGNSS, ReceivedGNSSTask, osPriorityAboveNormal, 0, 2048);
	ReceivedGNSSHandle = osThreadCreate(osThread(ReceivedGNSS), NULL);

	/* definition and creation of ReceivedLORA */
	osThreadDef(ReceivedLORA, ReceivedLORATask, osPriorityAboveNormal, 0, 2048);
	ReceivedLORAHandle = osThreadCreate(osThread(ReceivedLORA), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartInitTask */
/**
 * @brief  Function implementing the InitTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInitTask */
void StartInitTask(void const * argument)
{
	/* USER CODE BEGIN StartInitTask */
	/* Infinite loop */
	//  for(;;)
	//  {
	const char startMessage[] = "\r\nStarting...\r\n";
	const char initDoneMessage[] = "\r\nInit Done\r\n\n";

	HAL_UART_Transmit(&huart1, (uint8_t *)startMessage, sizeof(startMessage), 10);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4,GPIO_PIN_SET);
	__enable_irq();
	GNSSCom_Init(&huart3,&huart1);
	LORACom_Init(&hspi2, &huart1);
	RFM9x_Init();
	receptionGNSS->typeMessage=INIT;
	HAL_UART_Transmit(&huart1, (uint8_t *)initDoneMessage, sizeof(initDoneMessage), 10);

	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
	osThreadTerminate(InitTaskHandle);
	//  }
	/* USER CODE END StartInitTask */
}

/* USER CODE BEGIN Header_ReceivedGNSSTask */
/**
 * @brief Function implementing the ReceivedGNSS thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ReceivedGNSSTask */
void ReceivedGNSSTask(void const * argument)
{
	/* USER CODE BEGIN ReceivedGNSSTask */
	/* Infinite loop */
	for(;;)
	{
		osSemaphoreWait(xSem_GNSSReceive_startHandle, osWaitForever);
		ITM_Port32(31)=12345;
		//	TODO : Filtrer la partie envoie et la partie debug
		//	Si on veut debug alors quel messsage on debug
		//	Si on veut transferer alors quel message (class,id)

		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		receptionGNSS = GNSSCom_Receive(hGNSSCom.Rx->buffer,hGNSSCom.Rx->size);

		if (receptionGNSS->typeMessage == UBX){
			ITM_Port32(31)=56789;
			xSemaphoreGiveFromISR(xSem_GNSSReceive_endHandle, &xHigherPriorityTaskWoken);
			osDelay(1);
		}
		if(receptionGNSS->typeMessage == NMEA){
			// Il est possible d'en avoir mais pas utile
		}
		GNSSCom_UartActivate(&hGNSSCom);
		osDelay(1);
	}
	/* USER CODE END ReceivedGNSSTask */
}

/* USER CODE BEGIN Header_ReceivedLORATask */
/**
 * @brief Function implementing the ReceivedLORA thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ReceivedLORATask */
void ReceivedLORATask(void const * argument)
{
	/* USER CODE BEGIN ReceivedLORATask */
	/* Infinite loop */
	for(;;)
	{
		osSemaphoreWait(xSem_LORAReceive_startHandle, osWaitForever);

		LORA_Message* LORA_Receive_Message = (LORA_Message*)malloc(sizeof(LORA_Message));
		RFM9x_Receive(LORA_Receive_Message);
		if (!LORA_Receive_Message->RxNbrBytes){ITM_Port32(31)=666;} //Si on recoit du bruit
		else
			if (LORA_Receive_Message->header->recipient == MODULE_BROADCAST_ADDRESS
					||LORA_Receive_Message->header->recipient == MODULE_SOURCE_ADDRESS){

				Header* headerSend =(Header*) malloc(sizeof(Header));
				switch (LORA_Receive_Message->header->type){

				case PACKET_TYPE_ACK:
					//Lora send un messsage vide
					*headerSend = (Header){
						.recipient = 0xFE,
								.sender = MODULE_SOURCE_ADDRESS,
								.type = PACKET_TYPE_ACK,
								.len_payload = sizeof(NULL)
					};
					LORA_Send(headerSend, NULL);
					free(headerSend);
					break;


				case PACKET_TYPE_POLL:
					//il faut que le gnss poll
					CommandnSize poll = {(const uint8_t*) LORA_Receive_Message->payload,
							(size_t) LORA_Receive_Message->header->len_payload};

					char message[50];
					sprintf(message, "\r\t\t\n...UBXMessage --FROM-- LORA Polling...\r\n");
					HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
					ITM_Port32(31)=11;
					osSemaphoreWait(xSem_GNSSReceive_endHandle, osWaitForever);//on attend la fin d'une potenitel reception
					GNSSCom_Send_SetVal(poll); //On envoie un message
					osSemaphoreWait(xSem_GNSSReceive_endHandle, osWaitForever); //donc on attend la 1ere reponse
					ITM_Port32(31)=22;
					if(	receptionGNSS->typeMessage!=UBX ||
							receptionGNSS->Message.UBXMessage->msgClass != LORA_Receive_Message->payload[2]||
							receptionGNSS->Message.UBXMessage->msgID !=LORA_Receive_Message->payload[3])
					{
						osSemaphoreWait(xSem_GNSSReceive_endHandle, osWaitForever); //On n'as pas recu le bon message on attend encore
					}
					else
					{
						*headerSend = (Header){
							.recipient = 0xFE,
									.sender = MODULE_SOURCE_ADDRESS,
									.type = PACKET_TYPE_POLL,
									.len_payload = (size_t)receptionGNSS->Message.UBXMessage->len
						};
						LORA_Send(headerSend, (uint8_t*)receptionGNSS->Message.UBXMessage->UBX_Brute->buffer);
						sprintf(message, "\r\t\t\n...UBXMessage --SEND-- LORA Polling...\r\n");
						HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
						ITM_Port32(31)=44;
					}
					break;
				}
				free(headerSend);
			}
		free(LORA_Receive_Message);
		ITM_Port32(31)=9999;
		osDelay(1);
	}
	/* USER CODE END ReceivedLORATask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
