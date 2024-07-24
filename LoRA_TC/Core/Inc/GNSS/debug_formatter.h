/*
 * traductor.h
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#ifndef INC_TRADUCTOR_H_
#define INC_TRADUCTOR_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>  // Pour size_t
#include <stdlib.h>
#include <string.h> // Pour memcpy
#include <stdarg.h>
#include <stdbool.h>

#include "RTOS_subfunctions/RTOS_extern.h"
#include <GNSS/debug_getter.h>
#include <GNSS/debug_utils.h>
#include <GNSS/traductor.h>
#include <GNSS/UBX_messageStructure.h>

#define SEPARATON "\r\n--------------------------------------------------------------\r\n"

char* UBX_format(void* fieldToDebug, DataType type, char* (*funcSpe)(void*));
#endif /* INC_TRADUCTOR_H_ */
