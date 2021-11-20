/*
 * GlobalStateMachine.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <GlobalStateMachine.h>

void GSM_Init(void)
{
	GSM_Disarm();
}

void GSM_Disarm(void)
{
	FoxState.GlobalState.IsArmed = false;
	GSM_StopFox();
}

void GSM_Arm(void)
{
	if (FoxState.GlobalState.StartDateTime >= FoxState.GlobalState.EndDateTime)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_StartTimeGreaterOrEqualFinishTimeInGSMArm);
	}

	/* Moving start and end times to today */
	Time currentTime = TimestampToTime(FoxState.CurrentTime);

	Time startTime = TimestampToTime(FoxState.GlobalState.StartDateTime);
	Time endTime = TimestampToTime(FoxState.GlobalState.EndDateTime);

	startTime.Days = currentTime.Days;
	endTime.Days = currentTime.Days;

	FoxState.GlobalState.StartDateTime = TimeToTimestamp(startTime);
	FoxState.GlobalState.EndDateTime = TimeToTimestamp(endTime);

	FoxState.GlobalState.IsArmed = true;
	GSM_MoveToBeforeStart();
}

void GSM_Tick(void)
{
	if (!FoxState.GlobalState.IsArmed)
	{
		return;
	}

	volatile GlobalFoxStateEnum newGlobalState = GSM_GetExpectedState();

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
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_UnknownGlobalStateInGSMTick);
			break;
	}
}

void GSM_MoveToBeforeStart(void)
{
	FoxState.GlobalState.StateChangeTime = FoxState.GlobalState.StartDateTime;
	FoxState.GlobalState.CurrentState = GfsBeforeStart;

	CSM_Stop();
}

void GSM_PrepareFox(void)
{
	if (FoxState.GlobalState.CurrentState != GfsReady)
	{
		/* Slow preparation (with antenna matching) */
		Main_PrepareAndMatchAntenna();

		FoxState.GlobalState.CurrentState = GfsReady;
	}
}

void GSM_StartFox(void)
{
	/* Starting */
	FoxState.GlobalState.CurrentState = GfsBeforeFinish;
	FoxState.GlobalState.StateChangeTime = FoxState.GlobalState.EndDateTime;

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
	uint32_t preparationStartTime = FoxState.GlobalState.StartDateTime - YHL_HL_FOX_PREPARATION_AND_MATCHING_TIME;

	if (FoxState.CurrentTime < preparationStartTime)
	{
		/* Current time < Start time -> Before start */
		return GfsBeforeStart;
	}

	if ((FoxState.CurrentTime >= preparationStartTime) && (FoxState.CurrentTime < FoxState.GlobalState.StartDateTime))
	{
		return GfsPreparation;
	}

	if (FoxState.CurrentTime < FoxState.GlobalState.EndDateTime)
	{
		/* Start time <= Current time < End time */
		return GfsBeforeFinish;
	}

	return GfsStandby;
}

