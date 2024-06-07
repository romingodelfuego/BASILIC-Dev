/*
 * debug.h
 *
 *  Created on: Jun 3, 2024
 *      Author: romain.pace
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_

#define ITM_Port32(n) (*((volatile unsigned long *)(0xE0000000+4*n)))

#endif /* INC_DEBUG_H_ */
