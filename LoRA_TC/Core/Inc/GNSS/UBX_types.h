/*
 * UBX_types.h
 *
 *  Created on: Jul 19, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSS_UBX_TYPES_H_
#define INC_GNSS_UBX_TYPES_H_

typedef struct {
	uint8_t gnssId[1];
	uint8_t svId[1];
	uint8_t sigId[1];
	uint8_t freqId[1];
	uint8_t prRes[2];
	uint8_t cno[1];
	uint8_t qualityInd[1];
	uint8_t corrSource[1];
	uint8_t ionoModel[1];
	//uint8_t sigFlags[2];
	uint8_t health[1];
	uint8_t prSmoothed[1];
	uint8_t prUsed[1];
	uint8_t crUsed[1];
	uint8_t doUsed[1];
	uint8_t prCorrUsed[1];
	uint8_t crCorrUsed[1];
	uint8_t doCorrUsed[1];
}repeatedGroup_UBX_NAV_SIG;

typedef struct {
	uint8_t iTOW[4];
	uint8_t version[1];
	uint8_t numSigs[1];
	uint8_t reserved0[2];
	repeatedGroup_UBX_NAV_SIG* sig;
	uint8_t reserved1[4];
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
