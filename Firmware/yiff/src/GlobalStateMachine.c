/*
 * GlobalStateMachine.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

void GSM_Init(void)
{
	GSM_Disarm();
}

void GSM_Disarm(void)
{
	FoxState.GlobalState.IsArmed = false;
	FoxState.GlobalState.CurrentState = Standby;
}

void GSM_Arm(void)
{
	if (CompareTimes(FoxState.GlobalState.StartTime, FoxState.GlobalState.EndTime) != TIME1_LESS)
	{
		L2HAL_Error(Generic);
	}

	FoxState.GlobalState.IsArmed = true;
	GSM_MoveToBeforeStart();
}

void GSM_Tick(void)
{
	if (!FoxState.GlobalState.IsArmed)
	{
		return;
	}

	GlobalFoxStateEnum newGlobalState = GSM_GetExpectedState();

	if (newGlobalState == FoxState.GlobalState.CurrentState)
	{
		return;
	}

	switch(newGlobalState)
	{
		case BeforeStart:
			GSM_MoveToBeforeStart();
			break;

		case BeforeFinish:
			GSM_StartFox();
			break;

		case Standby:
			GSM_StopFox();
			break;
	}
}

void GSM_MoveToBeforeStart(void)
{
	FoxState.GlobalState.StateChangeTime = FoxState.GlobalState.StartTime;
	FoxState.GlobalState.CurrentState = BeforeStart;

	CSM_Stop();
}

void GSM_StartFox(void)
{
	/* Starting */
	FoxState.GlobalState.CurrentState = BeforeFinish;
	FoxState.GlobalState.StateChangeTime = FoxState.GlobalState.EndTime;

	/* Starting cycle */
	CSM_Start();
}

void GSM_StopFox(void)
{
	/* Finishing */
	FoxState.GlobalState.CurrentState = Standby;

	/* Stopping cycle */
	CSM_Stop();
}

GlobalFoxStateEnum GSM_GetExpectedState(void)
{
	int8_t compResStartTime = CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.StartTime);
	int8_t compResEndTime = CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.EndTime);

	if (TIME1_LESS == compResStartTime)
	{
		/* Current time < Start time -> Before start */
		return BeforeStart;
	}

	if (TIME1_LESS == compResEndTime)
	{
		/* Start time <= Current time < End time */
		return BeforeFinish;
	}

	return Standby;
}

