/*
 * UBX_types.h
 *
 *  Created on: Jul 19, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSS_UBX_TYPES_H_
#define INC_GNSS_UBX_TYPES_H_
#include "UBX_assets.h"
typedef struct {
	U1 gnssId;
	U1 svId;
	U1 sigId;
	U1 freqId;
	I2 prRes;
	U1 cno;
	U1 qualityInd;
	U1 corrSource;
	U1 ionoModel;
	X2_sigFlags sigFlags;
	uint8_t reserved1[4];
}repeatedGroup_UBX_NAV_SIG;

typedef struct {
	U4 iTOW;
	U1 version;
	U1 numSigs;
	uint8_t reserved0[2];
	repeatedGroup_UBX_NAV_SIG* sig;

}UBX_NAV_SIG;

typedef struct {
	uint8_t iTOW[4];
	uint8_t tAcc[4];
	uint8_t nano[4];
	uint8_t year[2];
	uint8_t month[1];
	uint8_t day[1];
	uint8_t hour[1];
	uint8_t min[1];
	uint8_t sec[1];
	uint8_t valid[1];
}UBX_NAV_TIMEUTC;
//extern UBX_NAV_TIMEUTC UBX_NAV_TIMEUTC_instance;

typedef struct {
	uint8_t version[1];
	uint8_t layers[1];
	uint8_t reserved[2];
	uint8_t cfgData[UART_RX_BUFFER_SIZE];
}UBX_CFG_SETVAL;
//extern UBX_CFG_SETVAL UBX_CFG_SETVAL_instance;

typedef struct {
	uint8_t msgClass[1];
	uint8_t msgID[1];
	uint8_t rate[1];
}UBX_CFG_MSG;
//extern UBX_CFG_MSG UBX_CFG_MSG_instance;


#endif /* INC_GNSS_UBX_TYPES_H_ */
