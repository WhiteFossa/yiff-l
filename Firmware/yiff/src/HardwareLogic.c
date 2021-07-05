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

	if (fabs(currentVoltage - HL_U80mTarget) <= YHL_HL_U80M_TOLERANCY)
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
		}

		return;
	}

	HL_U80mLockCounter = 0; /* Miss */

	int16_t newRegulatorCode = HAL_GetU80mRegulatorCode();

	if (currentVoltage < HL_U80mTarget)
	{
		/* Need to increase voltage -> decrease code */
		newRegulatorCode --;
	}
	else
	{
		newRegulatorCode ++;
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
