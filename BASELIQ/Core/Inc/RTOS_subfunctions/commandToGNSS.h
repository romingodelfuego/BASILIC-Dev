/*
 * commandToGNSS.h
 *
 *  Created on: Jun 27, 2024
 *      Author: romain.pace
 */

#ifndef INC_RTOS_SUBFUNCTIONS_COMMANDTOGNSS_H_
#define INC_RTOS_SUBFUNCTIONS_COMMANDTOGNSS_H_

#include "RTOS_extern.h"

void commandToGNSS(void);
void request_commandToGNSS(GNSStoPollQ_t transmitToGnss);

#endif /* INC_RTOS_SUBFUNCTIONS_COMMANDTOGNSS_H_ */
