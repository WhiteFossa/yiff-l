/*
 * Sleepmodes.c
 *
 *  Created on: Nov 6, 2021
 *      Author: fossa
 */

#include <Sleepmodes.h>
#include <HardwareLogic.h>

void Sleepmodes_Init(void)
{
	/* Starting with awakened fox */
	FoxState.Sleepmodes.Mode = SleepmodeAwake;
	FoxState.Sleepmodes.SleepTimer = YHL_SLEEPMODES_SLEEP_TIMEOUT;

	RTC_AddOnNewSecondListener(&Sleepmodes_OnNewSecond);
}


void Sleepmodes_PreventSleep(void)
{
	switch (FoxState.Sleepmodes.Mode)
	{
		case SleepmodeAwake:
			FoxState.Sleepmodes.SleepTimer = YHL_SLEEPMODES_SLEEP_TIMEOUT; /* Resetting the timer, preventing sleep */
			break;

		case SleepmodeSleep:
			Sleepmodes_WakeFromSleep();
			break;

		case SleepmodeDeepSleep:
			Sleepmodes_WakeFromDeepSleep();
			Sleepmodes_WakeFromSleep();
			break;

		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongSleepmodeInSleepmodesPreventSleep);
			break;
	}
}


void Sleepmodes_PreventDeepSleep(void)
{
	switch (FoxState.Sleepmodes.Mode)
	{
		case SleepmodeAwake:
			/* Do nothing, downcounting to sleep mode */
			break;

		case SleepmodeSleep:
			FoxState.Sleepmodes.SleepTimer = YHL_SLEEPMODES_DEEP_SLEEP_TIMEOUT;
			break;

		case SleepmodeDeepSleep:
			Sleepmodes_WakeFromDeepSleep();
			break;
	}
}


void Sleepmodes_OnNewSecond(void)
{
	if (FoxState.Sleepmodes.SleepTimer > 0)
	{
		FoxState.Sleepmodes.SleepTimer --;

		if (0 == FoxState.Sleepmodes.SleepTimer)
		{
			/* Going deeper */
			switch (FoxState.Sleepmodes.Mode)
			{
				case SleepmodeAwake: /* Awake -> Sleep */
					EmitEnteringSleepmodeEvent();
					Sleepmodes_EnterSleep();
					break;

				case SleepmodeSleep: /* Sleep -> Deep sleep */
					Sleepmodes_EnterDeepSleep();
					break;

				case SleepmodeDeepSleep:
					SelfDiagnostics_HaltOnFailure(YhlFailureCause_CantGoDeeperInSleepmodesOnNewSecond);
					break;

				default:
					SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongSleepmodeInSleepmodesOnNewSecond);
					break;
			}
		}
	}
}


void Sleepmodes_DeactivateControls(void)
{
	HL_TurnBluetoothOff();
	HL_TurnDisplayOff();
}


void Sleepmodes_ActivateControls(void)
{
	HL_TurnBluetoothOn();
	HL_TurnDisplayOn();

	/* We need to restart UART listening after low-level UART commands in Bluetooth initialization code */
	UART_StartListen(&OnNewRawPacket);
}

void Sleepmodes_EnterSleep(void)
{
	/* We can enter sleep only from awaken state */
	if (FoxState.Sleepmodes.Mode != SleepmodeAwake)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AlreadySleeping);
	}

	Sleepmodes_DeactivateControls(); /* Some regulators are disabled here */

	FoxState.Sleepmodes.SleepTimer = YHL_SLEEPMODES_DEEP_SLEEP_TIMEOUT;
	FoxState.Sleepmodes.Mode = SleepmodeSleep;
}

void Sleepmodes_EnterDeepSleep(void)
{
	/* We can enter deep sleep only from ordinary sleep */
	if (FoxState.Sleepmodes.Mode != SleepmodeSleep)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AlreadyDeepSleeping);
	}

	HAL_SwitchManipulator(false);
	HAL_SetAntennaMatchingValue(0);
	HAL_Activate80M(false);
	HAL_Activate2M(false);
	HAL_PutSynthesizerToSleep();

	/* Disabling regulators */
	HAL_SwitchUBattCheck(false);
	HAL_ActivateFox(false); /* 5V regulator shutdown */

	/* Entering economy mode */
	HAL_EnterEconomyMode();

	FoxState.Sleepmodes.SleepTimer = 0; /* No deeper sleep is possible */
	FoxState.Sleepmodes.Mode = SleepmodeDeepSleep;
}

void Sleepmodes_WakeFromSleep(void)
{
	if (FoxState.Sleepmodes.Mode != SleepmodeSleep)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_NotSleeping);
	}

	Sleepmodes_ActivateControls();

	FoxState.Sleepmodes.SleepTimer = YHL_SLEEPMODES_SLEEP_TIMEOUT;
	FoxState.Sleepmodes.Mode = SleepmodeAwake;
}

void Sleepmodes_WakeFromDeepSleep(void)
{
	if (FoxState.Sleepmodes.Mode != SleepmodeDeepSleep)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_NotDeepSleeping);
	}

	/* Exiting economy mode */
	HAL_ExitEconomyMode();

	HAL_ActivateFox(true);
	HAL_SwitchUBattCheck(true);

	FoxState.Sleepmodes.SleepTimer = YHL_SLEEPMODES_DEEP_SLEEP_TIMEOUT;
	FoxState.Sleepmodes.Mode = SleepmodeSleep;
}
