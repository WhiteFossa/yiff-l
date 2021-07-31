/*
 * FoxStatus.c
 *
 *  Created on: May 14, 2021
 *      Author: fossa
 */

#include <main.h>

void FoxState_Init(void)
{
	FoxState.ForceCarrierOff = false;
	FoxState.ForceCarrierOn = false;

	PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = false; /* At load current profile is actual */

	PendingCommandsFlags.FoxStateNameChanged = false;
	PendingCommandsFlags.NeedToAddNewProfile = false;
	PendingCommandsFlags.NeedToSwitchProfile = false;
	PendingCommandsFlags.NeedToSetProfileName = false;
	PendingCommandsFlags.NeedToSetFrequency = false;
	PendingCommandsFlags.NeedToSetCode = false;
	PendingCommandsFlags.NeedToSetSpeed = false;
	PendingCommandsFlags.NeedToSetCycle = false;
	PendingCommandsFlags.NeedToSetEndingToneDuration = false;
	PendingCommandsFlags.NeedToSetBeginAndEndTimes = false;
	PendingCommandsFlags.NeedToSetPower = false;
	PendingCommandsFlags.NeedToArmFox = false;

	FoxState.CurrentDisplay = StatusDisplay;

	FoxState.MatchingDisplayData.IsMatchingInProgress = false;
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

bool FoxState_SetFrequency(bool is144MHz, uint32_t frequency)
{
	if (!FoxState_IsFrequencyValid(is144MHz, frequency))
	{
		return false;
	}

	FoxState.Frequency.Is144MHz = is144MHz;
	FoxState.Frequency.FrequencyHz = frequency;

	return true;
}

bool FoxState_IsCycleDurationsValid(Time txTime, Time pauseTime)
{
	Time minTxTime = TimeSinceDayBegin(YHL_MIN_TX_DURATION);
	Time minPauseTime = TimeSinceDayBegin(YHL_MIN_PAUSE_DURATION);

	if ((TIME1_LESS == CompareTimes(txTime, minTxTime)) || (TIME1_LESS == CompareTimes(pauseTime, minPauseTime)))
	{
		return false;
	}

	return true;
}

bool FoxState_SetCycleDurations(Time txTime, Time pauseTime)
{
	if (!FoxState_IsCycleDurationsValid(txTime, pauseTime))
	{
		return false;
	}

	FoxState.Cycle.TxTime = txTime;
	FoxState.Cycle.PauseTime = pauseTime;

	return true;
}

bool FoxState_IsEndingtoneDurationValid(uint8_t endingtoneDuration)
{
	if (endingtoneDuration > YHL_MAX_ENDINGTONE_DURATION)
	{
		return false;
	}

	return true;
}

bool FoxState_SetEndingtoneDuration(uint8_t endingtoneDuration)
{
	if (!FoxState_IsEndingtoneDurationValid(endingtoneDuration))
	{
		return false;
	}

	FoxState.EndingToneLength = endingtoneDuration;

	return true;
}

bool FoxState_IsBeginAndEndTimesValid(Time beginTime, Time endTime)
{
	int8_t comparisonResult;

	comparisonResult = CompareTimes(beginTime, endTime);

	if ((TIME2_LESS == comparisonResult) || (TIMES_EQUAL == comparisonResult))
	{
		return false;
	}

	Time wholeDay = TimeSinceDayBegin(YHL_TIME_DAY_IN_SECONDS);

	comparisonResult = CompareTimes(endTime, wholeDay);

	if ((TIME2_LESS == comparisonResult) || (TIMES_EQUAL == comparisonResult))
	{
		return false;
	}

	return true;
}

bool FoxState_SetBeginAndEndTimes(Time beginTime, Time endTime)
{
	if (!FoxState_IsBeginAndEndTimesValid(beginTime, endTime))
	{
		return false;
	}

	FoxState.GlobalState.StartTime = beginTime;
	FoxState.GlobalState.EndTime = endTime;

	return true;
}

bool FoxState_IsPowerValid(float power)
{
	if (FoxState.Frequency.Is144MHz)
	{
		return false; /* Power setting is not supported in 144MHz mode */
	}

	if ((power < YHL_MIN_POWER_80M) || (power > YHL_MAX_POWER_80M))
	{
		return false;
	}

	return true;
}

bool FoxState_SetPower(float power)
{
	if (!FoxState_IsPowerValid(power))
	{
		return false;
	}

	FoxState.Power = power;

	return true;
}

