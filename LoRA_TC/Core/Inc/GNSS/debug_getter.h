/*
 * getFormatter.h
 *
 *  Created on: Jul 23, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSS_DEBUG_GETTER_H_
#define INC_GNSS_DEBUG_GETTER_H_

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>  // Pour size_t
#include <stdlib.h>
#include <string.h> // Pour memcpy
#include <stdarg.h>

#include <GNSS/UBX_messageStructure.h>
#include "RTOS_subfunctions/RTOS_extern.h"
char* get_GNSSID(U1* gnssID);
char* get_SVID(U1* svID);
char* get_PRES(I2* prRes);
char* get_QUALTIYIND(U1* qualityInd);
char* get_CORRSOURCE(U1* corrSource);
char* get_IONOMODEL(U1* ionoModel);
char* get_HEALTH(uint8_t* health);


#endif /* INC_GNSS_DEBUG_GETTER_H_ */
