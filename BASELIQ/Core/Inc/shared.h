/*
 * shared.h
 *
 *  Created on: Jun 17, 2024
 *      Author: romain.pace
 */

#ifndef SRC_GNSS_SHARED_H_
#define SRC_GNSS_SHARED_H_

#include <stdint.h>

typedef struct{
	uint8_t recipient;
	uint8_t sender;
	uint8_t type;
	uint8_t len_payload;
}Header ;

typedef struct {
	uint8_t IRQFlags;
	uint8_t RxCurrAddr;
	uint8_t RxNbrBytes;
	uint8_t SNR;
	uint8_t RSSI;
	Header* header;
	uint8_t* payload;
}LORA_Receive;


#endif /* SRC_GNSS_SHARED_H_ */
