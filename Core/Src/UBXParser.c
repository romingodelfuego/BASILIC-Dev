/*
 * UBXParser.c
 *
 *  Created on: May 29, 2024
 *      Author: romain.pace
 */
#include "UBXParser.h"
#include "traductor.h"


UBX_NAV_TIMEUTC UBX_NAV_TIMEUTC_instance;
UBX_CFG_SETVAL UBX_CFG_SETVAL_instance;

MessageMapping message_mappings[] = {
		{0x01, 0x21, get_UBX_NAV_TIMEUTC,  &UBX_NAV_TIMEUTC_instance},
		{0x06,0x8a, get_SetVal, &UBX_CFG_SETVAL_instance},
		// Ajoutez d'autres mappings pour d'autres types de messages si nécessaire
};

void* create_message_struct(UBXMessage_parsed UBXMessage){
	void *(*get_func)(UBXMessage_parsed,void*) = NULL;
	void *structAssociate = NULL; // Déclaration de structAssociate

	for (int i = 0; i < sizeof(message_mappings)/ sizeof(message_mappings[0]); i++){
		if (message_mappings[i].msgClass == UBXMessage.msgClass &&
				message_mappings[i].msgID == UBXMessage.msgID){
					//On recupere la fonction get correspondante
					get_func = message_mappings[i].get_func;
					structAssociate = message_mappings[i].structAssociate;
					break;
		}
	}
	// Vérification de la fonction de création de la structure
	    if (get_func != NULL) {
	        // Appel de la fonction pour créer la structure
	        return get_func(UBXMessage,structAssociate);
	    } else{
	        return NULL;
	    }
}
