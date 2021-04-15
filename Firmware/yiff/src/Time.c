/*
 * Time.c
 *
 *  Created on: Apr 11, 2021
 *      Author: fossa
 */

#include <main.h>

void TimeToHMS(Time time, char* result)
{
	sprintf(result, "%02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);
}

void TimeToMS(Time time, char* result)
{
	if (time.Hours > 0)
	{
		L2HAL_Error(Generic);
	}

	sprintf(result, "%02d:%02d", time.Minutes, time.Seconds);
}

uint32_t SecondsSinceDayBegin(Time time)
{
	return 3600UL * time.Hours + 60UL * time.Minutes + time.Seconds;
}

Time TimeSinceDayBegin(uint32_t seconds)
{
	Time result;

	result.Hours = seconds / 3600UL;

	seconds -= result.Hours * 3600UL;

	result.Minutes = seconds / 60UL;

	seconds -= result.Minutes * 60UL;

	result.Seconds = seconds;

	return result;
}

void NewSecondCallback()
{
	FoxState.CurrentTime.Hours = CurrentTime.Hours;
	FoxState.CurrentTime.Minutes = CurrentTime.Minutes;
	FoxState.CurrentTime.Seconds = CurrentTime.Seconds;

	DrawStatusDisplay(FoxState);
}
