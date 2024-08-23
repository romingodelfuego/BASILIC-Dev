/*
 * receveivedLora.h
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#ifndef INC_RTOS_SUBFUNCTIONS_RECEVEIVEDLORA_H_
#define INC_RTOS_SUBFUNCTIONS_RECEVEIVEDLORA_H_

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "RTOS_extern.h"
#include "commandToGNSS.h"


void receivedLora(void);
void PACKET_TYPE_POLL_fct(LORA_MessageReception* LORA_Receive_Message);
void PACKET_TYPE_ACK_fct(void);


#endif /* INC_RTOS_SUBFUNCTIONS_RECEVEIVEDLORA_H_ */
