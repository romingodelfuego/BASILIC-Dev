/*
 * UBX_NAV_SIG.c
 *
 *  Created on: Jul 23, 2024
 *      Author: romain.pace
 */

#include "GNSS/UBX_NAV.h"

void debug_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate){
	size_t offsetDebug = 0;
	char bufferDebug[MAX_BUFFER_SIZE];

	const int MAX_COLUMNS = 18;
	static char *columnNames[18] = {"Signal","gnssId","svId","sigId","freqId","prRes","cno",
			"qualityInd","corrSource","ionoModel","health","prSmoothed","prUsed","crUsed",
			"doUsed","prCorrUsed","crCorrUsed","doCorrUsed"};
	static int sizeColumns[18]={6,6,4,5,5,5,
								 5,5,5,5,5,5,
								 5,5,5,5,5,5};
	logMemoryUsage("START - formattedString");
	formattedString(bufferDebug, &offsetDebug, 3, true,false,
			"\r\n___debug_UBX_NAV_SIG__\r\n"
			"iTOW [ms]: %s\r\n"
			"Version: %s\r\n"
			"NumSigs: %s\r\n",
			uint8ArrayToString(structAssociate->iTOW.bytes,sizeof(structAssociate->iTOW.bytes)),
			uint8ArrayToString(structAssociate->version.bytes,sizeof(structAssociate->version.bytes)),
			uint8ArrayToString(structAssociate->numSigs.bytes,sizeof(structAssociate->numSigs.bytes))
	);
	logMemoryUsage("END - formattedString");

	formattedString(bufferDebug, &offsetDebug,1, false,false, SEPARATON);
	for (int i = 0; i < MAX_COLUMNS; ++i) {
		formattedString(bufferDebug, &offsetDebug, 1, false,true,
				"| %-*s |",
				sizeColumns[i],
				columnNames[i]);
	}

	formattedString(bufferDebug, &offsetDebug, 1, false,false, SEPARATON);

	UARTdebugQ_t UARTdebug; char *color = ANSI_COLOR_RESET;
	UARTdebug.message = pvPortMalloc(offsetDebug+1);
	UARTdebug.color = pvPortMalloc(strlen(color) + 1);
	updateMemoryUsage();
	strncpy(UARTdebug.message, bufferDebug, offsetDebug+1);
	strcpy(UARTdebug.color, color);
	offsetDebug = 0 ;

	xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
	logMemoryUsage("START - formattedString");

	for (int i = 0; i < structAssociate->numSigs.bytes[0]; i++) {
		formattedString(bufferDebug, &offsetDebug, 1, false,true,
				"| %-*i |",sizeColumns[0],i+1);
		formattedString(bufferDebug, &offsetDebug, 2*(MAX_COLUMNS - 1),true,true,
				"| %-*s || %-*s || %-*s || %-*s || %-*s || %-*s || %-*s |"
				"| %-*s || %-*s || %-*s || %-*s || %-*s || %-*s || %-*s || %-*s |"
				"| %-*s || %-*s |\n",
				sizeColumns[1],
				UBX_format(&(structAssociate->sig[i].gnssId),TYPE_U1,(char*(*)(void*))get_GNSSID),
				sizeColumns[2],
				UBX_format(&(structAssociate->sig[i].svId),TYPE_U1,(char*(*)(void*))get_SVID),
				sizeColumns[3],
				UBX_format(&(structAssociate->sig[i].sigId),TYPE_U1,NULL),
				sizeColumns[4],
				UBX_format(&(structAssociate->sig[i].freqId),TYPE_U1,NULL),
				sizeColumns[5],
				UBX_format(&(structAssociate->sig[i].prRes),TYPE_I2,(char*(*)(void*))get_PRES),
				sizeColumns[6],
				UBX_format(&(structAssociate->sig[i].cno ),TYPE_U1,NULL),
				sizeColumns[7],
				UBX_format(&(structAssociate->sig[i].qualityInd),TYPE_U1,(char*(*)(void*))get_QUALTIYIND),
				sizeColumns[8],
				UBX_format(&(structAssociate->sig[i].corrSource),TYPE_U1,(char*(*)(void*))get_CORRSOURCE),
				sizeColumns[9],
				UBX_format(&(structAssociate->sig[i].ionoModel),TYPE_U1,(char*(*)(void*))get_IONOMODEL),

				sizeColumns[10],
				UBX_format(structAssociate->sig[i].sigFlags.health,TYPE_BITFIELD,(char*(*)(void*))get_HEALTH),
				sizeColumns[11],
				UBX_format(structAssociate->sig[i].sigFlags.prSmoothed,TYPE_BITFIELD,NULL),
				sizeColumns[12],
				UBX_format(structAssociate->sig[i].sigFlags.prUsed,TYPE_BITFIELD,NULL),
				sizeColumns[13],
				UBX_format(structAssociate->sig[i].sigFlags.crUsed,TYPE_BITFIELD,NULL),
				sizeColumns[14],
				UBX_format(structAssociate->sig[i].sigFlags.doUsed,TYPE_BITFIELD,NULL),
				sizeColumns[15],
				UBX_format(structAssociate->sig[i].sigFlags.prCorrUsed,TYPE_BITFIELD,NULL),
				sizeColumns[16],
				UBX_format(structAssociate->sig[i].sigFlags.crCorrUsed,TYPE_BITFIELD,NULL),
				sizeColumns[17],
				UBX_format(structAssociate->sig[i].sigFlags.doCorrUsed,TYPE_BITFIELD,NULL)
		);
		formattedString(bufferDebug, &offsetDebug, 1, false,false, SEPARATON);

		UARTdebugQ_t UARTdebug; char *color = ANSI_COLOR_RESET;

		UARTdebug.message = pvPortMalloc(offsetDebug+1);
		UARTdebug.color = pvPortMalloc(strlen(color) + 1);
		updateMemoryUsage();
		strncpy(UARTdebug.message, bufferDebug, offsetDebug+1);
		strcpy(UARTdebug.color, color);
		offsetDebug=0;

		xQueueSendToBack(UARTdebugHandle, &UARTdebug, osWaitForever);
		updateMemoryUsage();
	}
}

void cleaner_UBX_NAV_SIG(UBX_NAV_SIG *structAssociate) {
	vPortFree(structAssociate->sig);
	updateMemoryUsage();
}
void create_UBX_NAV_SIG(UBXMessage_parsed* UBXMessage, UBX_NAV_SIG *structAssociate){
	size_t offset = 6; //B562...
	logMemoryUsage("START - CREATION - UBX_NAV_SIG");
	memcpy(&(structAssociate->iTOW), UBXMessage->brute->buffer + offset, sizeof(structAssociate->iTOW));
	offset += sizeof(structAssociate->iTOW);

	memcpy(&(structAssociate->version), UBXMessage->brute->buffer + offset, sizeof(structAssociate->version));
	offset += sizeof(structAssociate->version);

	memcpy(&(structAssociate->numSigs), UBXMessage->brute->buffer + offset, sizeof(structAssociate->numSigs));
	offset += sizeof(structAssociate->numSigs);

	offset += sizeof(structAssociate->reserved0);
	structAssociate->sig = (repeatedGroup_UBX_NAV_SIG*)pvPortMalloc(structAssociate->numSigs.bytes[0] * sizeof(repeatedGroup_UBX_NAV_SIG));
	if (structAssociate->sig == NULL && structAssociate->numSigs.bytes[0]!=0 ) Error_Handler();
	//updateMemoryUsage();
    for (int i = 0; i < structAssociate->numSigs.bytes[0]; i++) {

		memcpy(&(structAssociate->sig[i].gnssId.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].gnssId.bytes));
		offset += sizeof(structAssociate->sig[i].gnssId.bytes);

		memcpy(&(structAssociate->sig[i].svId.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].svId.bytes));
		offset += sizeof(structAssociate->sig[i].svId.bytes);

		memcpy(&(structAssociate->sig[i].sigId.bytes), UBXMessage->brute->buffer + offset, sizeof(structAssociate->sig[i].sigId.bytes));
		offset += sizeof(structAssociate->sig[i].sigId.bytes);

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
    logMemoryUsage("END - CREATION - UBX_NAV_SIG");
}




