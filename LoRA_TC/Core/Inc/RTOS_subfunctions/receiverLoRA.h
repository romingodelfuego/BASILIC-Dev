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
#include "LORA/LORACom.h"
#include "LORA/RFM9x.h"
#include "GNSS/traductor.h"

void receivedLora(void);
void messageLoRATreatment(LORA_MessageReception*);
uint8_t* concat_payloads(LoRAinReceptionQ_t* structsToConcatenate, uint8_t nbOfstructsToConcatenate, size_t* total_length);
void processQueueAndStoreIdentifiers(osMessageQId xQueue, uint8_t identifierToFind ,LoRAinReceptionQ_t* correspondingIdentifiers);
int isIdentifierMatching(LoRAinReceptionQ_t* item, uint8_t identifier);
#endif /* INC_RTOS_SUBFUNCTIONS_RECEVEIVEDLORA_H_ */
