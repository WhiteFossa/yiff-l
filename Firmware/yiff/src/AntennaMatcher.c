/*
 * AntennaMatcher.c
 *
 *  Created on: Dec 27, 2021
 *      Author: fossa
 */

#include <AntennaMatcher.h>
#include <AntennaMatcherPrivate.h>

void AMSM_Init(void)
{
	AMSM_State = AMSM_Inactive;
	AMSM_Timer = 0;

	FoxState.AntennaMatching.Status = AntennaMatching_NeverInitiated;
	FoxState.AntennaMatching.IsNewForApp = false;

	AMSM_ResetValues();

	L2HAL_SysTick_RegisterHandler(&AMSM_SysTickHandler);
}

void AMSM_SysTickHandler(void)
{
	if (AMSM_Timer > 0)
	{
		AMSM_Timer --;
	}

	if (AMSM_State != AMSM_Inactive)
	{
		Sleepmodes_PreventSleep();
	}
}

void AMSM_MoveToNextState(void)
{
	switch (AMSM_State)
	{
		case AMSM_Inactive:
			return;

		case AMSM_WaitingForU80mLock:
			if (!HL_CheckIfU80mLocked())
			{
				return; /* U80m is not locked yet */
			}

			/* First position */
			AMSM_SupressCarrier(true);

			HAL_SetAntennaMatchingValue(0);

			AMSM_Timer = AMSM_ON_COMMUTATION_DELAY;

			AMSM_State = AMSM_WaitingForOnCommutation;

			return;

		case AMSM_WaitingForOnCommutation:
			if (AMSM_Timer > 0)
			{
				return;
			}

			AMSM_Timer = AMSM_ON_MEASUREMENT_DELAY;
			AMSM_SupressCarrier(false);
			AMSM_ForceCarrier(true);

			AMSM_State = AMSM_WaitingForMeasurement;

			return;

		case AMSM_WaitingForMeasurement:
			if (AMSM_Timer > 0)
			{
				return;
			}

			float measuredSignalLevel = HAL_GetUAntVolts();
			FoxState.AntennaMatching.MatchingVoltages[FoxState.AntennaMatching.CurrentPosition] = measuredSignalLevel;

			if (measuredSignalLevel > FoxState.AntennaMatching.BestMatchVoltage)
			{
				FoxState.AntennaMatching.BestMatchVoltage = measuredSignalLevel;
				FoxState.AntennaMatching.BestMatchPosition = FoxState.AntennaMatching.CurrentPosition;
			}

			DrawMatchingDisplay(FoxState);

			AMSM_Timer = AMSM_OFF_COMMUTATION_DELAY;
			AMSM_SupressCarrier(true);
			AMSM_ForceCarrier(false);

			AMSM_State = AMSM_WaitingForOffCommutation;

			return;

		case AMSM_WaitingForOffCommutation:
			if (AMSM_Timer > 0)
			{
				return;
			}

			/* Is last measurement? */
			if (FoxState.AntennaMatching.CurrentPosition == HAL_AM_MAX_VALUE)
			{
				/* Matching completed */
				AMSM_State = AMSM_Inactive;

				AMSM_SupressCarrier(false);
				AMSM_ForceCarrier(false);

				HL_UnPrepareFoxFromCycle();

				FoxState.CurrentDisplay = StatusDisplay;
				FoxState.AntennaMatching.Status = AntennaMatching_Completed;

				FMGL_API_ClearScreen(&fmglContext);

				if (AMSM_IsArmFoxAfterMatching)
				{
					GSM_Arm();
					EmitFoxArmedEvent();
				}

				return;
			}

			FoxState.AntennaMatching.CurrentPosition ++;

			AMSM_SupressCarrier(true);

			HAL_SetAntennaMatchingValue(FoxState.AntennaMatching.CurrentPosition);

			AMSM_Timer = AMSM_ON_MEASUREMENT_DELAY;

			AMSM_State = AMSM_WaitingForOnCommutation;

			return;

		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongAMSMStateInAMSMMoveToNextState);
	}
}

void AMSM_StartMatching(bool isArmFoxAfterMatching)
{
	if (FoxState.Frequency.Is144MHz)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AttemptToSetup80mAntennaWhile2m);
	}

	if (AMSM_State != AMSM_Inactive)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AntennaMatchingAlreadyInProgress);
	}

	if (HL_CheckIsFoxPrepared())
	{
		/* Already prepared somehow, bug is somewhere */
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_FoxIsPreparedOnHLPrepareAndMatch80m);
	}

	AMSM_IsArmFoxAfterMatching = isArmFoxAfterMatching;

	FoxState.AntennaMatching.IsNewForApp = true;
	FoxState.AntennaMatching.Status = AntennaMatching_InProgress;

	AMSM_ResetValues();

	FoxState.CurrentDisplay = AntennaMatchingDisplay;
	DrawMatchingDisplay(FoxState);

	HL_PrepareFoxForCycle(); /* Turning on voltages, awaiting for voltages lock */

	AMSM_State = AMSM_WaitingForU80mLock;
}


void AMSM_SupressCarrier(bool isSupress)
{
	FoxState.ForceCarrierOff = isSupress;
	HL_ProcessManipulatorFoxStateChange();
}


void AMSM_ForceCarrier(bool isForce)
{
	FoxState.ForceCarrierOn = isForce;
	HL_ProcessManipulatorFoxStateChange();
}


void AMSM_ResetValues(void)
{
	FoxState.AntennaMatching.CurrentPosition = 0;
	FoxState.AntennaMatching.BestMatchPosition = 0;
	FoxState.AntennaMatching.BestMatchVoltage = 0;
	memset(FoxState.AntennaMatching.MatchingVoltages, 0x00, YHL_MATCHING_LEVELS_COUNT * sizeof(float));
}
