/*
 * Time.h
 *
 *  Created on: Apr 11, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

#include <l2hal.h>

typedef struct
{
	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;
}
Time;

/**
 * Converts time to string with hours, minutes and seconds
 */
void TimeToHMS(Time time, char* result);

/**
 * Converts time to string with minutes and seconds. Hours must be zero
 */
void TimeToMS(Time time, char* result);

/**
 * Returns amount of seconds since day begin
 */
uint32_t SecondsSinceDayBegin(Time time);

/**
 * Returns time since day begin.
 */
Time TimeSinceDayBegin(uint32_t seconds);

/**
 * Call this function on each new second.
 */
void NewSecondCallback();

/**
 * Convert RTC time to our time.
 */
Time ToTime(RTC_TimeTypeDef rtcTime);

/**
 * Compare two times.
 * -1 if time2 less than time1
 * 0 if time1 is equal to time2
 * 1 if time1 less than time2
 */
int8_t CompareTimes(Time time1, Time time2);

/**
 * Adds two times.
 */
Time AddTimes(Time time1, Time time2);

#endif /* INCLUDE_TIME_H_ */
