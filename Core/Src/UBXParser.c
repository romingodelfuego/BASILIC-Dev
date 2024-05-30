/*
 * UBXParser.c
 *
 *  Created on: May 29, 2024
 *      Author: romain.pace
 */
#include "UBXParser.h"
#include "traductor.h"

// Define the instances for the message structures
UBX_NAV_TIMEUTC UBX_NAV_TIMEUTC_instance;
UBX_CFG_SETVAL UBX_CFG_SETVAL_instance;

// Define the message mappings array
MessageMapping message_mappings[] = {
    {0x01, 0x21, get_UBX_NAV_TIMEUTC, &UBX_NAV_TIMEUTC_instance},
	{0x06, 0x8a, get_SetVal, &UBX_CFG_SETVAL_instance},
    // Add other mappings for other message types if necessary
};

// Function to create a message structure
void create_message_debug(UBXMessage_parsed* UBXMessage) {
    // Function pointer and variables to hold the matched mapping's values
    void (*get_func)(UBXMessage_parsed*, void *) = NULL;
    void* structAssociate = NULL;

    // Loop through the message mappings to find a match
    for (int i = 0; i < sizeof(message_mappings) / sizeof(message_mappings[0]); i++) {
        if (message_mappings[i].msgClass == UBXMessage->msgClass &&
            message_mappings[i].msgID == UBXMessage->msgID) {
            // Retrieve the corresponding get function and associated structure and variables
            get_func = message_mappings[i].get_func;
            structAssociate = message_mappings[i].structAssociate;
            get_func(UBXMessage,structAssociate);
            break;
        }

    }
}

