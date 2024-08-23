/*
 * UBX_types.h
 *
 *  Created on: Jul 19, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSS_UBX_MESSAGESTRUCTURE_H_
#define INC_GNSS_UBX_MESSAGESTRUCTURE_H_
#include <GNSS/UBX_types.h>
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
	U4 iTOW;
	U4 tAcc;
	I4 nano;
	U2 year;
	U1 month;
	U1 day;
	U1 hour;
	U1 min;
	U1 sec;
	X1_valid valid;
}UBX_NAV_TIMEUTC;

typedef struct {
	uint8_t version[1];
	uint8_t layers[1];
	uint8_t reserved[2];
	uint8_t cfgData[200];
}UBX_CFG_SETVAL;

typedef struct {
	uint8_t msgClass[1];
	uint8_t msgID[1];
	uint8_t rate[1];
}UBX_CFG_MSG;


#endif /* INC_GNSS_UBX_MESSAGESTRUCTURE_H_ */
