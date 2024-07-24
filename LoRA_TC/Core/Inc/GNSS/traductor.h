/*
 * UBXParser.h
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */

#ifndef INC_UBXPARSER_H_
#define INC_UBXPARSER_H_

#include <GNSS/debug_formatter.h>
#include <GNSS/UBX_messageStructure.h>
#include <stdio.h>
#include <stdint.h>
#include "shared.h"

extern OutputProtocol protocol;

typedef struct {
    uint8_t CLASS;
    uint8_t ID;
    void* structAssociate;
    void (*create_func)(UBXMessage_parsed*, void*);
    void (*debug_func)(void*);
    void (*cleaner_func)(void*);

} MessageMapping;
extern MessageMapping message_mappings[];

void traductor(UBXMessage_parsed* UBXMessage , ModuleConfig_t ModuleConfig);

#endif /* INC_UBXPARSER_H_ */
