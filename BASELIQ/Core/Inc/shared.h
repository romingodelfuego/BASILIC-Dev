/*
 * shared.h
 *
 *  Created on: Jun 17, 2024
 *      Author: romain.pace
 */

#ifndef SRC_GNSS_SHARED_H_
#define SRC_GNSS_SHARED_H_

#include <stdint.h>
/******** DEBUG ********/
#define ITM_Port32(n) (*((volatile unsigned long *)(0xE0000000+4*n)))
/******** ----- ********/
#define UART_RX_BUFFER_SIZE 200
#define UART_TX_BUFFER_SIZE 200
#define UART_DEBUG_BUFFER_SIZE 200

#define HEADER_UBX_1 0xB5
#define HEADER_UBX_2 0x62

#define HEADER_NMEA '$'

/******** LORA ********/
typedef struct{
	uint8_t recipient;
	uint8_t sender;
	uint8_t type;
	uint8_t len_payload;
}Header;
typedef struct {
	uint8_t IRQFlags;
	uint8_t RxCurrAddr;
	uint8_t RxNbrBytes;
	uint8_t SNR;
	uint8_t RSSI;
	Header* header;
	uint8_t* payload;
}LORA_Message;
/******** ---- ********/

typedef struct {
	uint8_t msg_class;
	uint8_t msg_id;
}UBX_identifier;

typedef struct {
	UBX_identifier* TIMEUTC;
	UBX_identifier* GPSSTATUT;
}UBXtoSend;
typedef struct{
	UBXtoSend* debug;
	UBXtoSend* lora;
	UBXtoSend* sd;
}needsIt;


#endif /* SRC_GNSS_SHARED_H_ */
