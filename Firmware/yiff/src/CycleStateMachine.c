/*
 * CycleStateMachine.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

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
	if (FoxState.GlobalState.CurrentState != BeforeFinish)
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
		L2HAL_Error(Generic);
	}

	CycleStateEnum newState = CSM_GetStateByCycleTime(timeSinceCycleStart);

	if (newState != FoxState.CycleState.CycleState)
	{
		switch(newState)
		{
			case Tx:
				CSM_Cycle_StartTx((uint16_t)timeSinceCycleStart);
				break;

			case EndingTone:
				CSM_Cycle_StartEndingTone((uint16_t)timeSinceCycleStart);
				break;

			case Pause:
				CSM_Cycle_StartPause((uint16_t)timeSinceCycleStart);
				break;

			case Preparation:

				if (FoxState.CycleState.CycleState != Ready)
				{
					if (!HL_CheckIsFoxPrepared()) /* Because of possible time shift we can occur here from any state */
					{
						HL_PrepareFoxForCycle();
					}

					FoxState.CycleState.CycleState = Ready;
				}

				break;
		}
	}

	/* After state switch */
	CSM_RecalculateStateChangeTime(timeSinceCycleStart);
}

int16_t CSM_GetCycleTime(void)
{
	if (TIME1_LESS == CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.StartTime))
	{
		return -1; /* Fox not in cycle */
	}

	uint32_t secondsSinceFoxStart = SecondsSinceDayBegin(SubtractTimes(FoxState.CurrentTime, FoxState.GlobalState.StartTime));

	uint32_t cycleLength = SecondsSinceDayBegin(AddTimes(FoxState.Cycle.TxTime, FoxState.Cycle.PauseTime));

	return secondsSinceFoxStart % cycleLength;
}

CycleStateEnum CSM_GetStateByCycleTime(int16_t cycleTime)
{
	Time timeSinceCycleStart = TimeSinceDayBegin(cycleTime);
	Time endingToneStartTime = SubtractSeconds(FoxState.Cycle.TxTime, FoxState.EndingToneLength);
	Time pauseStartTime = FoxState.Cycle.TxTime;
	Time preparationStartTime = SubtractSeconds(AddTimes(FoxState.Cycle.TxTime, FoxState.Cycle.PauseTime), YHL_HL_FOX_PREPARATION_TIME);

	if (TIME1_LESS == CompareTimes(timeSinceCycleStart, endingToneStartTime))
	{
		/* Current Time < Ending Tone Start Time -> TX*/
		return Tx;
	}

	if ((TIME2_LESS != CompareTimes(endingToneStartTime, timeSinceCycleStart))
			&& (TIME1_LESS == CompareTimes(timeSinceCycleStart, pauseStartTime)))
	{
		/* Ending Tone Start Time <= Current Time < Pause Start Time -> Ending tone */
		return EndingTone;
	}

	if (TIME2_LESS != CompareTimes(preparationStartTime, timeSinceCycleStart))
	{
		/* Preparation Start Time <= Current Time -> Preparation */
		return Preparation;
	}

	return Pause;
}

void CSM_Cycle_StartTx(uint16_t timeSinceCycleBegin)
{
	if (!HL_CheckIsFoxPrepared())
	{
		/* Looks like time adjustment happened and now we are in cycle, but fox is not ready.
		Activating immediately */
		HL_PrepareFoxForCycle();
	}

	FoxState.CycleState.CycleState = Tx;
	FoxState.CycleState.IsEndingTone = false;
	ProcessManipulatorFoxStateChange();
	CSM_RecalculateStateChangeTime(timeSinceCycleBegin);

	/* Starting transmission */
	MorsePlayerPlay(MorseGetFoxSequence());
}

void CSM_Cycle_StartEndingTone(uint16_t timeSinceCycleBegin)
{
	FoxState.CycleState.CycleState = EndingTone;
	FoxState.CycleState.IsEndingTone = true;
	CSM_RecalculateStateChangeTime(timeSinceCycleBegin);
	ProcessManipulatorFoxStateChange();
}

void CSM_Cycle_StartPause(uint16_t timeSinceCycleBegin)
{
	FoxState.CycleState.CycleState = Pause;
	FoxState.CycleState.IsEndingTone = false;
	CSM_RecalculateStateChangeTime(timeSinceCycleBegin);
	ProcessManipulatorFoxStateChange();

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
	Time remainder;
	switch(FoxState.CycleState.CycleState)
	{
		case Tx:
		case EndingTone:
			remainder = SubtractSeconds(FoxState.Cycle.TxTime, timeSinceCycleBegin);
			break;

		case Pause:
		case Preparation:
		case Ready:
			remainder = SubtractSeconds(AddTimes(FoxState.Cycle.TxTime, FoxState.Cycle.PauseTime), timeSinceCycleBegin);
			break;
	}

	FoxState.CycleState.StateChangeTime = AddTimes(FoxState.CurrentTime, remainder);
}
