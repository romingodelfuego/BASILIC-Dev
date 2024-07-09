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
#include "usb_otg.h"
#include "RTOS_subfunctions/matcher.h"
#include "RTOS_subfunctions/fakeuseSD.h"
#include "RTOS_subfunctions/uartbyteToGnssMessage.h"
#include "RTOS_subfunctions/receiverLoRA.h"
#include "RTOS_subfunctions/debug.h"
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
extern TIM_HandleTypeDef htim2;

/* USER CODE END Variables */
osThreadId InitTaskHandle;
osThreadId ReceiverLoRAHandle;
osThreadId UARTbyte_to_GNHandle;
osThreadId MatcherHandle;
osThreadId Fake_SDuseHandle;
osThreadId UartDebugHandle;
osThreadId commandToGNSSHandle;
osThreadId SenderLoRaHandle;
osMessageQId UARTbyteHandle;
osMessageQId UBXQueueHandle;
osMessageQId GNSS_RequestHandle;
osMessageQId GNSS_ReturnHandle;
osMessageQId UARTdebugHandle;
osMessageQId GNSS_toPollHandle;
osMessageQId LoRA_toSendHandle;
osSemaphoreId xSem_LORAReceive_startHandle;
osSemaphoreId SD_Access_GNSS_ReturnHandle;
osSemaphoreId LORA_Access_GNSS_ReturnHandle;
osSemaphoreId GNSS_UART_AccessHandle;
osSemaphoreId xSem_GNSS_InitHandle;
osSemaphoreId STARTUP_INIT_doneHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInitTask(void const * argument);
void ReceiverLoRA_Task(void const * argument);
void UARTbyte_to_GNSSMessage_Task(void const * argument);
void MatcherTask(void const * argument);
void Fake_SDuse_Task(void const * argument);
void UartDebugTask(void const * argument);
void commandToGNSSTask(void const * argument);
void SenderLoRa_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
}
//extern volatile unsigned long ulHighFrequencyTimerTicks;
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
  /* definition and creation of xSem_LORAReceive_start */
  osSemaphoreDef(xSem_LORAReceive_start);
  xSem_LORAReceive_startHandle = osSemaphoreCreate(osSemaphore(xSem_LORAReceive_start), 1);

  /* definition and creation of SD_Access_GNSS_Return */
  osSemaphoreDef(SD_Access_GNSS_Return);
  SD_Access_GNSS_ReturnHandle = osSemaphoreCreate(osSemaphore(SD_Access_GNSS_Return), 1);

  /* definition and creation of LORA_Access_GNSS_Return */
  osSemaphoreDef(LORA_Access_GNSS_Return);
  LORA_Access_GNSS_ReturnHandle = osSemaphoreCreate(osSemaphore(LORA_Access_GNSS_Return), 1);

  /* definition and creation of GNSS_UART_Access */
  osSemaphoreDef(GNSS_UART_Access);
  GNSS_UART_AccessHandle = osSemaphoreCreate(osSemaphore(GNSS_UART_Access), 1);

  /* definition and creation of xSem_GNSS_Init */
  osSemaphoreDef(xSem_GNSS_Init);
  xSem_GNSS_InitHandle = osSemaphoreCreate(osSemaphore(xSem_GNSS_Init), 1);

  /* definition and creation of STARTUP_INIT_done */
  osSemaphoreDef(STARTUP_INIT_done);
  STARTUP_INIT_doneHandle = osSemaphoreCreate(osSemaphore(STARTUP_INIT_done), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of UARTbyte */
  osMessageQDef(UARTbyte, 50, uint8_t);
  UARTbyteHandle = osMessageCreate(osMessageQ(UARTbyte), NULL);

  /* definition and creation of UBXQueue */
  osMessageQDef(UBXQueue, 16, UBXMessageQ_t);
  UBXQueueHandle = osMessageCreate(osMessageQ(UBXQueue), NULL);

  /* definition and creation of GNSS_Request */
  osMessageQDef(GNSS_Request, 16, GNSSRequestQ_t);
  GNSS_RequestHandle = osMessageCreate(osMessageQ(GNSS_Request), NULL);

  /* definition and creation of GNSS_Return */
  osMessageQDef(GNSS_Return, 16, GNSSReturnQ_t);
  GNSS_ReturnHandle = osMessageCreate(osMessageQ(GNSS_Return), NULL);

  /* definition and creation of UARTdebug */
  osMessageQDef(UARTdebug, 128, UARTdebugQ_t);
  UARTdebugHandle = osMessageCreate(osMessageQ(UARTdebug), NULL);

  /* definition and creation of GNSS_toPoll */
  osMessageQDef(GNSS_toPoll, 16, GNSStoPollQ_t);
  GNSS_toPollHandle = osMessageCreate(osMessageQ(GNSS_toPoll), NULL);

  /* definition and creation of LoRA_toSend */
  osMessageQDef(LoRA_toSend, 16, LoRAtoSendQ_t);
  LoRA_toSendHandle = osMessageCreate(osMessageQ(LoRA_toSend), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of InitTask */
  osThreadDef(InitTask, StartInitTask, osPriorityRealtime, 0, 1024);
  InitTaskHandle = osThreadCreate(osThread(InitTask), NULL);

  /* definition and creation of ReceiverLoRA */
  osThreadDef(ReceiverLoRA, ReceiverLoRA_Task, osPriorityNormal, 0, 2048);
  ReceiverLoRAHandle = osThreadCreate(osThread(ReceiverLoRA), NULL);

  /* definition and creation of UARTbyte_to_GN */
  osThreadDef(UARTbyte_to_GN, UARTbyte_to_GNSSMessage_Task, osPriorityHigh, 0, 2048);
  UARTbyte_to_GNHandle = osThreadCreate(osThread(UARTbyte_to_GN), NULL);

  /* definition and creation of Matcher */
  osThreadDef(Matcher, MatcherTask, osPriorityHigh, 0, 1024);
  MatcherHandle = osThreadCreate(osThread(Matcher), NULL);

  /* definition and creation of Fake_SDuse */
  osThreadDef(Fake_SDuse, Fake_SDuse_Task, osPriorityNormal, 0, 2048);
  Fake_SDuseHandle = osThreadCreate(osThread(Fake_SDuse), NULL);

  /* definition and creation of UartDebug */
  osThreadDef(UartDebug, UartDebugTask, osPriorityBelowNormal, 0, 2048);
  UartDebugHandle = osThreadCreate(osThread(UartDebug), NULL);

  /* definition and creation of commandToGNSS */
  osThreadDef(commandToGNSS, commandToGNSSTask, osPriorityHigh, 0, 512);
  commandToGNSSHandle = osThreadCreate(osThread(commandToGNSS), NULL);

  /* definition and creation of SenderLoRa */
  osThreadDef(SenderLoRa, SenderLoRa_Task, osPriorityBelowNormal, 0, 2048);
  SenderLoRaHandle = osThreadCreate(osThread(SenderLoRa), NULL);

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
	const char startMessage[] = "\r\nStarting...\r\n";
	const char initDoneMessage[] = "\r\nInit Done\r\n\n";

	HAL_UART_Transmit(&huart1, (uint8_t *)startMessage, sizeof(startMessage), 10);

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4,GPIO_PIN_SET);
	GNSSCom_Init(&huart3,&huart1);
	LORACom_Init(&hspi2, &huart1);

	RFM9x_Init();
	GNSSCom_SetUp_Init();

	HAL_UART_Transmit(&huart1, (uint8_t *)initDoneMessage, sizeof(initDoneMessage), 10);
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
	osSignalSet(ReceiverLoRAHandle, 0x01);
	osSignalSet(Fake_SDuseHandle, 0x01);
	osSignalSet(MatcherHandle, 0x01);

	//On prend les semaphores
	//osSemaphoreWait(SD_Access_GNSS_ReturnHandle, osWaitForever);
	//osSemaphoreWait(LORA_Access_GNSS_ReturnHandle,osWaitForever);

	osThreadTerminate(InitTaskHandle);
	//__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
  /* USER CODE END StartInitTask */
}

/* USER CODE BEGIN Header_ReceiverLoRA_Task */
/**
 * @brief Function implementing the ReceiverLoRA thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_ReceiverLoRA_Task */
void ReceiverLoRA_Task(void const * argument)
{
  /* USER CODE BEGIN ReceiverLoRA_Task */
	/* Infinite loop */
	osEvent event = osSignalWait(0x01, osWaitForever);
	if (event.status == osEventSignal){
		for(;;)
		{
			osEvent eventFromISR = osSignalWait(0x02, osWaitForever); //On attend de recevoir un ISR depuis un EXTI
			if (eventFromISR.status == osEventSignal) receivedLora();
		}
	}
  /* USER CODE END ReceiverLoRA_Task */
}

/* USER CODE BEGIN Header_UARTbyte_to_GNSSMessage_Task */
/**
 * @brief Function implementing the UARTbytet_to_GN thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_UARTbyte_to_GNSSMessage_Task */
void UARTbyte_to_GNSSMessage_Task(void const * argument)
{
  /* USER CODE BEGIN UARTbyte_to_GNSSMessage_Task */
	/* Infinite loop */
	for(;;)
	{
		uartbyteToGnssMessage();
		//Surtout pas delay ici
	}
  /* USER CODE END UARTbyte_to_GNSSMessage_Task */
}

/* USER CODE BEGIN Header_MatcherTask */
/**
 * @brief Function implementing the Matcher thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_MatcherTask */
void MatcherTask(void const * argument)
{
  /* USER CODE BEGIN MatcherTask */
	/* Infinite loop */
	osEvent event = osSignalWait(0x01, osWaitForever);
	if (event.status==osEventSignal){
		for(;;)
		{
			//On attend qu'un message UBX soit recu car nous avons prealablement envoyer une requete au GNSS}
			while(uxQueueMessagesWaiting(UBXQueueHandle)==0){vTaskDelay(1);}
			matcher();
		}
	}
  /* USER CODE END MatcherTask */
}

/* USER CODE BEGIN Header_Fake_SDuse_Task */
/**
 * @brief Function implementing the Fake_SDuse thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Fake_SDuse_Task */
void Fake_SDuse_Task(void const * argument)
{
  /* USER CODE BEGIN Fake_SDuse_Task */
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	osEvent event = osSignalWait(0x01, osWaitForever);
	if (event.status==osEventSignal){

		/* Infinite loop */
		for(;;)
		{
			fakeuseSD();
			vTaskDelayUntil(&xLastWakeTime,1000);
		}
	}
  /* USER CODE END Fake_SDuse_Task */
}

/* USER CODE BEGIN Header_UartDebugTask */
/**
 * @brief Function implementing the UartDebug thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_UartDebugTask */
void UartDebugTask(void const * argument)
{
  /* USER CODE BEGIN UartDebugTask */
	/* Infinite loop */
	for(;;)
	{
		debug();

	}
  /* USER CODE END UartDebugTask */
}

/* USER CODE BEGIN Header_commandToGNSSTask */
/**
 * @brief Function implementing the commandToGNSS thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_commandToGNSSTask */
void commandToGNSSTask(void const * argument)
{
  /* USER CODE BEGIN commandToGNSSTask */
	/* Infinite loop */
	for(;;)
	{
		commandToGNSS();
		//vTaskDelay(200);//Pas terrible
	}

  /* USER CODE END commandToGNSSTask */
}

/* USER CODE BEGIN Header_SenderLoRa_Task */
/**
 * @brief Function implementing the SenderLoRa thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_SenderLoRa_Task */
void SenderLoRa_Task(void const * argument)
{
  /* USER CODE BEGIN SenderLoRa_Task */
	/* Infinite loop */
	for(;;)
	{
		senderLoRA();
	}
  /* USER CODE END SenderLoRa_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/*void initPool(osPoolId* poolId, int POOL_SIZE) {
    osPoolDef_t poolDef = {POOL_SIZE, sizeof(UBXMessage_parsed), NULL};
 *poolId = osPoolCreate(&poolDef);
    if (*poolId == NULL) {
        // Gérer l'erreur d'initialisation du pool
    }
}*/

/* USER CODE END Application */
