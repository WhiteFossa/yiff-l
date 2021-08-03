/*
 * Time.c
 *
 *  Created on: Apr 11, 2021
 *      Author: fossa
 */

#include <time.h>
#include <main.h>
#include <TimePrivate.h>

uint32_t TimeToTimestamp(Time time)
{
	return (int32_t)YHL_TIME_DAY_ZERO_TIMESTAMP
			+ (int32_t)(time.Days * YHL_TIME_DAY_IN_SECONDS)
			+ (int32_t)(time.Hours * YHL_TIME_HOUR_IN_SECONDS)
			+ (int32_t)(time.Minutes * YHL_TIME_MINUTE_IN_SECONDS)
			+ (int32_t)time.Seconds;
}

Time TimestampToTime(uint32_t timestamp)
{
	int64_t timestampLong = timestamp;

	bool isInPast = false;
	if (timestampLong < (int64_t)YHL_TIME_DAY_ZERO_TIMESTAMP)
	{
		isInPast = true;
	}

	Time result;

	result.Days = ((int64_t)timestampLong - (int64_t)YHL_TIME_DAY_ZERO_TIMESTAMP) / (int64_t)YHL_TIME_DAY_IN_SECONDS;

	if (isInPast)
	{
		result.Days --;
	}

	timestampLong -= (int64_t)YHL_TIME_DAY_ZERO_TIMESTAMP + (int64_t)result.Days * (int64_t)YHL_TIME_DAY_IN_SECONDS;

	result.Hours = (uint8_t)(timestampLong / YHL_TIME_HOUR_IN_SECONDS);
	timestampLong -= result.Hours * YHL_TIME_HOUR_IN_SECONDS;

	result.Minutes = (uint8_t)(timestampLong / YHL_TIME_MINUTE_IN_SECONDS);
	timestampLong -= result.Minutes * YHL_TIME_MINUTE_IN_SECONDS;

	result.Seconds = (uint8_t)timestampLong;

	return result;
}

void TimestampToHMSString(uint32_t timestamp, char* result)
{
	Time time = TimestampToTime(timestamp);

	sprintf(result, "%02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);
}

void TimestampToMSString(uint32_t timestamp, char* result)
{
	Time time = TimestampToTime(timestamp);

	sprintf(result, "%02d:%02d", time.Minutes, time.Seconds);
}

void TimespanToMSString(uint32_t timespan, char* result)
{
	uint32_t timestamp = timespan + YHL_TIME_DAY_ZERO_TIMESTAMP;

	TimestampToMSString(timestamp, result);
}

void TimespanToHMSString(uint32_t timespan, char* result)
{
	uint32_t timestamp = timespan + YHL_TIME_DAY_ZERO_TIMESTAMP;

	TimestampToHMSString(timestamp, result);
}

void NewSecondCallback(void)
{
	Time CurrentRTCTime;
	CurrentRTCTime.Days = CurrentDate.Year * 365 + CurrentDate.Month * 30 + CurrentDate.Date; /* We don't need precise conversion here */
	CurrentRTCTime.Hours = CurrentTime.Hours;
	CurrentRTCTime.Minutes = CurrentTime.Minutes;
	CurrentRTCTime.Seconds = CurrentTime.Seconds;

	FoxState.CurrentTime = TimeToTimestamp(CurrentRTCTime);

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

uint32_t TimestampToSecondsSinceMidnight(uint32_t timestamp)
{
	Time time = TimestampToTime(timestamp);

	return time.Hours * YHL_TIME_HOUR_IN_SECONDS + time.Minutes * YHL_TIME_MINUTE_IN_SECONDS + time.Seconds;
}

uint32_t GetMidnightTimestamp(uint32_t timestamp)
{
	Time time = TimestampToTime(timestamp);
	time.Hours = 0;
	time.Minutes = 0;
	time.Seconds = 0;

	return TimeToTimestamp(time);
}
