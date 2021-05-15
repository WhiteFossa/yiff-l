/*
 * CycleStateMachine.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

void CSM_Start(void)
{
	FoxState.CycleState.CycleState = Tx;
	FoxState.CycleState.StateChangeTime = AddTimes(FoxState.CurrentTime, FoxState.Cycle.TxTime);

	CSM_CalculateEndingToneStartTime();
	FoxState.CycleState.IsEndingTone = false;
	ProcessManipulatorFoxStateChange();

	MorsePlayerPlay(MorseGetFoxSequence());
}

void CSM_Stop(void)
{
	FoxState.CycleState.CycleState = Pause;
	FoxState.CycleState.IsEndingTone = false;
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

	int8_t comparisonResult = CompareTimes(FoxState.CurrentTime, FoxState.CycleState.StateChangeTime);

	switch(FoxState.CycleState.CycleState)
	{
		case Tx:
			if (TIMES_EQUAL == comparisonResult)
			{
				FoxState.CycleState.CycleState = Pause;
				FoxState.CycleState.StateChangeTime = AddTimes(FoxState.CurrentTime, FoxState.Cycle.PauseTime);
				FoxState.CycleState.IsEndingTone = false;
				ProcessManipulatorFoxStateChange();

				/* Stopping transmission */
				MorsePlayerStop();
			}
			else
			{
				if (TIME2_LESS == CompareTimes(FoxState.CurrentTime, EndingToneStartTime))
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
				FoxState.CycleState.StateChangeTime = AddTimes(FoxState.CurrentTime, FoxState.Cycle.TxTime);

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
