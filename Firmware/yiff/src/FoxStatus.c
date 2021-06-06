/*
 * FoxStatus.c
 *
 *  Created on: May 14, 2021
 *      Author: fossa
 */

#include <main.h>

void FoxState_Init(void)
{
	PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = false; /* At load current profile is actual */

	PendingCommandsFlags.FoxStateNameChanged = false;
	PendingCommandsFlags.NeedToAddNewProfile = false;
	PendingCommandsFlags.NeedToSwitchProfile = false;
	PendingCommandsFlags.NeedToSetProfileName = false;
	PendingCommandsFlags.NeedToSetFrequency = false;
	PendingCommandsFlags.NeedToSetCode = false;
	PendingCommandsFlags.NeedToSetSpeed = false;
	PendingCommandsFlags.NeedToSetCycle = false;
}

void FoxState_CorrectDateTime(void)
{
	FoxState.CurrentTime.Hours = CurrentTime.Hours;
	FoxState.CurrentTime.Minutes = CurrentTime.Minutes;
	FoxState.CurrentTime.Seconds = CurrentTime.Seconds;
}

bool FoxState_IsFrequencyValid(bool is144MHz, uint32_t frequency)
{
	if (is144MHz)
	{
		if (frequency < YHL_MIN_2M_FREQUENCY || frequency > YHL_MAX_2M_FREQUENCY)
		{
			return false;
		}
	}
	else
	{
		if (frequency < YHL_MIN_80M_FREQUENCY || frequency > YHL_MAX_80M_FREQUENCY)
		{
			return false;
		}
	}

	return true;
}
