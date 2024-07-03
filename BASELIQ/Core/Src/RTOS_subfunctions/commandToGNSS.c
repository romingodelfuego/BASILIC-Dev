/*
 * commandToGNSS.c
 *
 *  Created on: Jun 27, 2024
 *      Author: romain.pace
 */

#include "RTOS_subfunctions/commandToGNSS.h"

/************************ TASK ************************/
void commandToGNSS(void){
	GNSStoPollQ_t transmitToGnss;

	xQueueReceive(GNSS_toPollHandle, &transmitToGnss, osWaitForever);
	osSemaphoreWait(GNSS_UART_AccessHandle, osWaitForever);	//Attendre avant de pouvoir utiliser le TX

	while(hGNSSCom.huart->gState != HAL_UART_STATE_READY) vTaskDelay(1);

	HAL_StatusTypeDef statut = HAL_UART_Transmit(hGNSSCom.huart, transmitToGnss.command, transmitToGnss.size,HAL_MAX_DELAY);
	ITM_Port32(28)=333;
	if (statut!= HAL_OK) Error_Handler();

	osSemaphoreRelease(GNSS_UART_AccessHandle);
}
/************************ ---- ************************/
/************************ FUNCTIONS ************************/
void request_commandToGNSS(GNSStoPollQ_t transmitToGnss){
	ITM_Port32(28)=222;
	xQueueSendToBack(GNSS_toPollHandle,&transmitToGnss,osWaitForever);
}
/************************ -------- ************************/


