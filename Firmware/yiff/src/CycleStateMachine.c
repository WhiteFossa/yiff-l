/*
 * CycleStateMachine.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

void CSM_Start(void)
{
	Time currentTime = ToTime(CurrentTime);

	FoxState.CycleState.CycleState = Tx;
	FoxState.CycleState.StateChangeTime = AddTimes(currentTime, FoxState.Cycle.TxTime);

	CSM_CalculateEndingToneStartTime();
	FoxState.CycleState.IsEndingTone = false;
	ProcessManipulatorFoxStateChange();

	MorsePlayerPlay(MorseGetFoxSequence());
}

void CSM_Stop(void)
{
	FoxState.CycleState.CycleState = Pause;
	MorsePlayerStop();
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

	Time currentTime = ToTime(CurrentTime);
	int8_t comparisonResult = CompareTimes(currentTime, FoxState.CycleState.StateChangeTime);

	switch(FoxState.CycleState.CycleState)
	{
		case Tx:
			if (TIMES_EQUAL == comparisonResult)
			{
				FoxState.CycleState.CycleState = Pause;
				FoxState.CycleState.StateChangeTime = AddTimes(currentTime, FoxState.Cycle.PauseTime);
				FoxState.CycleState.IsEndingTone = false;
				ProcessManipulatorFoxStateChange();

				/* Stopping transmission */
				MorsePlayerStop();
			}
			else
			{
				if (TIME2_LESS == CompareTimes(currentTime, EndingToneStartTime))
				{
					/* Ending tone on */
					FoxState.CycleState.IsEndingTone = true;
					ProcessManipulatorFoxStateChange();
				}
			}
			break;

		case Pause:
			if (TIMES_EQUAL == comparisonResult)
			{
				FoxState.CycleState.CycleState = Tx;
				FoxState.CycleState.StateChangeTime = AddTimes(currentTime, FoxState.Cycle.TxTime);

				CSM_CalculateEndingToneStartTime();
				FoxState.CycleState.IsEndingTone = false;
				ProcessManipulatorFoxStateChange();

				/* Starting transmission */
				MorsePlayerPlay(MorseGetFoxSequence());
			}
			break;

		default:
			L2HAL_Error(Generic);
	}

}

void CSM_CalculateEndingToneStartTime(void)
{
	if (FoxState.Cycle.IsContinuous)
	{
		return;
	}

	if (FoxState.CycleState.CycleState != Tx)
	{
		return;
	}

	EndingToneStartTime = SubstractSeconds(FoxState.CycleState.StateChangeTime, FoxState.EndingToneLength);
}
