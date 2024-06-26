/*
 * matcher.h
 *
 *  Created on: Jun 26, 2024
 *      Author: romain.pace
 */

#ifndef INC_RTOS_SUBFUNCTIONS_MATCHER_H_
#define INC_RTOS_SUBFUNCTIONS_MATCHER_H_

#include "RTOS_extern.h"

void matcher(void);
UBXMessage_parsed* findAndRemoveItemFromQueue(osMessageQId xQueue, GNSSRequestQ_t gnssRequest);
UBXMessage_parsed* findItemQueueWithoutCopy(osMessageQId xQueue, GNSSRequestQ_t gnssRequest);
int isItemMatching(UBXMessageQ_t* item,  GNSSRequestQ_t itemTarget);
#endif /* INC_RTOS_SUBFUNCTIONS_MATCHER_H_ */
