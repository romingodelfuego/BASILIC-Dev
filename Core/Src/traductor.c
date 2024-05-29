/*
 * traductor.c
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#include "traductor.h"

UBX_NAV_TIMEUTC get_UBX_NAV_TIMEUTC(UBX_NAV_TIMEUTC load){
	UBX_NAV_TIMEUTC trad;
	trad.day=load[0];
	/* ... */
	return trad;
};
