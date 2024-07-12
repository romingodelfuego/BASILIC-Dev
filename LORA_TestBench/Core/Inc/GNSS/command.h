/*
 * Command.h
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include <stdint.h>

/* 							DISCLAIMER
 *	All these commands are going to be write on the flash memory,
 *	so at every start commands are send to ZED-F9T.
 *	Commands are not made by the STM, because it takes too muche space,
 *	so use the u-blox application : u-center to make faster and easier commands.
 */

/*
 * GPS : ENABLE			| L1C/A : ENABLE - L1C & L2C & L5 : DISABLE
 * SBS: ENABLE			| L1C/A : DISABLE
 * GALILEO : ENABLE		| E1 & E5A : ENALE - E5b & E6 : DISABLE
 * BEIDOU : ENABLE		| B1 & B2A : ENALE - B1C & B2 & B3 : DISABLE
 * IMES : DISABLE		| L1 : DISABLE
 * QZSS : ENABLE		| L1C/A & L5 : ENABLE - L1C & L1S & L2C : DISABLE
 * GLONASS : ENABLE		| L1 : ENABLE - L10C & L2 & L3 : DISABLE
 * NAVIC : ENABLE		| L5 :DISABLE
 */
static uint8_t commandSetGNSS_Config[] __attribute__((used)) = {
		0xb5, 0x62, 0x06, 0x8a, 0x7c, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00,
		0x31, 0x10, 0x01, 0x03, 0x00, 0x31, 0x10, 0x00, 0x04, 0x00, 0x31, 0x10,
		0x00, 0x05, 0x00, 0x31, 0x10, 0x00, 0x07, 0x00, 0x31, 0x10, 0x01, 0x09,
		0x00, 0x31, 0x10, 0x01, 0x0a, 0x00, 0x31, 0x10, 0x00, 0x0d, 0x00, 0x31,
		0x10, 0x01, 0x0e, 0x00, 0x31, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x12, 0x00, 0x31, 0x10, 0x01, 0x15, 0x00, 0x31, 0x10, 0x00, 0x17, 0x00,
		0x31, 0x10, 0x01, 0x18, 0x00, 0x31, 0x10, 0x01, 0x1a, 0x00, 0x31, 0x10,
		0x00, 0x1d, 0x00, 0x31, 0x10, 0x00, 0x1f, 0x00, 0x31, 0x10, 0x01, 0x20,
		0x00, 0x31, 0x10, 0x01, 0x21, 0x00, 0x31, 0x10, 0x01, 0x22, 0x00, 0x31,
		0x10, 0x01, 0x24, 0x00, 0x31, 0x10, 0x01, 0x25, 0x00, 0x31, 0x10, 0x01,
		0x26, 0x00, 0x31, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0xec
};
/*
  RAM CFG-UART1OUTPROT-NMEA 0                    	# write value 0                    to item id 0x10740002 in layer 0
  RAM CFG-UART1OUTPROT-UBX 1                    	# write value 1                    to item id 0x10740001 in layer 0
  RAM CFG-UART1OUTPROT-RTCM3X 0                    	# write value 0
 */
static uint8_t commandUart1Ouput[] __attribute__((used))= {
		0xb5, 0x62, 0x06, 0x8a, 0x13, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x02, 0x00, 0x74, 0x10, 0x00, 0x01,
		0x00, 0x74, 0x10, 0x01, 0x04, 0x00, 0x74, 0x10,
		0x00, 0x38, 0x79
};
/*
 ENABLE POLLING UBX_NAV_TIMEUTC every RAM CFG-RATE-MEAS period
 */
static uint8_t commandUBXTimeUTC[] __attribute__((used))= {
		0xb5, 0x62, 0x06, 0x01, 0x03, 0x00, 0x01, 0x21, 0x01, 0x2d, 0x85
};
static uint8_t pollUBXTimeUTC[] __attribute__((used))= {
		0xb5, 0x62, 0x01, 0x21, 0x00, 0x00, 0x22, 0x67
};

/*
  RAM CFG-TP-PULSE_DEF     1                    # write value 1 - FREQ             to item id 0x20050023 in layer 0
  RAM CFG-TP-PULSE_LENGTH_DEF 0                    # write value 0 - RATIO            to item id 0x20050030 in layer 0
  RAM CFG-TP-ANT_CABLEDELAY 0x32                 # write value 50  0x32             to item id 0x30050001 in layer 0
  RAM CFG-TP-PERIOD_TP1    0x186a0              # write value 100000  0x186a0      to item id 0x40050002 in layer 0
  RAM CFG-TP-PERIOD_LOCK_TP1 0x186a0              # write value 100000  0x186a0      to item id 0x40050003 in layer 0
  RAM CFG-TP-FREQ_TP1      0xa                  # write value 10  0xa              to item id 0x40050024 in layer 0
  RAM CFG-TP-FREQ_LOCK_TP1 0xa                  # write value 10  0xa              to item id 0x40050025 in layer 0
  RAM CFG-TP-LEN_TP1       0x0                  # write value 0  0x0               to item id 0x40050004 in layer 0
  RAM CFG-TP-LEN_LOCK_TP1  0x0                  # write value 0  0x0               to item id 0x40050005 in layer 0
  RAM CFG-TP-DUTY_TP1      10.000000            # write value 10.000000            to item id 0x5005002a in layer 0
  RAM CFG-TP-DUTY_LOCK_TP1 50.000000            # write value 50.000000            to item id 0x5005002b in layer 0
  RAM CFG-TP-TP1_ENA       1                    # write value 1                    to item id 0x10050007 in layer 0
  RAM CFG-TP-TP2_ENA       1                    # write value 1                    to item id 0x10050012 in layer 0
 */
static uint8_t commandSetTP1_atNVTRate[] __attribute__((used))={
		0xb5, 0x62, 0x06, 0x8a, 0x55, 0x00, 0x00, 0x01, 0x00, 0x00, 0x23, 0x00,
		0x05, 0x20, 0x01, 0x30, 0x00, 0x05, 0x20, 0x00, 0x01, 0x00, 0x05, 0x30,
		0x32, 0x00, 0x06, 0x00, 0x05, 0x40, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00,
		0x05, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x40, 0x07, 0x00,
		0x05, 0x10, 0x01, 0x08, 0x00, 0x05, 0x10, 0x01, 0x09, 0x00, 0x05, 0x10,
		0x01, 0x0b, 0x00, 0x05, 0x10, 0x01, 0x0c, 0x00, 0x05, 0x20, 0x00, 0x24,
		0x00, 0x05, 0x40, 0x0a, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x05, 0x50, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x40, 0x67, 0x7f
};

 /*
  RAM CFG-TP-PULSE_DEF     1                    # write value 1 - FREQ             to item id 0x20050023 in layer 0
  RAM CFG-TP-PULSE_LENGTH_DEF 0                    # write value 0 - RATIO            to item id 0x20050030 in layer 0
  RAM CFG-TP-ANT_CABLEDELAY 0x32                 # write value 50  0x32             to item id 0x30050001 in layer 0
  RAM CFG-TP-FREQ_TP2      0xf4240              # write value 1000000  0xf4240     to item id 0x40050026 in layer 0
  RAM CFG-TP-FREQ_LOCK_TP2 0xf4240              # write value 1000000  0xf4240     to item id 0x40050027 in layer 0
  RAM CFG-TP-DUTY_TP2      50.000000            # write value 50.000000            to item id 0x5005002c in layer 0
  RAM CFG-TP-DUTY_LOCK_TP2 50.000000            # write value 50.000000            to item id 0x5005002d in layer 0
  RAM CFG-TP-USER_DELAY_TP2 0x0                  # write value 0  0x0               to item id 0x40050011 in layer 0
  RAM CFG-TP-TP2_ENA       1                    # write value 1                    to item id 0x10050012 in layer 0
  RAM CFG-TP-SYNC_GNSS_TP2 1                    # write value 1                    to item id 0x10050013 in layer 0
  RAM CFG-TP-ALIGN_TO_TOW_TP2 1                    # write value 1                    to item id 0x10050015 in layer 0
  RAM CFG-TP-POL_TP2       1                    # write value 1                    to item id 0x10050016 in layer 0
  RAM CFG-TP-TIMEGRID_TP2  0                    # write value 0 - UTC              to item id 0x20050017 in layer 0
 */
static uint8_t commandSetTP2[] __attribute__((used))= {
		0xb5, 0x62, 0x06, 0x8a, 0x5d, 0x00, 0x00, 0x01, 0x00, 0x00, 0x23, 0x00,
		0x05, 0x20, 0x01, 0x30, 0x00, 0x05, 0x20, 0x00, 0x01, 0x00, 0x05, 0x30,
		0x32, 0x00, 0x26, 0x00, 0x05, 0x40, 0x40, 0x42, 0x0f, 0x00, 0x27, 0x00,
		0x05, 0x40, 0x40, 0x42, 0x0f, 0x00, 0x2c, 0x00, 0x05, 0x50, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x49, 0x40, 0x2d, 0x00, 0x05, 0x50, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x49, 0x40, 0x11, 0x00, 0x05, 0x40, 0x00, 0x00,
		0x00, 0x00, 0x12, 0x00, 0x05, 0x10, 0x01, 0x13, 0x00, 0x05, 0x10, 0x01,
		0x15, 0x00, 0x05, 0x10, 0x01, 0x16, 0x00, 0x05, 0x10, 0x01, 0x17, 0x00,
		0x05, 0x20, 0x00, 0x3c, 0xb5
};

/*
  RAM CFG-RATE-MEAS        0x64               	# write value 100  0x64        	   to item id 0x30210001 in layer 0
  RAM CFG-RATE-NAV         0x1                  # write value 1  0x1               to item id 0x30210002 in layer 0
  RAM CFG-RATE-TIMEREF     0                    # write value 0 - UTC              to item id 0x20210003 in layer 0
 */
static uint8_t commandMeasureRate[] __attribute__((used))= {
		0xb5, 0x62, 0x06, 0x8a, 0x15, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x01, 0x00, 0x21, 0x30, 0x64, 0x00,
		0x02, 0x00, 0x21, 0x30, 0x01, 0x00, 0x03, 0x00,
		0x21, 0x20, 0x00, 0xf4, 0xa9
};

#endif /* INC_COMMAND_H_ */
