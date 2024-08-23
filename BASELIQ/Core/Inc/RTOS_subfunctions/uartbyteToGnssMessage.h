/*
 * uartbyteToGnssMessage.h
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#ifndef INC_RTOS_SUBFUNCTIONS_UARTBYTETOGNSSMESSAGE_H_
#define INC_RTOS_SUBFUNCTIONS_UARTBYTETOGNSSMESSAGE_H_


#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "RTOS_extern.h"

#define HEADER_UBX_1 0xB5
#define HEADER_UBX_2 0x62

void uartbyteToGnssMessage(void);

#endif /* INC_RTOS_SUBFUNCTIONS_UARTBYTETOGNSSMESSAGE_H_ */
