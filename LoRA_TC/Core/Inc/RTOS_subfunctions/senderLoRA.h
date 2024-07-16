/*
 * senderLoRA.h
 *
 *  Created on: Jul 8, 2024
 *      Author: romain.pace
 */

#ifndef INC_RTOS_SUBFUNCTIONS_SENDERLORA_H_
#define INC_RTOS_SUBFUNCTIONS_SENDERLORA_H_

#include "RTOS_extern.h"
#include "LORA/RFM9x.h"
//WHO AM I
#define MODULE_SOURCE_ADDRESS 0x01
#define MODULE_BROADCAST_ADDRESS 0xFF
// PAQUET TYPE
#define PACKET_TYPE_DATA 0x01
#define PACKET_TYPE_ACK  0x02
#define PACKET_TYPE_POLL 0x03

void senderLoRA(void);



#endif /* INC_RTOS_SUBFUNCTIONS_SENDERLORA_H_ */
