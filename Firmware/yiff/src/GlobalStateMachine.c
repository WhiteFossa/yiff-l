/*
 * GlobalStateMachine.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

void GSM_Cancel(void)
{
	FoxState.GlobalState.CurrentState = Standby;
}

void GSM_Program(void)
{
	if (CompareTimes(FoxState.GlobalState.StartTime, FoxState.GlobalState.EndTime) != TIME1_LESS)
	{
		L2HAL_Error(Generic);
	}

	if ((CompareTimes(FoxState.GlobalState.StartTime, FoxState.CurrentTime) != TIME2_LESS) && (CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.EndTime) != TIME2_LESS))
	{
		/* Current time can't be between start time and end time*/
		L2HAL_Error(Generic);
	}

	GSM_MoveToBeforeStart();
}

void GSM_MoveToBeforeStart(void)
{
	FoxState.GlobalState.StateChangeTime = FoxState.GlobalState.StartTime;
	FoxState.GlobalState.CurrentState = BeforeStart;

	CSM_Stop();
}

void GSM_Tick(void)
{
	int8_t comparisonResult = CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.StateChangeTime);

	switch (FoxState.GlobalState.CurrentState)
	{
		case Standby:
			/* It's impossible to get out from standby on timer */
			break;

		case BeforeStart:

			if (TIMES_EQUAL == comparisonResult)
			{
				GSM_StartFox();
			}

			break;

		case BeforeFinish:
			if (TIMES_EQUAL == comparisonResult)
			{
				GSM_StopFox();
			}

			break;

		default:
			L2HAL_Error(Generic);
	}
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

void GSM_FixStateAfterTimeChange(void)
{
	int8_t compResStartTime = CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.StartTime);
	int8_t compResEndTime = CompareTimes(FoxState.CurrentTime, FoxState.GlobalState.EndTime);

	bool timeLStartTime = TIME1_LESS == compResStartTime; /* T < StartTime */
	bool timeGEEndTime = TIME2_LESS == compResEndTime || TIMES_EQUAL == compResEndTime; /* T >= EndTime */

	switch(FoxState.GlobalState.CurrentState)
	{
		case Standby:
			if (timeLStartTime)
			{
				/* T < StartTime -> BeforeStart */
				GSM_MoveToBeforeStart();
				return;
			}

			if (timeGEEndTime)
			{
				/* T >= EndTime -> Standby */
				return;
			}

			/* StartTime <= T < EndTime -> BeforeFinish */
			GSM_StartFox();
			return;

		case BeforeStart:
			if (timeLStartTime)
			{
				/* T < StartTime -> BeforeStart */
				return;
			}

			if (timeGEEndTime)
			{
				/* T >= EndTime -> Standby */
				GSM_StopFox();
				return;
			}

			/* StartTime <= T < EndTime -> BeforeFinish */
			GSM_StartFox();
			return;

		case BeforeFinish:
			if (timeLStartTime)
			{
				/* T < StartTime -> BeforeStart */
				GSM_MoveToBeforeStart();
				return;
			}

			if (timeGEEndTime)
			{
				/* T >= EndTime -> Standby */
				GSM_StopFox();
				return;
			}

			/* StartTime <= T < EndTime -> BeforeFinish */
			return;
	}
}
