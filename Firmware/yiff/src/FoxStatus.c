/*
 * FoxStatus.c
 *
 *  Created on: May 14, 2021
 *      Author: fossa
 */

#include <FoxStatus.h>
#include <EEPROM.h>

void FoxState_Init(void)
{
	FoxState.BatteryLevel = 1.0f;
	FoxState.CurrentTime = YHL_TIME_DAY_ZERO_TIMESTAMP;
	FoxState.IsTXOn = false;
	FoxState.GlobalState.IsArmed = false;
	FoxState.GlobalState.CurrentState = GfsStandby;
	FoxState.CycleState.CycleState = CsPause;
	FoxState.IsEEPROMHeadersInitialized = false; /* Headers aren't initialized yet, self-diagnostics unable to use EEPROM */
	FoxState.SupressDrawing = true; /* Display is not initialized yet */
	FoxState.ForceCarrierOff = false;
	FoxState.ForceCarrierOn = false;
	FoxState.IsNotReportedManualProfileChanges = false;

	FoxState.ServiceSettings.IsForceTx = false;

	PendingCommandsFlags.FoxStateNameChanged = false;
	PendingCommandsFlags.NeedToAddNewProfile = false;
	PendingCommandsFlags.NeedToSwitchProfile = false;
	PendingCommandsFlags.NeedToSetProfileName = false;

	PendingCommandsFlags.NeedToSetFrequency = false;
	PendingCommandsFlags.NeedToSetFrequencyByHandpaws = false;

	PendingCommandsFlags.NeedToSetCode = false;
	PendingCommandsFlags.NeedToSetCodeByHandpaws = false;

	PendingCommandsFlags.NeedToSetSpeed = false;
	PendingCommandsFlags.NeedToSetSpeedByHandpaws = false;

	PendingCommandsFlags.NeedToSetCycle = false;
	PendingCommandsFlags.NeedToSetCycleByHandpaws = false;

	PendingCommandsFlags.NeedToSetEndingToneDuration = false;
	PendingCommandsFlags.NeedToSetEndingToneDurationByHandpaws = false;

	PendingCommandsFlags.NeedToSetBeginAndEndTimes = false;
	PendingCommandsFlags.NeedToSetBeginAndEndTimesByHandpaws = false;

	PendingCommandsFlags.NeedToSetPower = false;
	PendingCommandsFlags.NeedToSetPowerByHandpaws = false;

	PendingCommandsFlags.NeedToArmFox = false;
	PendingCommandsFlags.NeedToArmFoxByHandpaws = false;

	PendingCommandsFlags.NeedToResetLastFailureCode = false;
	PendingCommandsFlags.NeedToUpdateSerialNumber = false;

	PendingCommandsFlags.NeedToSetUbattADCToUbattVoltsFactors = false;
	PendingCommandsFlags.NeedToSetUbattVoltsToBattLevelFactors = false;
	PendingCommandsFlags.NeedToSetU80mADCtoU80mVoltsFactors = false;
	PendingCommandsFlags.NeedToSetP80mToU80mFactors = false;
	PendingCommandsFlags.NeedToSetUantADCToUantVoltsFactors = false;

	PendingCommandsFlags.NeedToForceTx = false;

	FoxState.CurrentDisplay = StatusDisplay;
}

void FoxState_CorrectDateTime(void)
{
	/* TODO: Fix code duplication*/
	Time CurrentRTCTime;
	CurrentRTCTime.Days = CurrentDate.Year * 365 + CurrentDate.Month * 30 + CurrentDate.Date; /* We don't need precise conversion here */
	CurrentRTCTime.Hours = CurrentTime.Hours;
	CurrentRTCTime.Minutes = CurrentTime.Minutes;
	CurrentRTCTime.Seconds = CurrentTime.Seconds;

	FoxState.CurrentTime = TimeToTimestamp(CurrentRTCTime);
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

bool FoxState_IsCycleDurationsValid(uint32_t txTime, uint32_t pauseTime)
{
	if ((txTime < YHL_MIN_TX_DURATION) || (pauseTime < YHL_MIN_PAUSE_DURATION))
	{
		return false;
	}

	return true;
}

bool FoxState_SetCycleDurations(uint32_t txTime, uint32_t pauseTime)
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

bool FoxState_IsBeginAndEndTimespansValid(uint32_t beginTimespan, uint32_t endTimespan)
{
	if (beginTimespan >= YHL_TIME_DAY_IN_SECONDS || endTimespan >= YHL_TIME_DAY_IN_SECONDS)
	{
		return false;
	}

	return true;
}

void FoxState_SetBeginAndEndTimespans(uint32_t beginTimespan, uint32_t endTimespan)
{
	if (!FoxState_IsBeginAndEndTimespansValid(beginTimespan, endTimespan))
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_InvalidStartOrEndTimespans);
	}

	FoxState.GlobalState.StartTimespan = beginTimespan;
	FoxState.GlobalState.EndTimespan = endTimespan;
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

bool FoxState_SetUbattADCToUbattVoltsFactors(bool reset, float a, float b)
{
	/* No fox-side check (at least for now) */
	if (reset)
	{
		FoxState.ServiceSettings.SetThisUbattADCToUbattVoltsAFactor = YHL_DEFAULT_ADC_UBATT_A;
		FoxState.ServiceSettings.SetThisUbattADCToUbattVoltsBFactor = YHL_DEFAULT_ADC_UBATT_B;
	}
	else
	{
		FoxState.ServiceSettings.SetThisUbattADCToUbattVoltsAFactor = a;
		FoxState.ServiceSettings.SetThisUbattADCToUbattVoltsBFactor = b;
	}


	PendingCommandsFlags.NeedToSetUbattADCToUbattVoltsFactors = true;

	return true;
}

bool FoxState_SetUbattVoltsToBattLevelFactors(bool reset, float a, float b)
{
	/* No fox-side check (at least for now) */
	if (reset)
	{
		FoxState.ServiceSettings.SetThisUbattVoltsToBattLevelAFactor = YHL_DEFAULT_BATTERY_LEVEL_A;
		FoxState.ServiceSettings.SetThisUbattVoltsToBattLevelBFactor = YHL_DEFAULT_BATTERY_LEVEL_B;
	}
	else
	{
		FoxState.ServiceSettings.SetThisUbattVoltsToBattLevelAFactor = a;
		FoxState.ServiceSettings.SetThisUbattVoltsToBattLevelBFactor = b;
	}

	PendingCommandsFlags.NeedToSetUbattVoltsToBattLevelFactors = true;

	return true;
}

bool FoxState_SetU80mADCtoU80mVoltsFactors(bool reset, float a, float b)
{
	/* No fox-side check (at least for now) */
	if (reset)
	{
		FoxState.ServiceSettings.SetThisU80mADCtoU80mVoltsAFactor = YHL_DEFAULT_ADC_U80M_A;
		FoxState.ServiceSettings.SetThisU80mADCtoU80mVoltsBFactor = YHL_DEFAULT_ADC_U80M_B;
	}
	else
	{
		FoxState.ServiceSettings.SetThisU80mADCtoU80mVoltsAFactor = a;
		FoxState.ServiceSettings.SetThisU80mADCtoU80mVoltsBFactor = b;
	}

	PendingCommandsFlags.NeedToSetU80mADCtoU80mVoltsFactors = true;

	return true;
}

bool FoxState_SetP80mToU80mFactors(bool reset, float a, float b)
{
	/* No fox-side check (at least for now) */
	if (reset)
	{
		FoxState.ServiceSettings.SetThisP80mToU80mAFactor = YHL_DEFAULT_P80M_A;
		FoxState.ServiceSettings.SetThisP80mToU80mBFactor = YHL_DEFAULT_P80M_B;
	}
	else
	{
		FoxState.ServiceSettings.SetThisP80mToU80mAFactor = a;
		FoxState.ServiceSettings.SetThisP80mToU80mBFactor = b;
	}

	PendingCommandsFlags.NeedToSetP80mToU80mFactors = true;

	return true;
}

bool FoxState_SetUantADCToUantVoltsFactors(bool reset, float a, float b)
{
	/* No fox-side check (at least for now) */
	if (reset)
	{
		FoxState.ServiceSettings.SetThisUantADCToUantVoltsAFactor = YHL_DEFAULT_ADC_UANT_A;
		FoxState.ServiceSettings.SetThisUantADCToUantVoltsBFactor = YHL_DEFAULT_ADC_UANT_B;
	}
	else
	{
		FoxState.ServiceSettings.SetThisUantADCToUantVoltsAFactor = a;
		FoxState.ServiceSettings.SetThisUantADCToUantVoltsBFactor = b;
	}

	PendingCommandsFlags.NeedToSetUantADCToUantVoltsFactors = true;

	return true;
}

