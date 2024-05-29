/*
 * UBXParser.h
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#ifndef INC_UBXPARSER_H_
#define INC_UBXPARSER_H_

#include <stdio.h>

typedef struct {
	uint8_t msgclass;
	uint8_t msgId;
	uint8_t load[];
}UBXMessage_parsed ;



#endif /* INC_UBXPARSER_H_ */
