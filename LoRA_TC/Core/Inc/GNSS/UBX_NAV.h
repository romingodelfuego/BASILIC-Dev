/*
 * UBX_NAV.h
 *
 *  Created on: Jul 23, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSS_UBX_NAV_H_
#define INC_GNSS_UBX_NAV_H_

#include <stdbool.h>
#include "GNSS/traductor.h"
#include "GNSS/debug_utils.h"

/*------------------------ UBX_NAV_SIG ------------------------*/
static uint8_t poll_UBX_NAV_SIG [] __attribute__((used)) = {0xb5, 0x62, 0x01, 0x43, 0x00, 0x00, 0x44, 0xcd};

void debug_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate);
void create_UBX_NAV_SIG(UBXMessage_parsed* UBXMessage, UBX_NAV_SIG *structAssociate);
void cleaner_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate);
/*------------------------ ____________ ------------------------*/
/*------------------------ UBX_NAV_TIMEUTC ------------------------*/
static  uint8_t poll_UBX_NAV_TIMEUTC [] __attribute__((used)) = {0xb5, 0x62, 0x01, 0x21, 0x00, 0x00, 0x22, 0x67};
void debug_UBX_NAV_TIMEUTC(UBX_NAV_TIMEUTC *structAssociate);
void create_UBX_NAV_TIMEUTC(UBXMessage_parsed* UBXMessage, UBX_NAV_TIMEUTC *structAssociate);
void cleaner_UBX_NAV_TIMEUTC(UBX_NAV_TIMEUTC *structAssociate);
/*------------------------ ____________ ------------------------*/

#endif /* INC_GNSS_UBX_NAV_H_ */
