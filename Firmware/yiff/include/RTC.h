/*
 * RTC.h
 *
 *  Created on: Apr 15, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_RTC_H_
#define INCLUDE_RTC_H_

#include <l2hal.h>
#include <stdlib.h>

/**
 * RTC handle
 */
RTC_HandleTypeDef RtcHandle;

/**
 * Second number at the moment of previous call of RTC_Poll();
 */
uint8_t PreviousSecond;

/**
 * This functions listens for new second event.
 */
void (**RtcListeners)(void);

/**
 * How many listeners we have.
 */
uint8_t RtcListenersCount;

/**
 * Current date and time.
 */
RTC_TimeTypeDef CurrentTime;
RTC_DateTypeDef CurrentDate;

/**
 * Initialize RTC
 */
void InitRTC(void);

/**
 * Set current date
 */
void RTC_SetCurrentDate(RTC_DateTypeDef date);

/**
 * Set current time
 */
void RTC_SetCurrentTime(RTC_TimeTypeDef time);

/**
 * Call this function as often as possible.
 */
void RTC_Poll(void);

/**
 * Call this function to add new listener. Listener will be called at the beginning of each new second.
 */
void RTC_AddListener(void (*listener)(void));

/**
 * Returns weekday based on day number. Monday is 1, Sunday is 7
 */
uint8_t GetWeekdayFromDayNumber(uint8_t dayNumber);

#endif /* INCLUDE_RTC_H_ */
