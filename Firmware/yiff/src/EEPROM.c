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
	snprintf(defaultHeader.Name, YHL_PROFILE_NAME_MEMORY_SIZE, YHL_DEFAULT_FOX_NAME);
	defaultHeader.NumberOfProfiles = 1;

	/* Zero in profile address means "not allocated" */
	memset(defaultHeader.ProfilesAddresses, 0x00, YHL_MAX_PROFILES_COUNT * 2); /* *2 because each address is 2 bytes wide */
	defaultHeader.ProfilesAddresses[0] = (uint16_t)(constantHeader.HeaderAddress + sizeof(EEPROMHeaderStruct)); /* First profile goes after header */
	defaultHeader.ProfileInUse = 0;

	/* Factors for ADC measurements */
	defaultHeader.UBattADCA = YHL_DEFAULT_ADC_UBATT_A;
	defaultHeader.UBattADCB = YHL_DEFAULT_ADC_UBATT_B;

	defaultHeader.U80mADCA = YHL_DEFAULT_ADC_U80M_A;
	defaultHeader.U80mADCB = YHL_DEFAULT_ADC_U80M_B;

	/* Factors for output power */
	defaultHeader.P80mA = YHL_DEFAULT_P80M_A;
	defaultHeader.P80mB = YHL_DEFAULT_P80M_B;

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

	snprintf(result.Name, YHL_PROFILE_NAME_MEMORY_SIZE, YHL_DEFAULT_PROFILE_NAME);

	result.Frequency.Is144MHz = false;
	result.Frequency.FrequencyHz = YHL_DEFAULT_80M_FREQUENCY;

	result.Code = F1;

	result.IsFast = true;

	result.Cycle.IsContinuous = false;
	result.Cycle.TxTime = 60;
	result.Cycle.PauseTime = 240;

	result.EndingToneLength = 5;

	result.Power = 3.0f;

	Time time;

	time.Days = 0;
	time.Hours = 10;
	time.Minutes = 0;
	time.Seconds = 0;
	result.StartTime = TimeToTimestamp(time);

	time.Days = 0;
	time.Hours = 13;
	time.Minutes = 0;
	time.Seconds = 0;
	result.EndTime = TimeToTimestamp(time);

	result.CRCSum = 0;

	return result;
}

void EEPROM_LoadProfileIntoFoxState(FoxStateStruct* foxState, EEPROMProfileStruct* profile)
{
	GSM_Disarm();

	foxState->Frequency = profile->Frequency;

	foxState->Code = profile->Code;
	foxState->IsFast = profile->IsFast;

	foxState->Cycle = profile->Cycle;

	foxState->EndingToneLength = profile->EndingToneLength;

	foxState->Power = profile->Power;

	foxState->GlobalState.StartTime = profile->StartTime;
	foxState->GlobalState.EndTime = profile->EndTime;
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

bool EEPROM_IsProfileIdValid(uint8_t profileId)
{
	if (profileId > EEPROM_Header.NumberOfProfiles - 1U)
	{
		return false;
	}

	return true;
}

void EEPROM_UpdateCurrentProfile()
{
	EEPROM_UpdateProfile(&EEPROM_CurrentProfile, EEPROM_Header.ProfileInUse);
}
