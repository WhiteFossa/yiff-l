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
	FoxState.GlobalState.CurrentState = GfsStandby;
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
		case GfsPreparation:
			GSM_PrepareFox();
			break;

		case GfsReady:
			break;

		case GfsBeforeStart:
			GSM_MoveToBeforeStart();
			break;

		case GfsBeforeFinish:
			GSM_StartFox();
			break;

		case GfsStandby:
			GSM_StopFox();
			break;

		default:
			L2HAL_Error(Generic);
			break;
	}
}

void GSM_MoveToBeforeStart(void)
{
	FoxState.GlobalState.StateChangeTime = FoxState.GlobalState.StartTime;
	FoxState.GlobalState.CurrentState = GfsBeforeStart;

	CSM_Stop();
}

void GSM_PrepareFox(void)
{
	if (FoxState.GlobalState.CurrentState != GfsReady)
	{
		/* Going to prepare fox*/
		if (!HL_CheckIsFoxPrepared()) /* Because of possible time shift we can occur here from any state */
		{
			HL_PrepareFoxForCycle();
		}

		FoxState.GlobalState.CurrentState = GfsReady;
	}
}

void GSM_StartFox(void)
{
	/* Starting */
	FoxState.GlobalState.CurrentState = GfsBeforeFinish;
	FoxState.GlobalState.StateChangeTime = FoxState.GlobalState.EndTime;

	/* Starting cycle */
	CSM_Start();
}

void GSM_StopFox(void)
{
	/* Finishing */
	FoxState.GlobalState.CurrentState = GfsStandby;

	/* Stopping cycle */
	CSM_Stop();
}

GlobalFoxStateEnum GSM_GetExpectedState(void)
{
	Time preparationStartTime = SubtractSeconds(FoxState.GlobalState.StartTime, YHL_HL_FOX_PREPARATION_TIME);

	int8_t compResPreparationStartTime = CompareTimes(FoxState.CurrentTime, preparationStartTime);
	int8_t compResStartTime = CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.StartTime);
	int8_t compResEndTime = CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.EndTime);


	if (TIME1_LESS == compResPreparationStartTime)
	{
		/* Current time < Start time -> Before start */
		return GfsBeforeStart;
	}

	if (TIME1_LESS != compResPreparationStartTime && TIME1_LESS == compResStartTime)
	{
		return GfsPreparation;
	}

	if (TIME1_LESS == compResEndTime)
	{
		/* Start time <= Current time < End time */
		return GfsBeforeFinish;
	}

	return GfsStandby;
}

