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
	FoxState.Sleepmodes.IsSleeping = false;

	Sleepmodes_ResetSleepTimer();

	RTC_AddOnNewSecondListener(&Sleepmodes_OnNewSecond);
}


void Sleepmodes_ResetSleepTimer(void)
{
	FoxState.Sleepmodes.SleepTimer = YHL_SLEEPMODES_SLEEP_TIMEOUT;

	if (FoxState.Sleepmodes.IsSleeping)
	{
		Sleepmodes_WakeFromDeepsleep();
	}
}


void Sleepmodes_OnNewSecond(void)
{
	/* Controls timer */
	if (!FoxState.Sleepmodes.IsSleeping)
	{
		FoxState.Sleepmodes.SleepTimer --;

		if (FoxState.Sleepmodes.SleepTimer == 0)
		{
			Sleepmodes_EnterDeepsleep();
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
}

void Sleepmodes_EnterDeepsleep(void)
{
	Sleepmodes_DeactivateControls(); /* Some regulators are disabled here */

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

	FoxState.Sleepmodes.IsSleeping = true;
}

void Sleepmodes_WakeFromDeepsleep(void)
{
	/* Exiting economy mode */
	HAL_ExitEconomyMode();

	HAL_ActivateFox(true);
	HAL_SwitchUBattCheck(true);

	Sleepmodes_ActivateControls();

	FoxState.Sleepmodes.IsSleeping = false;
}
