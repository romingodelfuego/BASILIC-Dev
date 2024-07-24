/*
 * UBX_assets.h
 *
 *  Created on: Jul 22, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSS_UBX_TYPES_H_
#define INC_GNSS_UBX_TYPES_H_
#include <stdio.h>
#include <stdint.h>

typedef enum {
	TYPE_BITFIELD,
	TYPE_U1,
	TYPE_I1,
	TYPE_X1,
	TYPE_U2,
	TYPE_I2,
	TYPE_X2,
	TYPE_U4,
	TYPE_I4,
	TYPE_X4,
	// Ajoutez d'autres types si nécessaire
} DataType;

typedef struct{
	uint8_t bytes[1];
}U1;

typedef struct{
	int8_t bytes[1];
}I1;

typedef union {
	// De cette facon il est possible de modifier byte en modifiant bits
	uint8_t bytes[1];
	struct {
		uint8_t bit1 : 1;
		uint8_t bit2 : 1;
		uint8_t bit3 : 1;
		uint8_t bit4 : 1;
		uint8_t bit5 : 1;
		uint8_t bit6 : 1;
		uint8_t bit7 : 1;
		uint8_t bit8 : 1;
	};
}X1;

typedef struct{
	uint8_t bytes[2];
}U2;

typedef struct{
	int8_t bytes[2];
}I2;

typedef union {
	// De cette facon il est possible de modifier byte en modifiant bits
	uint8_t bytes[2];
	struct {
		uint8_t bit1 : 1;
		uint8_t bit2 : 1;
		uint8_t bit3 : 1;
		uint8_t bit4 : 1;
		uint8_t bit5 : 1;
		uint8_t bit6 : 1;
		uint8_t bit7 : 1;
		uint8_t bit8 : 1;
		uint8_t bit9 : 1;
		uint8_t bit10 : 1;
		uint8_t bit11 : 1;
		uint8_t bit12 : 1;
		uint8_t bit13 : 1;
		uint8_t bit14 : 1;
		uint8_t bit15 : 1;
		uint8_t bit16 : 1;
	};
}X2;

typedef struct{
	uint8_t bytes[4];
}U4;

typedef struct{
	int8_t bytes[4];
}I4;

typedef union {
	// De cette facon il est possible de modifier byte en modifiant bits
	uint8_t bytes[4];
	struct {
		uint8_t bit1 : 1;
		uint8_t bit2 : 1;
		uint8_t bit3 : 1;
		uint8_t bit4 : 1;
		uint8_t bit5 : 1;
		uint8_t bit6 : 1;
		uint8_t bit7 : 1;
		uint8_t bit8 : 1;
		uint8_t bit9 : 1;
		uint8_t bit10 : 1;
		uint8_t bit11 : 1;
		uint8_t bit12 : 1;
		uint8_t bit13 : 1;
		uint8_t bit14 : 1;
		uint8_t bit15 : 1;
		uint8_t bit16 : 1;
		uint8_t bit17 : 1;
		uint8_t bit18 : 1;
		uint8_t bit19 : 1;
		uint8_t bit20 : 1;
		uint8_t bit21 : 1;
		uint8_t bit22 : 1;
		uint8_t bit23 : 1;
		uint8_t bit24 : 1;
		uint8_t bit25 : 1;
		uint8_t bit26 : 1;
		uint8_t bit27 : 1;
		uint8_t bit28 : 1;
		uint8_t bit29 : 1;
		uint8_t bit30 : 1;
		uint8_t bit31 : 1;
		uint8_t bit32 : 1;
	};
}X4;

/* UBX_NAV_SIG - SIGLAGS  */
typedef union {
	struct {
		uint8_t health : 2;
		uint8_t prSmoothed : 1;
		uint8_t prUsed : 1;
		uint8_t crUsed : 1;
		uint8_t doUsed : 1;
		uint8_t prCorrUsed : 1;
		uint8_t crCorrUsed : 1;
		uint8_t doCorrUsed : 1;
	};
	uint8_t bytes[2]; // Représentation entière de 8 bits
} X2_sigFlags;
typedef union {
	struct {
		uint8_t validTOW : 1;
		uint8_t validWKN : 1;
		uint8_t validUTC : 1;
		uint8_t utcStandard : 4;
	};
	uint8_t bytes[1]; // Représentation entière de 8 bits
} X1_valid;


#endif /* INC_GNSS_UBX_TYPES_H_ */
