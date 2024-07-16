/*
 * shared.h
 *
 *  Created on: Jun 17, 2024
 *      Author: romain.pace
 */

#ifndef SRC_GNSS_SHARED_H_
#define SRC_GNSS_SHARED_H_

#include <stdint.h>
#include <stdio.h>

/******** DEBUG ********/
#define ITM_Port32(n) (*((volatile unsigned long *)(0xE0000000+4*n)))
/******** ----- ********/
#define UART_RX_BUFFER_SIZE 200
#define UART_MAX_BUFFER_SIZE 2048
#define UART_TX_BUFFER_SIZE 200
#define UART_DEBUG_BUFFER_SIZE 200

#define HEADER_UBX_1 0xB5
#define HEADER_UBX_2 0x62

#define HEADER_NMEA '$'
/******** GNSS ********/
typedef enum {
	INIT,
	NMEA,
	UBX,
} OutputProtocol;
typedef struct {
	uint8_t *buffer;
	size_t size;
} DynamicBuffer;

typedef struct {
	uint8_t CLASS;
	uint8_t ID;
	uint16_t len_payload;
	DynamicBuffer* brute; // Pointeur de tableau de longeur variable
}UBXMessage_parsed ;

typedef struct {
	char* NMEA_Brute;
}NMEAMessage_parsed;

typedef union {
	NMEAMessage_parsed* NMEAMessage;
	UBXMessage_parsed* UBXMessage;
}MessageStruct;

typedef struct{
	OutputProtocol typeMessage;
	MessageStruct Message;
}GenericMessage;
/******** ---- ********/

/******** QUEUE ********/
typedef struct {
	uint8_t data;  // Un octet de données reçu via UART
} UARTMessageQ_t;

typedef struct {
	UBXMessage_parsed* UBXMessage;
}UBXMessageQ_t;
/******** ---- ********/

/******** LORA ********/
typedef struct{
	uint8_t recipient;
	uint8_t sender;
	uint8_t type;
	uint8_t identifier;// Pas sure de l'utilité
	uint8_t nbOf_packet;// Le nombre total de paquet qui vont etre send

	uint8_t num_packet;// Le numero du paquet send
	uint8_t	len_payload;

}LORA_HeaderforSending;

typedef struct{
	uint8_t recipient;
	uint8_t sender;
	uint8_t type;
	uint8_t	len_payload;

}LORA_HeaderforReception;
typedef struct {
	uint8_t IRQFlags;
	uint8_t RxCurrAddr;
	uint8_t RxNbrBytes;
	uint8_t SNR;
	uint8_t RSSI;
	LORA_HeaderforSending* header;
	uint8_t* payload;
}LORA_MessageReception;
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
