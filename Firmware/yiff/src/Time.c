/*
 * Time.c
 *
 *  Created on: Apr 11, 2021
 *      Author: fossa
 */

#include <main.h>
#include <TimePrivate.h>

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

	result.Hours = (uint8_t)(seconds / 3600UL);

	seconds -= result.Hours * 3600UL;

	result.Minutes = (uint8_t)(seconds / 60UL);

	seconds -= result.Minutes * 60UL;

	result.Seconds = (uint8_t)seconds;

	return result;
}

void NewSecondCallback(void)
{
	FoxState.CurrentTime.Hours = CurrentTime.Hours;
	FoxState.CurrentTime.Minutes = CurrentTime.Minutes;
	FoxState.CurrentTime.Seconds = CurrentTime.Seconds;

	/* Global state machine */
	GSM_Tick();

	/* Cycle state machine */
	CSM_Tick();

	/* Only status display need to be redrawn each second */
	if (StatusDisplay == FoxState.CurrentDisplay)
	{
		DrawStatusDisplay(FoxState);
	}
}

Time ToTime(RTC_TimeTypeDef rtcTime)
{
	Time time;
	time.Hours = rtcTime.Hours;
	time.Minutes = rtcTime.Minutes;
	time.Seconds = rtcTime.Seconds;

	return time;
}

int8_t CompareTimes(Time time1, Time time2)
{
	uint32_t seconds1 = SecondsSinceDayBegin(time1);
	uint32_t seconds2 = SecondsSinceDayBegin(time2);

	if (seconds1 == seconds2)
	{
		return TIMES_EQUAL;
	}

	if (seconds1 < seconds2)
	{
		return TIME1_LESS;
	}

	return TIME2_LESS;
}

Time AddTimes(Time time1, Time time2)
{
	return TimeSinceDayBegin(SecondsSinceDayBegin(time1) + SecondsSinceDayBegin(time2));
}

Time SubtractSeconds(Time time, uint32_t seconds)
{
	return TimeSinceDayBegin(SecondsSinceDayBegin(time) - seconds);
}

Time SubtractTimes(Time time1, Time time2)
{
	return SubtractSeconds(time1, SecondsSinceDayBegin(time2));
}
