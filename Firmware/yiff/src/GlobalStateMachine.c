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

void GSM_Program(Time startTime, Time endTime)
{
	if (CompareTimes(startTime, endTime) != 1)
	{
		L2HAL_Error(Generic);
	}

	Time time = ToTime(CurrentTime);
	if ((CompareTimes(startTime, time) != -1) && (CompareTimes(time, endTime) != -1))
	{
		/* Current time can't be between start time and end time*/
		L2HAL_Error(Generic);
	}

	GsmStartTime = startTime;
	GsmEndTime = endTime;

	FoxState.GlobalState.StateChangeTime = GsmStartTime;
	FoxState.GlobalState.CurrentState = BeforeStart;
}

void GSM_Tick(void)
{
	Time time = ToTime(CurrentTime);
	int8_t comparisonResult = CompareTimes(time, FoxState.GlobalState.StateChangeTime);

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
	FoxState.GlobalState.StateChangeTime = GsmEndTime;

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
	int8_t comparisonResult;

	switch(FoxState.GlobalState.CurrentState)
	{
		case Standby:
			/* No need to do anything */
			break;

		case BeforeStart:
			comparisonResult = CompareTimes(FoxState.CurrentTime, GsmStartTime);

			if (TIME1_LESS == comparisonResult)
			{
				return;
			}
			else
			{
				/* >= start time*/
				comparisonResult = CompareTimes(FoxState.CurrentTime, GsmEndTime);

				if (TIME1_LESS == comparisonResult)
				{
					/* < end time, starting the fox */
					GSM_StartFox();
				}
				else
				{
					GSM_StopFox();
				}
			}


			break;

		case BeforeFinish:

			comparisonResult = CompareTimes(FoxState.CurrentTime, GsmEndTime);

			if (TIME1_LESS == comparisonResult)
			{
				return;
			}
			else
			{
				GSM_StopFox();
			}

			break;
	}
}
