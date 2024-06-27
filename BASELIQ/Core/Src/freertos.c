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
#include "RTOS_subfunctions/matcher.h"
#include "RTOS_subfunctions/receveivedLora.h"
#include "RTOS_subfunctions/fakeuseSD.h"
#include "RTOS_subfunctions/uartbyteToGnssMessage.h"
#include "RTOS_subfunctions/debug.h"

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
osThreadId ReceivedLORAHandle;
osThreadId UARTbyte_to_GNHandle;
osThreadId MatcherHandle;
osThreadId Fake_SDuseHandle;
osThreadId UartDebugHandle;
osThreadId commandToGNSSHandle;
osMessageQId UARTbyteHandle;
osMessageQId UBXQueueHandle;
osMessageQId GNSS_RequestHandle;
osMessageQId GNSS_ReturnHandle;
osMessageQId UARTdebugHandle;
osMessageQId GNSS_toPollHandle;
osSemaphoreId xSem_LORAReceive_startHandle;
osSemaphoreId SD_Access_GNSS_ReturnHandle;
osSemaphoreId LORA_Access_GNSS_ReturnHandle;
osSemaphoreId GNSS_UART_AccessHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInitTask(void const * argument);
void ReceivedLORATask(void const * argument);
void UARTbyte_to_GNSSMessage_Task(void const * argument);
void MatcherTask(void const * argument);
void Fake_SDuse_Task(void const * argument);
void UartDebugTask(void const * argument);
void commandToGNSSTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
	HAL_TIM_Base_Start(&htim2);
}
extern volatile unsigned long ulHighFrequencyTimerTicks;
__weak unsigned long getRunTimeCounterValue(void)
{
	return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of UARTbyte */
  osMessageQDef(UARTbyte, 1, uint8_t);
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

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of InitTask */
  osThreadDef(InitTask, StartInitTask, osPriorityRealtime, 0, 1024);
  InitTaskHandle = osThreadCreate(osThread(InitTask), NULL);

  /* definition and creation of ReceivedLORA */
  osThreadDef(ReceivedLORA, ReceivedLORATask, osPriorityNormal, 0, 2048);
  ReceivedLORAHandle = osThreadCreate(osThread(ReceivedLORA), NULL);

  /* definition and creation of UARTbyte_to_GN */
  osThreadDef(UARTbyte_to_GN, UARTbyte_to_GNSSMessage_Task, osPriorityHigh, 0, 512);
  UARTbyte_to_GNHandle = osThreadCreate(osThread(UARTbyte_to_GN), NULL);

  /* definition and creation of Matcher */
  osThreadDef(Matcher, MatcherTask, osPriorityNormal, 0, 1024);
  MatcherHandle = osThreadCreate(osThread(Matcher), NULL);

  /* definition and creation of Fake_SDuse */
  osThreadDef(Fake_SDuse, Fake_SDuse_Task, osPriorityNormal, 0, 512);
  Fake_SDuseHandle = osThreadCreate(osThread(Fake_SDuse), NULL);

  /* definition and creation of UartDebug */
  osThreadDef(UartDebug, UartDebugTask, osPriorityNormal, 0, 512);
  UartDebugHandle = osThreadCreate(osThread(UartDebug), NULL);

  /* definition and creation of commandToGNSS */
  osThreadDef(commandToGNSS, commandToGNSSTask, osPriorityRealtime, 0, 512);
  commandToGNSSHandle = osThreadCreate(osThread(commandToGNSS), NULL);

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
	HAL_UART_Transmit(&huart1, (uint8_t *)initDoneMessage, sizeof(initDoneMessage), 10);

	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
	osThreadTerminate(InitTaskHandle);
	//  }
  /* USER CODE END StartInitTask */
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
		receivedLora();
	}
  /* USER CODE END ReceivedLORATask */
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


	/* Infinite loop */
	for(;;)
	{
		uartbyteToGnssMessage();
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
	for(;;)
	{
		matcher();
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


	/* Infinite loop */
	for(;;)
	{
		fakeuseSD();
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
  }
  /* USER CODE END commandToGNSSTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */




// Fonction pour copier une queue
/*void copyQueue(osMessageQId originalQueue, osMessageQId copyQueue) {
	uint32_t queueSize = uxQueueMessagesWaiting(originalQueue);

	osMessageQDef(UBXQueue, 16, UBXMessageQ_t);
	osMessageQId tempQueue = osMessageCreate(osMessageQ(UBXQueue), NULL);  // Créer une queue temporaire
	// Créer la copie de la queue
	UBXMessage_parsed item;
	if (tempQueue == NULL) {
		// Échec de la création des queues
		if (tempQueue != NULL) osMessageDelete(tempQueue);
		return;
	}
	// Copier les éléments de la queue d'origine vers la queue temporaire et la copie
	for (uint32_t index = 0; index < queueSize; index++) {
		xQueueReceive(originalQueue, &item, 0); // Recevoir un message
		xQueueSendToBack(tempQueue,&item, 0); // Insérer dans la queue temporaire
		xQueueSendToBack(copyQueue, &item, 0); // Insérer dans la copie de la queue
	}

	// Réinsérer les éléments dans la queue d'origine
	for (uint32_t index = 0; index < queueSize; index++) {
		xQueueReceive(tempQueue,&item, 0); // Recevoir un message de la queue temporaire
		xQueueSendToBack(originalQueue, &item, 0); // Réinsérer dans la queue d'origine
	}
	// Supprimer la queue temporaire
	osMessageDelete(tempQueue);

}*/
// Fonction pour convertir un tableau de uint8_t en une chaîne de caractères hexadécimale


/* USER CODE END Application */
