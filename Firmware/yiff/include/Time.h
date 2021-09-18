/*
 * Time.h
 *
 *  Created on: Apr 11, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

/**
 * 00:00:00 of day 0 have this timestamp
 */
#define YHL_TIME_DAY_ZERO_TIMESTAMP 2147483648U

/**
 * Intervals in seconds
 */
#define YHL_TIME_DAY_IN_SECONDS 86400U
#define YHL_TIME_HOUR_IN_SECONDS 3600U
#define YHL_TIME_MINUTE_IN_SECONDS 60

#include <l2hal.h>

typedef struct
{
	/**
	 * Days since fox power on
	 */
	int32_t Days;

	/**
	 * Hours within current day
	 */
	uint8_t Hours;

	/**
	 * Minutes within current day
	 */
	uint8_t Minutes;

	/**
	 * Seconds within current day
	 */
	uint8_t Seconds;
}
Time;

/**
 * Converts time structure to timestamp
 */
uint32_t TimeToTimestamp(Time time);

/**
 * Converts timestamp to time structure
 */
Time TimestampToTime(uint32_t timestamp);

/**
 * Converts time to string with hours, minutes and seconds (days are dropped)
 */
void TimestampToHMSString(uint32_t timestamp, char* result, uint8_t resultSize);

/**
 * Converts time to string with minutes and seconds (days and hours are dropped)
 */
void TimestampToMSString(uint32_t timestamp, char* result, uint8_t resultSize);

/**
 * Converts timeSPAN to string with minutes and seconds (days and hours are dropped)
 */
void TimespanToMSString(uint32_t timespan, char* result, uint8_t resultSize);

/**
 * Converts timeSPAN to string with hours, minutes and seconds (days are dropped)
 */
void TimespanToHMSString(uint32_t timespan, char* result, uint8_t resultSize);

/**
 * Returns amount of seconds since midnight (days are dropped)
 */
uint32_t TimestampToSecondsSinceMidnight(uint32_t timestamp);

/**
 * Gets midnight of given day
 */
uint32_t GetMidnightTimestamp(uint32_t timestamp);

/**
 * Call this function on each new second.
 */
void NewSecondCallback(void);

#endif /* INCLUDE_TIME_H_ */
