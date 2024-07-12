/*
 * LORACom.c
 *
 *  Created on: Jun 7, 2024
 *      Author: romain.pace
 */


#include "LORA/LORACom.h"
#include "LORA/RFM9x.h"
#include "shared.h"
LORACom_HandleTypeDef hLORACom;

void LORACom_Init(SPI_HandleTypeDef* hspi,UART_HandleTypeDef* huartDebug)
{
	hLORACom.hspi = hspi;
	hLORACom.huartDebug = huartDebug;
}




