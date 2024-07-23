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
void debug_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate);
void create_UBX_NAV_SIG(UBXMessage_parsed* UBXMessage, UBX_NAV_SIG *structAssociate);
void cleaner_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate);
/*------------------------ ____________ ------------------------*/

#endif /* INC_GNSS_UBX_NAV_H_ */
