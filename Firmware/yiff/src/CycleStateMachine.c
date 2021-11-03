/*
 * CycleStateMachine.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <CycleStateMachine.h>

void CSM_Init(void)
{
	CSM_Stop();
}

void CSM_Start(void)
{
	CSM_Cycle_StartTx(0);
}

void CSM_Stop(void)
{
	CSM_Cycle_StartPause(0);
}

void CSM_Tick(void)
{
	if (FoxState.GlobalState.CurrentState != GfsBeforeFinish)
	{
		return;
	}

	if (FoxState.Cycle.IsContinuous)
	{
		return;
	}

	int16_t timeSinceCycleStart = CSM_GetCycleTime();

	if (timeSinceCycleStart < 0)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_NegativeTimeSinceCycleStart);
	}

	CycleStateEnum newState = CSM_GetStateByCycleTime((uint32_t)timeSinceCycleStart);

	if (newState != FoxState.CycleState.CycleState)
	{
		switch(newState)
		{
			case CsTx:
				CSM_Cycle_StartTx((uint16_t)timeSinceCycleStart);
				break;

			case CsEndingTone:
				CSM_Cycle_StartEndingTone((uint16_t)timeSinceCycleStart);
				break;

			case CsPause:
				CSM_Cycle_StartPause((uint16_t)timeSinceCycleStart);
				break;

			case CsPreparation:
				CSM_Cycle_PrepareFox();
				break;

			case CsReady:
				/* Nothing to do */
				break;

			default:
				SelfDiagnostics_HaltOnFailure(YhlFailureCause_UnknownCycleStateInCSMTick);
		}
	}

	/* After state switch */
	CSM_RecalculateStateChangeTime((uint16_t)timeSinceCycleStart);
}

int16_t CSM_GetCycleTime(void)
{
	if (FoxState.CurrentTime < FoxState.GlobalState.StartTime)
	{
		return -1; /* Fox not in cycle */
	}

	uint32_t secondsSinceFoxStart = FoxState.CurrentTime - FoxState.GlobalState.StartTime;

	uint32_t cycleLength = FoxState.Cycle.TxTime + FoxState.Cycle.PauseTime;

	return (int16_t)(secondsSinceFoxStart % cycleLength);
}

CycleStateEnum CSM_GetStateByCycleTime(uint32_t timeSinceCycleStart)
{
	uint32_t endingToneStartTime = FoxState.Cycle.TxTime - FoxState.EndingToneLength;
	uint32_t pauseStartTime = FoxState.Cycle.TxTime;
	uint32_t preparationStartTime = FoxState.Cycle.TxTime + FoxState.Cycle.PauseTime - YHL_HL_FOX_PREPARATION_TIME;

	if (timeSinceCycleStart < endingToneStartTime)
	{
		/* Current Time < Ending Tone Start Time -> TX*/
		return CsTx;
	}

	if ((endingToneStartTime <= timeSinceCycleStart) && (timeSinceCycleStart < pauseStartTime))
	{
		/* Ending Tone Start Time <= Current Time < Pause Start Time -> Ending tone */
		return CsEndingTone;
	}

	if (preparationStartTime <= timeSinceCycleStart)
	{
		/* Preparation Start Time <= Current Time -> Preparation */
		return CsPreparation;
	}

	return CsPause;
}

void CSM_Cycle_PrepareFox(void)
{
	if (FoxState.CycleState.CycleState != CsReady)
	{
		if (!HL_CheckIsFoxPrepared()) /* Because of possible time shift we can occur here from any state */
		{
			HL_PrepareFoxForCycle();
		}

		FoxState.CycleState.CycleState = CsReady;
	}
}

void CSM_Cycle_StartTx(uint16_t timeSinceCycleBegin)
{
	if (!HL_CheckIsFoxPrepared())
	{
		/* Looks like time adjustment happened and now we are in cycle, but fox is not ready.
		Activating immediately */
		HL_PrepareFoxForCycle();
	}

	FoxState.CycleState.CycleState = CsTx;
	FoxState.CycleState.IsEndingTone = false;
	HL_ProcessManipulatorFoxStateChange();
	CSM_RecalculateStateChangeTime(timeSinceCycleBegin);

	/* Starting transmission */
	MorsePlayerPlay(MorseGetFoxSequence());
}

void CSM_Cycle_StartEndingTone(uint16_t timeSinceCycleBegin)
{
	FoxState.CycleState.CycleState = CsEndingTone;
	FoxState.CycleState.IsEndingTone = true;
	CSM_RecalculateStateChangeTime(timeSinceCycleBegin);
	HL_ProcessManipulatorFoxStateChange();
}

void CSM_Cycle_StartPause(uint16_t timeSinceCycleBegin)
{
	FoxState.CycleState.CycleState = CsPause;
	FoxState.CycleState.IsEndingTone = false;
	CSM_RecalculateStateChangeTime(timeSinceCycleBegin);
	HL_ProcessManipulatorFoxStateChange();

	/* Stopping transmission */
	MorsePlayerStop();

	/* Un-preparing fox */
	if (HL_CheckIsFoxPrepared())
	{
		HL_UnPrepareFoxFromCycle();
	}
}

void CSM_RecalculateStateChangeTime(uint16_t timeSinceCycleBegin)
{
	uint32_t remainder = 0;
	switch(FoxState.CycleState.CycleState)
	{
		case CsTx:
		case CsEndingTone:
			remainder = FoxState.Cycle.TxTime - timeSinceCycleBegin;
			break;

		case CsPause:
		case CsPreparation:
		case CsReady:
			remainder = FoxState.Cycle.TxTime + FoxState.Cycle.PauseTime - timeSinceCycleBegin;
			break;

		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_UnknownCycleStateInCSMRecalculateStateChangeTime);
	}

	FoxState.CycleState.StateChangeTime = FoxState.CurrentTime + remainder;
}
