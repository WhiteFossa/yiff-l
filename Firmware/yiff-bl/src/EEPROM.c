/*
 * EEPROM.c
 *
 *  Created on: Feb 13, 2022
 *      Author: fossa
 */

#include <EEPROM.h>

void EEPROM_ReadConstantHeader(EEPROMConstantHeaderStruct* constantHeader)
{
	L2HAL_24x_ReadData(&EEPROMContext, 0x00, (uint8_t*)constantHeader, sizeof(EEPROMConstantHeaderStruct));
}

bool EEPROM_CheckConstantHeader(EEPROMConstantHeaderStruct* constantHeader)
{
	uint32_t existingCRC = constantHeader->CRCSum;
	constantHeader->CRCSum = 0;
	volatile uint32_t calculatedCRC = L2HAL_CRC_Calculate(&CRC_Context, (uint8_t*)constantHeader, sizeof(EEPROMConstantHeaderStruct));
	bool result = existingCRC == calculatedCRC;
	constantHeader->CRCSum = existingCRC;

	return result;
}

void EEPROM_WriteConstantHeader(EEPROMConstantHeaderStruct* constantHeader)
{
	constantHeader->CRCSum = 0;
	constantHeader->CRCSum = L2HAL_CRC_Calculate(&CRC_Context, (uint8_t*)constantHeader, sizeof(EEPROMConstantHeaderStruct));
	L2HAL_24x_WriteData(&EEPROMContext, 0x00, (uint8_t*)constantHeader, sizeof(EEPROMConstantHeaderStruct));
}

