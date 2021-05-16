/*
 * EEPROM.c
 *
 *  Created on: Apr 21, 2021
 *      Author: fossa
 */

#include <main.h>

void EEPROM_Format(void)
{
	/* Writing constant header*/
	EEPROMConstantHeaderStruct constantHeader;
	constantHeader.Version = YHL_EEPROM_DATA_VERSION;
	constantHeader.HeaderAddress = sizeof(EEPROMConstantHeaderStruct); /* Main header goes immediately after constant header */
	EEPROM_WriteConstantHeader(&constantHeader);

	/* Writing main header */
	EEPROMHeaderStruct defaultHeader;
	sprintf(defaultHeader.Name, YHL_DEFAULT_FOX_NAME);
	defaultHeader.NumberOfProfiles = 1;

	/* Zero in profile address means "not allocated" */
	memset(defaultHeader.ProfilesAddresses, 0x00, YHL_MAX_PROFILES_COUNT * 2); /* *2 because each address is 2 bytes wide */
	defaultHeader.ProfilesAddresses[0] = (uint16_t)(constantHeader.HeaderAddress + sizeof(EEPROMHeaderStruct)); /* First profile goes after header */
	defaultHeader.ProfileInUse = 0;
	defaultHeader.CRCSum = 0;
	EEPROM_WriteHeader(&defaultHeader, constantHeader.HeaderAddress);
}

void EEPROM_WriteConstantHeader(EEPROMConstantHeaderStruct* constantHeader)
{
	constantHeader->CRCSum = 0;
	constantHeader->CRCSum = L2HAL_CRC_Calculate(&CRC_Context, (uint8_t*)constantHeader, sizeof(EEPROMConstantHeaderStruct));
	L2HAL_24x_WriteData(&EEPROMContext, 0x00, (uint8_t*)constantHeader, sizeof(EEPROMConstantHeaderStruct));
}

void EEPROM_ReadConstantHeader(EEPROMConstantHeaderStruct* constantHeader)
{
	L2HAL_24x_ReadData(&EEPROMContext, 0x00, (uint8_t*)constantHeader, sizeof(EEPROMConstantHeaderStruct));
}

bool EEPROM_CheckConstantHeader(EEPROMConstantHeaderStruct* constantHeader)
{
	uint32_t existingCRC = constantHeader->CRCSum;
	constantHeader->CRCSum = 0;
	uint32_t calculatedCRC = L2HAL_CRC_Calculate(&CRC_Context, (uint8_t*)constantHeader, sizeof(EEPROMConstantHeaderStruct));
	bool result = existingCRC == calculatedCRC;
	constantHeader->CRCSum = existingCRC;

	return result;
}

void EEPROM_WriteHeader(EEPROMHeaderStruct* header, uint16_t address)
{
	/* Calculating CRC for header with CRCSum = 0 */
	header->CRCSum = 0;
	header->CRCSum = L2HAL_CRC_Calculate(&CRC_Context, (uint8_t*)header, sizeof(EEPROMHeaderStruct));
	L2HAL_24x_WriteData(&EEPROMContext, address, (uint8_t*)header, sizeof(EEPROMHeaderStruct));
}

void EEPROM_ReadHeader(EEPROMHeaderStruct* header, uint16_t address)
{
	L2HAL_24x_ReadData(&EEPROMContext, address, (uint8_t*)header, sizeof(EEPROMHeaderStruct));
}

bool EEPROM_CheckHeader(EEPROMHeaderStruct* header)
{
	uint32_t existingCRC = header->CRCSum;
	header->CRCSum = 0;
	uint32_t calculatedCRC = L2HAL_CRC_Calculate(&CRC_Context, (uint8_t*)header, sizeof(EEPROMHeaderStruct));
	bool result = existingCRC == calculatedCRC;
	header->CRCSum = existingCRC;

	return result;
}

void EEPROM_Init(void)
{
	/* Loading headers and checking them for correctness */
	if (!EEPROM_InitHeaders())
	{
		EEPROM_Format();
		if (!EEPROM_InitHeaders())
		{
			/**
			 * EEPROM is faulty
			 */
			L2HAL_Error(Generic);
		}
	}

	/* Validating all existing profiles, trying to format damaged ones */
	for (uint8_t profileIndex = 0; profileIndex < EEPROM_Header.NumberOfProfiles; profileIndex ++)
	{
		uint16_t profileAddress = EEPROM_Header.ProfilesAddresses[profileIndex];

		EEPROMProfileStruct profile;
		EEPROM_ReadProfile(&profile, profileAddress);
		if (!EEPROM_CheckProfile(&profile))
		{
			profile = EEPROM_GenerateDefaultProfile();
			EEPROM_WriteProfile(&profile, profileAddress);

			EEPROM_ReadProfile(&profile, profileAddress);
			if (!EEPROM_CheckProfile(&profile))
			{
				/* Faulty EEPROM */
				L2HAL_Error(Generic);
			}
		}
	}

	/* Loading current profile */
	uint16_t profileAddress = EEPROM_Header.ProfilesAddresses[EEPROM_Header.ProfileInUse];
	EEPROM_ReadProfile(&EEPROM_CurrentProfile, profileAddress);
}

bool EEPROM_InitHeaders(void)
{
	/* Constant header */
	EEPROM_ReadConstantHeader(&EEPROM_ConstantHeader);

	bool isConstantHeaderValid = EEPROM_CheckConstantHeader(&EEPROM_ConstantHeader);
	if (!isConstantHeaderValid)
	{
		return false;
	}

	/* Main header */
	EEPROM_ReadHeader(&EEPROM_Header, EEPROM_ConstantHeader.HeaderAddress);
	bool isHeaderValid = EEPROM_CheckHeader(&EEPROM_Header);
	if (!isHeaderValid)
	{
		return false;
	}

	return true;
}

void EEPROM_ReadProfile(EEPROMProfileStruct* profile, uint16_t address)
{
	L2HAL_24x_ReadData(&EEPROMContext, address, (uint8_t*)profile, sizeof(EEPROMProfileStruct));
}

bool EEPROM_CheckProfile(EEPROMProfileStruct* profile)
{
	uint32_t existingCRC = profile->CRCSum;
	profile->CRCSum = 0;
	uint32_t calculatedCRC = L2HAL_CRC_Calculate(&CRC_Context, (uint8_t*)profile, sizeof(EEPROMProfileStruct));
	bool result = existingCRC == calculatedCRC;
	profile->CRCSum = existingCRC;

	return result;
}

void EEPROM_WriteProfile(EEPROMProfileStruct* profile, uint16_t address)
{
	profile->CRCSum = 0;
	profile->CRCSum = L2HAL_CRC_Calculate(&CRC_Context, (uint8_t*)profile, sizeof(EEPROMProfileStruct));
	L2HAL_24x_WriteData(&EEPROMContext, address, (uint8_t*)profile, sizeof(EEPROMProfileStruct));
}

EEPROMProfileStruct EEPROM_GenerateDefaultProfile(void)
{
	EEPROMProfileStruct result;

	sprintf(result.Name, YHL_DEFAULT_PROFILE_NAME);

	result.Frequency.Is144MHz = false;
	result.Frequency.FrequencyHz = 3500000U;

	result.Code = F1;

	result.IsFast = true;

	result.Cycle.IsContinuous = false;
	result.Cycle.TxTime.Hours = 0;
	result.Cycle.TxTime.Minutes = 1;
	result.Cycle.TxTime.Seconds = 0;
	result.Cycle.PauseTime.Hours = 0;
	result.Cycle.PauseTime.Minutes = 4;
	result.Cycle.PauseTime.Seconds = 0;

	result.EndingToneLength = 5;

	result.Power = 3.0f;

	result.StartTime.Hours = 10;
	result.StartTime.Minutes = 0;
	result.StartTime.Seconds = 0;

	result.EndTime.Hours = 13;
	result.EndTime.Minutes = 0;
	result.EndTime.Seconds = 0;

	result.CRCSum = 0;

	return result;
}

void EEPROM_LoadProfileIntoFoxState(FoxStateStruct* foxState, EEPROMProfileStruct* profile)
{
	GSM_Cancel();

	foxState->Frequency = profile->Frequency;

	foxState->Code = profile->Code;
	foxState->IsFast = profile->IsFast;

	foxState->Cycle = profile->Cycle;

	foxState->EndingToneLength = profile->EndingToneLength;

	foxState->Power = profile->Power;

	// TODO: Remove me, debug
	profile->StartTime.Hours = 0;
	profile->StartTime.Minutes = 1;
	profile->StartTime.Seconds = 0;

	profile->EndTime.Hours = 0;
	profile->EndTime.Minutes = 7;
	profile->EndTime.Seconds = 0;

	foxState->GlobalState.StartTime = profile->StartTime;
	foxState->GlobalState.EndTime = profile->EndTime;

	GSM_Program();
}

void EEPROM_UpdateHeader(void)
{
	EEPROM_WriteHeader(&EEPROM_Header, EEPROM_ConstantHeader.HeaderAddress);
}

void EEPROM_SwitchProfile(uint8_t profileId)
{
	if (profileId == EEPROM_Header.ProfileInUse)
	{
		return;
	}

	if (profileId >= EEPROM_Header.NumberOfProfiles)
	{
		L2HAL_Error(Generic);
	}

	EEPROM_Header.ProfileInUse = profileId;
	EEPROM_UpdateHeader();

	uint16_t profileAddress = EEPROM_Header.ProfilesAddresses[profileId];
	EEPROM_ReadProfile(&EEPROM_CurrentProfile,profileAddress);
	EEPROM_LoadProfileIntoFoxState(&FoxState, &EEPROM_CurrentProfile);
}

void EEPROM_AddProfile(void)
{
	if (EEPROM_Header.NumberOfProfiles == YHL_MAX_PROFILES_COUNT)
	{
		L2HAL_Error(Generic);
	}

	EEPROMProfileStruct newProfile = EEPROM_GenerateDefaultProfile();
	uint16_t newProfileAddress = EEPROM_Header.ProfilesAddresses[EEPROM_Header.NumberOfProfiles - 1] + sizeof(EEPROMProfileStruct);
	EEPROM_WriteProfile(&newProfile, newProfileAddress);

	EEPROM_Header.NumberOfProfiles ++;
	EEPROM_Header.ProfilesAddresses[EEPROM_Header.NumberOfProfiles - 1] = newProfileAddress;
	EEPROM_UpdateHeader();
}

EEPROMProfileStruct EEPROM_GetProfile(uint8_t profileId)
{
	if (profileId >= EEPROM_Header.NumberOfProfiles)
	{
		L2HAL_Error(Generic);
	}

	EEPROMProfileStruct result;
	uint16_t profileAddress = EEPROM_Header.ProfilesAddresses[profileId];
	EEPROM_ReadProfile(&result, profileAddress);

	return result;
}

void EEPROM_UpdateProfile(EEPROMProfileStruct* profile, uint8_t profileId)
{
	if (profileId >= EEPROM_Header.NumberOfProfiles)
	{
		L2HAL_Error(Generic);
	}

	uint16_t profileAddress = EEPROM_Header.ProfilesAddresses[profileId];
	EEPROM_WriteProfile(profile, profileAddress);

	if (profileId == EEPROM_Header.ProfileInUse)
	{
		EEPROM_ReadProfile(&EEPROM_CurrentProfile, profileAddress);
		EEPROM_LoadProfileIntoFoxState(&FoxState, &EEPROM_CurrentProfile);
	}
}
