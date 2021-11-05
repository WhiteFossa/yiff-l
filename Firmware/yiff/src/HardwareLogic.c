/*
 * HardwareLogic.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <HardwareLogic.h>
#include <HardwareLogicPrivate.h>

void HL_Init()
{
	HAL_IntiHardware();

	HL_IsU80mFeedbackActive = false;
	HL_U80mLockCounter = 0;
	HL_U80mLockCallback = NULL;
	HL_IsFoxPrepared = false;
	HL_OptimalAntennaMatching = 0;
	HL_IsU80mLocked = false;

	HAL_SetU80mMeasuredCallback(HL_U80mMeasurementCallback);
	HL_SetU80mLockCallback(HL_OnU80mLock);
}

void HL_ProcessManipulatorFoxStateChange(void)
{
	if ((FoxState.IsMorseTx || FoxState.CycleState.IsEndingTone || FoxState.ForceCarrierOn) && (!FoxState.ForceCarrierOff))
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
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongTargetU80m);
	}

	HL_U80mTarget = targetVoltage;

	/* Setting lowest possible voltage, then activating feedback */
	HAL_SetU80mRegulatorCode(HAL_U80M_LOWEST_VOLTAGE_CODE);

	HL_U80mLockCounter = 0;
	HL_IsU80mFeedbackActive = true;
	HL_IsU80mLocked = false;
}

void HL_U80mMeasurementCallback(void)
{
	if (!HL_IsU80mFeedbackActive)
	{
		return;
	}

	float currentVoltage = HAL_GetU80mVolts();

	float delta = (float)fabs(currentVoltage - HL_U80mTarget);

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
		newRegulatorCode -= floor(codeDelta + 0.5f);
	}
	else
	{
		newRegulatorCode += floor(codeDelta + 0.5f);
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
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AttemptToPrepareFoxFor80mWhile2m);
	}

	/* Disabling manipulator (just as precaution) */
	HAL_SwitchManipulator(false);

	/* Activating 3.5MHz tract */
	HAL_Activate80M(true);

	HAL_Delay(HAL_REGULATORS_SPIN_UP_TIME);

	/* Setting up frequency synthesizer */
	HAL_SetupSynthesizer((float)FoxState.Frequency.FrequencyHz);

	/* Converting required power into voltage and setting U80m */
	HL_SetupU80m(HAL_GetU80mFromPower(FoxState.Power));

	/* Switching antenna matching */
	HAL_SetAntennaMatchingValue(HL_OptimalAntennaMatching);
}

void HL_UnPrepareFoxFrom80mCycle(void)
{
	/* Disabling manipulator (just as precaution) */
	HAL_SwitchManipulator(false);

	HAL_PutSynthesizerToSleep();

	HL_IsU80mLocked = false;
	HAL_Activate80M(false);
}

void HL_PrepareFoxFor2mCycle(void)
{
	if (!FoxState.Frequency.Is144MHz)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AttemptToPrepareFoxFor2mWhile80m);
	}

	/* Disabling manipulator (just as precaution) */
	HAL_SwitchManipulator(false);

	/* Activating 144MHz tract */
	HAL_Activate2M(true);

	HAL_Delay(HAL_REGULATORS_SPIN_UP_TIME);

	/* Setting up frequency synthesizer */
	HAL_SetupSynthesizer((float)FoxState.Frequency.FrequencyHz / YHL_HL_2M_FREQUENCY_MULTIPLICATION_FACTOR);
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
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_FoxAlreadyPreparedForCycleInHLPrepareFoxForCycle);
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
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_FoxIsNotPreparedForCycleInHLUnPrepareFoxFromCycle);
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

void HL_Setup80mAntenna(void)
{
	if (FoxState.Frequency.Is144MHz)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AttemptToSetup80mAntennaWhile2m);
	}

	if (!HL_CheckIsFoxPrepared())
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AttemptToSetup80mAntennaWhileFoxIsNotPrepared);
	}

	FoxState.ForceCarrierOn = true;

	HL_ProcessManipulatorFoxStateChange();

	HL_OptimalAntennaMatching = 0;
	float signalLevel = 0;

	for (uint8_t amValue = 0; amValue <= HAL_AM_MAX_VALUE; amValue ++)
	{
		FoxState.MatchingDisplayData.MatchingStep = amValue;
		DrawMatchingDisplay(FoxState);

		HAL_SetAntennaMatchingValue(amValue);
		HAL_Delay(YHL_HL_FOX_WAIT_FOR_UANT_DELAY);

		float measuredSignalLevel = HAL_GetUAntADC();
		FoxState.MatchingDisplayData.MatchingLevels[amValue] = measuredSignalLevel;

		if (measuredSignalLevel > signalLevel)
		{
			signalLevel = measuredSignalLevel;
			HL_OptimalAntennaMatching = amValue;
		}

		EmitAntennaMatchingMeasurementEvent(amValue, measuredSignalLevel);
	}

	FoxState.ForceCarrierOn = false;
	HL_ProcessManipulatorFoxStateChange();
}

void HL_OnU80mLock(void)
{
	if (HL_IsU80mLocked)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_U80mAlreadyLocked);
	}

	HL_IsU80mLocked = true;
}

void HL_PrepareAndMatch80m(void)
{
	if (FoxState.Frequency.Is144MHz)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_2mOnHLPrepareAndMatch80m);
	}

	if (HL_CheckIsFoxPrepared())
	{
		/* Already prepared somehow, bug is somewhere */
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_FoxIsPreparedOnHLPrepareAndMatch80m);
	}

	HL_PrepareFoxForCycle();

	while(!HL_IsU80mLocked)
	{
		HAL_Delay(100); /* No need to put it into constants */
	}

	HL_Setup80mAntenna();
}

void HL_RenameBluetoothDevice(char* newName)
{
	HAL_SwitchBluetoothPower(false);
	HAL_Delay(YHL_HL_BLUETOOTH_POWERCYCLE_LENGTH);
	HAL_SwitchBluetoothPower(true);
	HAL_Delay(YHL_HL_BLUETOOTH_BOOT_TIME);

	/* Reconnecting to bluetooth module */
	HC06_Context = L2HAL_HC06_AttachToDevice(&UART_Handle);
	if (!HC06_Context.IsFound)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_FailedToReconnectToBluetoothAfterRename);
	}

	L2HAL_HC06_SetName(&HC06_Context, newName);
	L2HAL_HC06_SetPIN(&HC06_Context, YHL_HL_BLUETOOTH_PIN);
}

void HL_TurnDisplayOn(void)
{
	/* Detecting display (might require some attempts) */
	int8_t attemptsCounter = YHL_HL_INITIALIZE_DISPLAY_ATTEMPS_COUNT;
	while (attemptsCounter >= 0)
	{
		/* De-initializing I2C bus and pulling pins down to remove parasitic power */
		if (HAL_GetDisplayBusInitializationStatus())
		{
			HAL_DeInitializeDisplayBus();
		}
		HAL_SupressDisplayParasiticPower();

		/* Powering off */
		HAL_SwitchDisplayPower(false);

		HAL_Delay(YHL_HL_DISPLAY_POWERDOWN_TIME); /* Time to power down*/

		/* Powering on */
		HAL_SwitchDisplayPower(true);
		HAL_Delay(YHL_HL_DISPLAY_REGULATOR_SPINUP_TIME); /* Time to spin regulator up */

		HAL_InitializeDisplayBus();

		L2HAL_SSD1327_Context = L2HAL_SSD1327_DetectDisplay(&I2C_Display);
		if (L2HAL_SSD1327_Context.IsFound)
		{
			/* Found */
			break;
		}

		attemptsCounter --;
	}

	if (!L2HAL_SSD1327_Context.IsFound)
	{
		/* Display not found */
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_DisplayNotFound);
	}

	L2HAL_SSD1327_InitDisplay(&L2HAL_SSD1327_Context);
	FoxState.SupressDrawing = false;
}

void HL_TurnDisplayOff(void)
{
	FoxState.SupressDrawing = true;

	HAL_SwitchDisplayPower(false);
}
