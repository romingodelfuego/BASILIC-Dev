/*
 * getFormatter.c
 *
 *  Created on: Jul 23, 2024
 *      Author: romain.pace
 */
#include <GNSS/debug_getter.h>

char* get_GNSSID(U1* gnssID) {
	char* str = (char*)pvPortMalloc(21 * sizeof(char));
	if (str == NULL) Error_Handler();
	switch (gnssID->bytes[0]) {
	case 0: {strcpy(str,"GPS");}break;
	case 1: {strcpy(str,"SBAS");}break;
	case 2: {strcpy(str,"GAL");}break;
	case 3: {strcpy(str,"BDS");}break;
	case 4: {strcpy(str,"IMES");}break;
	case 5: {strcpy(str,"QZSS");}break;
	case 6: {strcpy(str,"GLO");}break;
	case 7: {strcpy(str,"NavIC");}break;
	default:{strcpy(str,"Unknown");}break;
	}
	return str;
}
char* get_SVID(U1* svID) {
	char* str = (char*)pvPortMalloc(8* sizeof(char));
	if (str == NULL) Error_Handler();

	if (1 < svID->bytes[0] && svID->bytes[0] < 32) {
		sprintf(str, "G%i", svID->bytes[0]);
	} else if (120 < svID->bytes[0] && svID->bytes[0] < 158) {
		sprintf(str, "S%i", svID->bytes[0]);
	} else if (211 < svID->bytes[0] && svID->bytes[0] < 246) {
		sprintf(str, "E%i", svID->bytes[0] - 210);
	} else if (159 < svID->bytes[0] && svID->bytes[0] < 163) {
		sprintf(str, "B%i", svID->bytes[0] - 158);
	} else if (33 < svID->bytes[0] && svID->bytes[0] < 64) {
		sprintf(str, "B%i", svID->bytes[0] - 27);
	} else if (173 < svID->bytes[0] && svID->bytes[0] < 182) {
		sprintf(str, "I%i", svID->bytes[0] - 172);
	} else if (193 < svID->bytes[0] && svID->bytes[0] < 202) {
		sprintf(str, "Q%i", svID->bytes[0] - 192);
	} else if (65 < svID->bytes[0] && svID->bytes[0] < 96) {
		sprintf(str, "R%i", svID->bytes[0] - 64);
	} else if (svID->bytes[0] == 255) {
		sprintf(str, "R?");
	} else if (247 < svID->bytes[0] && svID->bytes[0] < 253) {
		sprintf(str, "R%i", svID->bytes[0] - 246);
	} else {
		// Cas par défaut si aucune des conditions n'est remplie
		sprintf(str, "Unknown");
	}
	return str;
}
char* get_PRES(I2* prRes){
	char* str = (char*)pvPortMalloc(21 * sizeof(char));
	if (str == NULL) Error_Handler();
	sprintf(str,"%.1f",prRes->bytes[0]*0.1);
	return str;
}
char* get_QUALTIYIND(U1* qualityInd){
	char* str = (char*)pvPortMalloc(50 * sizeof(char));
	if (str == NULL) Error_Handler();
	switch (qualityInd->bytes[0]){
	case 0: {sprintf(str,"%sNO SIGNAL%s",ANSI_COLOR_RED,ANSI_COLOR_RESET);}break;
	case 1: {sprintf(str,"%sSEARCHING SIGNAL%s",ANSI_COLOR_MAGENTA,ANSI_COLOR_RESET);}break;
	case 2: {sprintf(str,"%sSIGNAL ACQUIRED%s",ANSI_COLOR_ORANGE,ANSI_COLOR_RESET);}break;
	case 3: {sprintf(str,"%sSIGNAL UNSUABLE%s",ANSI_COLOR_YELLOW,ANSI_COLOR_RESET);}break;
	case 4: {sprintf(str,"%sCODE LOCKED-TIME SYNC%s",ANSI_COLOR_GREEN,ANSI_COLOR_RESET);}break;
	case 5: {sprintf(str,"%sCODExCARRIER LOCKED-TIME SYNC%s",ANSI_COLOR_GREEN,ANSI_COLOR_RESET);}break;
	case 6: {sprintf(str,"%sCODExCARRIER LOCKED-TIME SYNC%s",ANSI_COLOR_GREEN,ANSI_COLOR_RESET);}break;
	case 7: {sprintf(str,"%sCODExCARRIER LOCKED-TIME SYNC%s",ANSI_COLOR_GREEN,ANSI_COLOR_RESET);}break;
	default:{strcpy(str,"Unknown");}break;
	}
	return str;
}
char* get_CORRSOURCE(U1* corrSource){
	char* str = (char*)pvPortMalloc(30 * sizeof(char));
	if (str == NULL) Error_Handler();
	switch (corrSource->bytes[0]){
	case 0: {strcpy(str,"NO CORRECTON");}break;
	case 1: {strcpy(str,"SBAS corrections");}break;
	case 2: {strcpy(str,"BeiDou corrections");}break;
	case 3: {strcpy(str,"RTCM2 corrections");}break;
	case 4: {strcpy(str,"RTCM3 OSR corrections");}break;
	case 5: {strcpy(str,"RTCM3 SSR corrections");}break;
	case 6: {strcpy(str,"QZSS SLAS corrections");}break;
	case 7: {strcpy(str,"SPARTN corrections");}break;
	case 8: {strcpy(str,"CLAS corrections");}break;
	default:{strcpy(str,"Unknown");}break;
	}
	return str;
}
char* get_IONOMODEL(U1* ionoModel){
	char* str = (char*)pvPortMalloc(52 * sizeof(char));
	if (str == NULL) Error_Handler();
	switch (ionoModel->bytes[0]){
	case 0: {strcpy(str,"NO MODEL");}break;
	case 1: {strcpy(str,"Klobuchar model(by GPS)");}break;
	case 2: {strcpy(str,"SBAS model");}break;
	case 3: {strcpy(str,"Klobuchar model(by BeiDou)");}break;
	case 8: {strcpy(str,"Iono delay(from derived dual frequency observation)");}break;
	default:{strcpy(str,"Unknown");}break;
	}
	return str;
}
char* get_HEALTH(uint8_t* health){
	char* str = (char*)pvPortMalloc(30 * sizeof(char));
	if (str == NULL) Error_Handler();
	switch (*health){
	case 0: {strcpy(str,"UNKNOMN");}break;
	case 1: {sprintf(str,"%sHEALTHY%s",ANSI_COLOR_GREEN,ANSI_COLOR_RESET);}break;
	case 2: {sprintf(str,"%sUNHEALTHY%s",ANSI_COLOR_RED,ANSI_COLOR_RESET);}break;
	default:{strcpy(str,"Unknown");}break;
	}
	return str;
}




