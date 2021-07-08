/*
 * HardwareLogic.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>
#include <HardwareLogicPrivate.h>

void HL_Init()
{
	HAL_IntiHardware();

	HL_IsU80mFeedbackActive = false;
	HL_U80mLockCounter = 0;
	HL_U80mLockCallback = NULL;
	HL_IsFoxPrepared = false;

	HAL_SetU80mMeasuredCallback(HL_U80mMeasurementCallback);
}

void ProcessManipulatorFoxStateChange(void)
{
	if (FoxState.IsMorseTx || FoxState.CycleState.IsEndingTone)
	{
		HAL_SwitchManipulator(true);
	}
	else
	{
		HAL_SwitchManipulator(false);
	}
}

void HL_SetupU80m(float targetVoltage)
{
	if (targetVoltage < YHL_HL_MIN_U80M || targetVoltage > YHL_HL_MAX_U80M)
	{
		L2HAL_Error(Generic);
	}

	HL_U80mTarget = targetVoltage;

	/* Setting lowest possible voltage, then activating feedback */
	HAL_SetU80mRegulatorCode(HAL_U80M_LOWEST_VOLTAGE_CODE);

	HL_U80mLockCounter = 0;
	HL_IsU80mFeedbackActive = true;
}

void HL_U80mMeasurementCallback(void)
{
	if (!HL_IsU80mFeedbackActive)
	{
		return;
	}

	float currentVoltage = HAL_GetU80mVolts();

	float delta = fabs(currentVoltage - HL_U80mTarget);

	if (delta <= YHL_HL_U80M_TOLERANCY)
	{
		/* Voltage is OK */
		if (HL_U80mLockCounter < YHL_HL_U80M_LOCK_DURATION)
		{
			HL_U80mLockCounter ++;
		}
		else
		{
			/* Required voltage is achieved */
			HL_IsU80mFeedbackActive = false;

			if (HL_U80mLockCallback != NULL)
			{
				HL_U80mLockCallback();
			}
		}

		return;
	}

	HL_U80mLockCounter = 0; /* Miss */

	int16_t newRegulatorCode = HAL_GetU80mRegulatorCode();

	float codeDelta = delta * YHL_HL_U80M_PROPORTIONAL_REGULATION_FACTOR;
	if (codeDelta < 1)
	{
		codeDelta = 1;
	}

	if (currentVoltage < HL_U80mTarget)
	{
		/* Need to increase voltage -> decrease code */
		newRegulatorCode -= codeDelta;
	}
	else
	{
		newRegulatorCode += codeDelta;
	}

	if (newRegulatorCode < 0)
	{
		newRegulatorCode = 0;
	}
	else if (newRegulatorCode > 0xFF)
	{
		newRegulatorCode = 0xFF;
	}

	HAL_SetU80mRegulatorCode((uint8_t)newRegulatorCode);
}

void HL_SetU80mLockCallback(void (*callback)(void))
{
	HL_U80mLockCallback = callback;
}

void HL_PrepareFoxFor80mCycle(void)
{
	if (FoxState.Frequency.Is144MHz)
	{
		L2HAL_Error(Generic);
	}

	/* Disabling manipulator (just as precaution) */
	HAL_SwitchManipulator(false);

	/* Activating 3.5MHz tract */
	HAL_Activate80M(true);

	HAL_Delay(HAL_REGULATORS_SPIN_UP_TIME);

	/* Setting up frequency synthesizer */
	HAL_SetupSynthesizer(FoxState.Frequency.FrequencyHz);

	/* Converting required power into voltage and setting U80m */
	HL_SetupU80m(HAL_GetU80mFromPower(FoxState.Power));
}

void HL_UnPrepareFoxFrom80mCycle(void)
{
	/* Disabling manipulator (just as precaution) */
	HAL_SwitchManipulator(false);

	HAL_PutSynthesizerToSleep();

	HAL_Activate80M(false);
}

void HL_PrepareFoxFor2mCycle(void)
{
	if (!FoxState.Frequency.Is144MHz)
	{
		L2HAL_Error(Generic);
	}

	/* Disabling manipulator (just as precaution) */
	HAL_SwitchManipulator(false);

	/* Activating 144MHz tract */
	HAL_Activate2M(true);

	HAL_Delay(HAL_REGULATORS_SPIN_UP_TIME);

	/* Setting up frequency synthesizer */
	HAL_SetupSynthesizer(FoxState.Frequency.FrequencyHz / YHL_HL_2M_FREQUENCY_MULTIPLICATION_FACTOR);
}

void HL_UnPrepareFoxFrom2mCycle(void)
{
	/* Disabling manipulator (just as precaution) */
	HAL_SwitchManipulator(false);

	HAL_PutSynthesizerToSleep();

	HAL_Activate2M(false);
}

void HL_PrepareFoxForCycle(void)
{
	if (HL_CheckIsFoxPrepared())
	{
		L2HAL_Error(Generic);
	}

	if (FoxState.Frequency.Is144MHz)
	{
		HL_UnPrepareFoxFrom80mCycle();
		HL_PrepareFoxFor2mCycle();
	}
	else
	{
		HL_UnPrepareFoxFrom2mCycle();
		HL_PrepareFoxFor80mCycle();
	}

	HL_IsFoxPrepared = true;
}

void HL_UnPrepareFoxFromCycle(void)
{
	if (!HL_CheckIsFoxPrepared())
	{
		L2HAL_Error(Generic);
	}

	if (FoxState.Frequency.Is144MHz)
	{
		HL_UnPrepareFoxFrom2mCycle();
	}
	else
	{
		HL_UnPrepareFoxFrom80mCycle();
	}

	HL_IsFoxPrepared = false;
}

bool HL_CheckIsFoxPrepared(void)
{
	return HL_IsFoxPrepared;
}
