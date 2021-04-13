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

#endif /* INCLUDE_TIME_H_ */
