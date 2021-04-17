/*
 * CycleStateMachine.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

void CSM_Start()
{
	Time currentTime = ToTime(CurrentTime);

	FoxState.CycleState.CycleState = Tx;
	FoxState.CycleState.StateChangeTime = AddTimes(currentTime, FoxState.Cycle.TxTime);
}

void CSM_Tick()
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
			if (comparisonResult == 0)
			{
				FoxState.CycleState.CycleState = Pause;
				FoxState.CycleState.StateChangeTime = AddTimes(currentTime, FoxState.Cycle.PauseTime);
			}
			break;

		case Pause:
			if (comparisonResult == 0)
			{
				FoxState.CycleState.CycleState = Tx;
				FoxState.CycleState.StateChangeTime = AddTimes(currentTime, FoxState.Cycle.TxTime);
			}
			break;

		default:
			L2HAL_Error(Generic);
	}
}
