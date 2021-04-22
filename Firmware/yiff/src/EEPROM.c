/*
 * EEPROM.c
 *
 *  Created on: Apr 21, 2021
 *      Author: fossa
 */

#include <main.h>

void EEPROM_Format(void)
{
	EEPROMHeaderStruct defaultHeader;

	defaultHeader.EEPROMVersion = YHL_EEPROM_DATA_VERSION;
	defaultHeader.NumberOfProfiles = 1;

	/* Zero in profile address means "not allocated" */
	memset(defaultHeader.ProfilesAddresses, 0x00, YHL_MAX_PROFILES_COUNT * 2); /* *2 because each address is 2 bytes wide */
	defaultHeader.ProfilesAddresses[0] = sizeof(EEPROMHeaderStruct) + 1U; /* First profile goes after header */
	defaultHeader.ProfileInUse = 0;
	defaultHeader.CRCSum = 0;

	/* Calculating CRC for header with CRCSum = 0 */
	defaultHeader.CRCSum = L2HAL_CRC_Calculate(&CRC_Context, &defaultHeader, sizeof(EEPROMHeaderStruct));

	EEPROM_WriteHeader(&defaultHeader);
}

void EEPROM_WriteHeader(EEPROMHeaderStruct* header)
{
	L2HAL_24x_WriteData(&EEPROMContext, 0x00, (uint8_t*)header, sizeof(EEPROMHeaderStruct));
}

void EEPROM_ReadHeader(EEPROMHeaderStruct* header)
{
	L2HAL_24x_ReadData(&EEPROMContext, 0x00, (uint8_t*)header, sizeof(EEPROMHeaderStruct));
}

bool EEPROM_CheckHeader(EEPROMHeaderStruct* header)
{
	uint32_t existingCRC = header->CRCSum;

	header->CRCSum = 0;

	uint32_t calculatedCRC = L2HAL_CRC_Calculate(&CRC_Context, header, sizeof(EEPROMHeaderStruct));

	bool result = existingCRC == calculatedCRC;

	header->CRCSum = existingCRC;

	return result;
}
