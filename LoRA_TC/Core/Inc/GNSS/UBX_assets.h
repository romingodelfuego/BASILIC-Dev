/*
 * UBX_assets.h
 *
 *  Created on: Jul 22, 2024
 *      Author: romain.pace
 */

#ifndef INC_GNSS_UBX_ASSETS_H_
#define INC_GNSS_UBX_ASSETS_H_

typedef struct{
	uint8_t array[1];
}U1;

typedef struct{
	int8_t array[1];
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
	} bits;
}X1;

typedef struct{
	uint8_t array[2];
}U2;

typedef struct{
	int8_t array[2];
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
	} bits;
}X2;

typedef struct{
	uint8_t array[4];
}U4;

typedef struct{
	int8_t array[4];
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
	} bits;
}X4;









typedef enum {
	U1,U2,U3,U4,
	I1,I2,I3,I4
}UBX_data_type;

typedef enum {
	U1,U2,U3,U4,
	I1,I2,I3,I4
}UBX_field_name;


#endif /* INC_GNSS_UBX_ASSETS_H_ */
