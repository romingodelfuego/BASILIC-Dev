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

typedef struct {
    uint8_t CLASS;
    uint8_t ID;
    osSemaphoreId applicantSemaphore;
    char* applicantName; // DEBUG PURPOSE: Assuming pointer to string
} GNSSRequestQ_t;

typedef struct {
    uint8_t CLASS;
    uint8_t ID;
    DynamicBuffer * bufferReturn;
    char* applicantName; // DEBUG PURPOSE: Assuming pointer to string
} GNSSReturnQ_t;

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
osThreadId UARTbytet_to_GNHandle;
osThreadId MatcherHandle;
osThreadId Fake_SDuseHandle;
osMessageQId UARTbyteHandle;
osMessageQId UBXQueueHandle;
osMessageQId GNSS_RequestHandle;
osMessageQId GNSS_ReturnHandle;
osSemaphoreId xSem_LORAReceive_startHandle;
osSemaphoreId SD_Access_GNSS_ReturnHandle;
osSemaphoreId LORA_Access_GNSS_ReturnHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartInitTask(void const * argument);
void ReceivedLORATask(void const * argument);
void UARTbyte_to_GNSSMessage_Task(void const * argument);
void MatcherTask(void const * argument);
void Fake_SDuse_Task(void const * argument);

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
	HAL_TIM_Base_Start_IT(&htim2);
}
extern volatile unsigned long ulHighFrequencyTimerTicks;
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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of UARTbyte */
  osMessageQDef(UARTbyte, 16, uint8_t);
  UARTbyteHandle = osMessageCreate(osMessageQ(UARTbyte), NULL);

  /* definition and creation of UBXQueue */
  osMessageQDef(UBXQueue, 16, UBXMessage_parsed);
  UBXQueueHandle = osMessageCreate(osMessageQ(UBXQueue), NULL);

  /* definition and creation of GNSS_Request */
  osMessageQDef(GNSS_Request, 16, uint16_t);
  GNSS_RequestHandle = osMessageCreate(osMessageQ(GNSS_Request), NULL);

  /* definition and creation of GNSS_Return */
  osMessageQDef(GNSS_Return, 16, uint16_t);
  GNSS_ReturnHandle = osMessageCreate(osMessageQ(GNSS_Return), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of InitTask */
  osThreadDef(InitTask, StartInitTask, osPriorityHigh, 0, 1024);
  InitTaskHandle = osThreadCreate(osThread(InitTask), NULL);

  /* definition and creation of ReceivedLORA */
  osThreadDef(ReceivedLORA, ReceivedLORATask, osPriorityAboveNormal, 0, 2048);
  ReceivedLORAHandle = osThreadCreate(osThread(ReceivedLORA), NULL);

  /* definition and creation of UARTbytet_to_GN */
  osThreadDef(UARTbytet_to_GN, UARTbyte_to_GNSSMessage_Task, osPriorityAboveNormal, 0, 512);
  UARTbytet_to_GNHandle = osThreadCreate(osThread(UARTbytet_to_GN), NULL);

  /* definition and creation of Matcher */
  osThreadDef(Matcher, MatcherTask, osPriorityAboveNormal, 0, 512);
  MatcherHandle = osThreadCreate(osThread(Matcher), NULL);

  /* definition and creation of Fake_SDuse */
  osThreadDef(Fake_SDuse, Fake_SDuse_Task, osPriorityAboveNormal, 0, 512);
  Fake_SDuseHandle = osThreadCreate(osThread(Fake_SDuse), NULL);

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
	GNSSMessageQ_t gnssMsg;

	/* Infinite loop */
	for(;;)
	{
		osSemaphoreWait(xSem_LORAReceive_startHandle, osWaitForever);
		ITM_Port32(31)=111;

		LORA_Message* LORA_Receive_Message = (LORA_Message*)malloc(sizeof(LORA_Message));
		RFM9x_Receive(LORA_Receive_Message);
		if (!LORA_Receive_Message->RxNbrBytes){ITM_Port32(31)=666;} //Si on recoit du bruit
		else
			if (LORA_Receive_Message->header->recipient == MODULE_BROADCAST_ADDRESS
					||LORA_Receive_Message->header->recipient == MODULE_SOURCE_ADDRESS){

				LORA_Header* headerSend =(LORA_Header*) malloc(sizeof(LORA_Header));
				switch (LORA_Receive_Message->header->type){

				case PACKET_TYPE_ACK:
					//Lora send un messsage vide
					*headerSend = (LORA_Header){
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

					ITM_Port32(31)=222;

					osSemaphoreWait(xSem_GNSSReceive_endHandle, osWaitForever);//on attend la fin d'une potentielle reception
					ITM_Port32(31)=333;
					//Give semaphore : Add to Queue

					osSemaphoreRelease(xSem_MessageGnssAddToQueue_RequestHandle);
					GNSSCom_Send_SetVal(poll); //On envoie un message vers GNSS
					ITM_Port32(31)=444;
					while (xQueueReceive(GNSSTransmissionQueueHandle, &gnssMsg, portMAX_DELAY) == pdTRUE){
						GenericMessage* receptionGNSS = gnssMsg.receptionGNSS;
						ITM_Port32(31)=555;


						if(	receptionGNSS->typeMessage==UBX &&
								receptionGNSS->Message.UBXMessage->class == LORA_Receive_Message->payload[2]&&
								receptionGNSS->Message.UBXMessage->ID ==LORA_Receive_Message->payload[3])
						{

							*headerSend = (LORA_Header){
								.recipient = 0xFE,
										.sender = MODULE_SOURCE_ADDRESS,
										.type = PACKET_TYPE_POLL,
										.len_payload = (size_t)receptionGNSS->Message.UBXMessage->len_payload
							};
							LORA_Send(headerSend, (uint8_t*)receptionGNSS->Message.UBXMessage->brute->buffer);
							sprintf(message, "\r\t\t\n...UBXMessage --SEND-- LORA Polling...\r\n");
							HAL_UART_Transmit(hGNSSCom.huartDebug, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
							ITM_Port32(31)=666;
							break;
						}
						if (uxQueueMessagesWaiting(GNSSTransmissionQueueHandle) == 0){break;}
					}
					break;
				}
				free(headerSend);
			}
		free(LORA_Receive_Message);
		osDelay(1);
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
	UARTMessageQ_t uartMsg;
	UARTState state;
	UBXMessage_parsed* messageUBX = (UBXMessage_parsed*) malloc(sizeof(UBXMessage_parsed)) ;
	uint16_t payloadIndex = 0;

	/* Infinite loop */
	for(;;)
	{
		if (xQueueReceive(UARTbyteHandle, &uartMsg, portMAX_DELAY) == pdTRUE) {
			uint8_t receivedByte = uartMsg.data;
			GenericMessage* receptionGNSS = (GenericMessage*)malloc(sizeof(GenericMessage));
			// Machine à états pour traiter les messages
			switch (state) {
			case WAIT_FOR_SYNC_1:
				if (receivedByte == 0xB5) {
					ITM_Port32(31)=12345;
					state = WAIT_FOR_SYNC_2;
				}
				break;
			case WAIT_FOR_SYNC_2:
				if (receivedByte == 0x62) {
					state = WAIT_FOR_CLASS;
				} else {
					state = WAIT_FOR_SYNC_1;
				}
				break;

			case WAIT_FOR_CLASS:
				messageUBX->class = receivedByte;
				state = WAIT_FOR_ID;
				break;
			case WAIT_FOR_ID:
				messageUBX->ID = receivedByte;
				state = WAIT_FOR_LENGTH_1;
				break;

			case WAIT_FOR_LENGTH_1:
				messageUBX->len_payload = receivedByte;
				state = WAIT_FOR_LENGTH_2;
				break;
			case WAIT_FOR_LENGTH_2:
				messageUBX->len_payload |= receivedByte << 8;
				if (messageUBX->len_payload > 0 && messageUBX->len_payload <= UART_MAX_BUFFER_SIZE) {
					messageUBX->load = initializeBuffer(messageUBX->len_payload);
					messageUBX->brute = initializeBuffer(messageUBX->len_payload+8);
					if (messageUBX->load == NULL) {
						// Erreur d'allocation de mémoire
						state = WAIT_FOR_SYNC_1;
					} else {
						payloadIndex = 0;
						messageUBX->brute->buffer[0]=0xb5;
						messageUBX->brute->buffer[1]=0x62;
						messageUBX->brute->buffer[2]=messageUBX->class;
						messageUBX->brute->buffer[3]=messageUBX->ID;
						messageUBX->brute->buffer[4]=messageUBX->len_payload & 0x00FF;
						messageUBX->brute->buffer[5]=messageUBX->len_payload >> 8;
						state = RECEIVE_MESSAGE;
					}
				} else {
					// Longueur invalide
					state = WAIT_FOR_SYNC_1;
				}
				break;
			case RECEIVE_MESSAGE:
				payloadIndex++ ;
				if (payloadIndex < messageUBX->len_payload){
					messageUBX->load->buffer[payloadIndex] = receivedByte;
				}
				if (payloadIndex < messageUBX->len_payload + 2){
					messageUBX->brute->buffer[6 + payloadIndex] = receivedByte;
				}
				else { // ON perd un byte ici !
					receptionGNSS->typeMessage=UBX;
					receptionGNSS->Message.UBXMessage = messageUBX;

					GNSSMessageQ_t gnssMsg = { .receptionGNSS = receptionGNSS };
					if (xQueueSendToBack(UBXQueueHandle, &gnssMsg, portMAX_DELAY) != pdTRUE) {
						// Erreur d'envoi dans la queue
						freeBuffer(messageUBX->load);
						freeBuffer(messageUBX->brute);
						free(messageUBX);
						free(receptionGNSS);
					}
					state = WAIT_FOR_SYNC_1;
				}
				break;
			default:
				state = WAIT_FOR_SYNC_1;
				break;
			}
		}
		GNSSCom_UartActivate(&hGNSSCom);
		osDelay(1);
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
	GNSSRequestQ_t gnssRequest;
	/* Infinite loop */
	for(;;)
	{
		xQueueReceive(GNSS_RequestHandle, &gnssRequest, osWaitForever);
		//Parcourir UBXQueue
		//Matcher avec Class et ID --> sortir le payload
		UBXMessage_parsed ubxFromQueueMatching;
		int itsMatching = 1;
		///////
		if (itsMatching){
			osSemaphoreRelease(gnssRequest.applicantSemaphore);
			GNSSReturnQ_t gnssReturn = {
					.CLASS = gnssRequest.CLASS,
					.ID = gnssRequest.ID,
					.bufferReturn = ubxFromQueueMatching.brute,
					.applicantName = gnssRequest.applicantName
			};
			xQueueSendToBack(GNSS_ReturnHandle,&gnssReturn,portMAX_DELAY);
		}
		osDelay(1);
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
	GNSSReturnQ_t gnssReturn;

  /* Infinite loop */
  for(;;)
  {
	osSemaphoreWait(SD_Access_GNSS_ReturnHandle, osWaitForever);
	xQueueReceive(GNSS_ReturnHandle, &gnssReturn, portMAX_DELAY);
    osDelay(1);
  }
  /* USER CODE END Fake_SDuse_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
