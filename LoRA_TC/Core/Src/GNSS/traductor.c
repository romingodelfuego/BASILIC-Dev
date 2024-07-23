/*
 * UBXParser.c
 *
 *  Created on: May 29, 2024
 *      Author: romain.pace
 */
#include "GNSS/UBX_NAV.h"
#include <GNSS/debug_formatter.h>
#include <GNSS/traductor.h>
#include "GNSS/GNSSCom.h"
#include "RTOS_subfunctions/debug.h"

// Define the instances for the message structures
UBX_NAV_TIMEUTC UBX_NAV_TIMEUTC_instance;
UBX_CFG_SETVAL UBX_CFG_SETVAL_instance;
UBX_CFG_MSG UBX_CFG_MSG_instance;
UBX_NAV_SIG UBX_NAV_SIG_instance;

// Define the message mappings array
MessageMapping message_mappings[] = {
		//{0x01, 0x21, (void (*)(UBXMessage_parsed *, void *)) debug_UBX_NAV_TIMEUTC, &UBX_NAV_TIMEUTC_instance}, //A la place de la fonction debug on peut penser a ecrire sur la SD
		//{0x06, 0x8a,(void (*)(UBXMessage_parsed *, void *)) debug_SetVal, &UBX_CFG_SETVAL_instance},
		//{0x06, 0x01,(void (*)(UBXMessage_parsed *, void *)) debug_PollMessage, &UBX_CFG_MSG_instance},
		{		0x01,0x43,
				&UBX_NAV_SIG_instance,
				(void (*)(UBXMessage_parsed *, void *))create_UBX_NAV_SIG,
				(void(*)(void*)) debug_UBX_NAV_SIG,
				(void(*)(void*)) cleaner_UBX_NAV_SIG
		},
		// Add other mappings for other message types if necessary
};


void traductor(UBXMessage_parsed* UBXMessage) {
	// Function pointer and variables to hold the matched mapping's values
	void (*create_func)(UBXMessage_parsed*, void *) = NULL;
	void (*debug_func)(void *) = NULL;
	void (*cleaner_func)(void *) = NULL;
	void* structAssociate = NULL;

	// Loop through the message mappings to find a match
	for (int i = 0; i < sizeof(message_mappings) / sizeof(message_mappings[0]); i++) {
		if (message_mappings[i].CLASS == UBXMessage->CLASS &&
				message_mappings[i].ID == UBXMessage->ID) {
			// Retrieve the corresponding get function and associated structure and variables
			create_func = message_mappings[i].create_func;
			debug_func = message_mappings[i].debug_func;
			cleaner_func = message_mappings[i].cleaner_func;
			structAssociate = message_mappings[i].structAssociate; //Pour l'utilisation dans d'autres fonctions, perhaps
			create_func(UBXMessage,structAssociate); //pvPortMaloc de structAssociate->sig apr exe
			debug_func(structAssociate);
			cleaner_func(structAssociate);
			break;
		}
	}
}
