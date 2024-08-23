/*
 * structAssociateuctor.c
 *
 *  Created on: May 28, 2024
 *      Author: romain.pace
 */
#include <GNSS/debug_formatter.h>

char* UBX_format(void* fieldToDebug, DataType type, char* (*funcSpe)(void*),...){
	switch (type) {
	case TYPE_BITFIELD:
	{
		uint8_t* fieldCasted = (uint8_t*)fieldToDebug;
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted,1);
		else return funcSpe(&fieldCasted);
	}
	case TYPE_U1:
	{
		U1* fieldCasted = (U1*)fieldToDebug;
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_U2:
	{
		U2* fieldCasted = (U2*)fieldToDebug;
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_U4:
	{
		U4* fieldCasted = (U4*)fieldToDebug;
		if (funcSpe == NULL) return uint8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_I1:
	{
		I1* fieldCasted = (I1*)fieldToDebug;
		if (funcSpe == NULL) return int8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_I2:
	{
		I2* fieldCasted = (I2*)fieldToDebug;
		if (funcSpe == NULL) return int8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_I4:
	{
		I4* fieldCasted = (I4*)fieldToDebug;
		if (funcSpe == NULL) return int8ArrayToString(fieldCasted->bytes,sizeof(fieldCasted->bytes));
		else return funcSpe(fieldCasted);
	}
	break;
	case TYPE_X1:
		break;
	case TYPE_X2:
		break;
	case TYPE_X4:
		break;


	default:
		return("Unknown type");
		break;
	}
	return "Error";
}


