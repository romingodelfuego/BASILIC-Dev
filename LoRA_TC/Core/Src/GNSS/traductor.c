/*
 * UBXParser.c
 *
 *  Created on: May 29, 2024
 *      Author: romain.pace
 */
#include <GNSS/debugFormatter.h>
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
				(void(*)(void*)) debug_UBX_NAV_SIG
		},
		// Add other mappings for other message types if necessary
};


void traductor(UBXMessage_parsed* UBXMessage) {
	// Function pointer and variables to hold the matched mapping's values
	void (*create_func)(UBXMessage_parsed*, void *) = NULL;
	void (*debug_func)(void *) = NULL;
	void* structAssociate = NULL;

	// Loop through the message mappings to find a match
	for (int i = 0; i < sizeof(message_mappings) / sizeof(message_mappings[0]); i++) {
		if (message_mappings[i].CLASS == UBXMessage->CLASS &&
				message_mappings[i].ID == UBXMessage->ID) {
			// Retrieve the corresponding get function and associated structure and variables
			create_func = message_mappings[i].create_func;
			debug_func = message_mappings[i].debug_func;
			structAssociate = message_mappings[i].structAssociate; //Pour l'utilisation dans d'autres fonctions, perhaps
			create_func(UBXMessage,structAssociate);
			debug_func(structAssociate);
			break;
		}
	}
}
void create_UBX_NAV_SIG(UBXMessage_parsed* UBXMessage, UBX_NAV_SIG *structAssociate){
	size_t offset = 6; //B562...
	updateMemoryUsage();
	memcpy(&(structAssociate->iTOW), UBXMessage->brute->buffer + offset, sizeof(structAssociate->iTOW));
	offset += sizeof(structAssociate->iTOW);

	memcpy(&(structAssociate->version), UBXMessage->brute->buffer + offset, sizeof(structAssociate->version));
	offset += sizeof(structAssociate->version);

	memcpy(&(structAssociate->numSigs), UBXMessage->brute->buffer + offset, sizeof(structAssociate->numSigs));
	offset += sizeof(structAssociate->numSigs);

	offset += sizeof(structAssociate->reserved0);
	structAssociate->sig = (repeatedGroup_UBX_NAV_SIG*)pvPortMalloc(structAssociate->numSigs.bytes[0] * sizeof(repeatedGroup_UBX_NAV_SIG));
	if (structAssociate->sig == NULL && structAssociate->numSigs.bytes[0]!=0 ) Error_Handler();
	updateMemoryUsage();
    for (int i = 0; i < structAssociate->numSigs.bytes[0]; i++) {

		memcpy(&(structAssociate->sig[i].gnssId.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].gnssId.bytes));
		offset += sizeof(structAssociate->sig[i].gnssId.bytes);

		memcpy(&(structAssociate->sig[i].svId), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].svId));
		offset += sizeof(structAssociate->sig[i].svId);

		memcpy(&(structAssociate->sig[i].sigId), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].sigId));
		offset += sizeof(structAssociate->sig[i].sigId);

		memcpy(&(structAssociate->sig[i].freqId.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].freqId.bytes));
		offset += sizeof(structAssociate->sig[i].freqId.bytes);

		memcpy(&(structAssociate->sig[i].prRes.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].prRes.bytes));
		offset += sizeof(structAssociate->sig[i].prRes.bytes);

		memcpy(&(structAssociate->sig[i].cno.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].cno.bytes));
		offset += sizeof(structAssociate->sig[i].cno.bytes);

		memcpy(&(structAssociate->sig[i].qualityInd.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].qualityInd.bytes));
		offset += sizeof(structAssociate->sig[i].qualityInd.bytes);

		memcpy(&(structAssociate->sig[i].corrSource.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].corrSource.bytes));
		offset += sizeof(structAssociate->sig[i].corrSource.bytes);

		memcpy(&(structAssociate->sig[i].ionoModel.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].ionoModel.bytes));
		offset += sizeof(structAssociate->sig[i].ionoModel.bytes);

		memcpy(&(structAssociate->sig[i].sigFlags.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].sigFlags.bytes));
		offset += sizeof(structAssociate->sig[i].sigFlags.bytes);

		offset += sizeof(structAssociate->sig[i].reserved1);

	}

	updateMemoryUsage();

}

