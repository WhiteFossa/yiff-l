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
#include <SelfDiagnostics.h>

extern RTC_HandleTypeDef RtcHandle;
extern uint8_t PreviousSecond;
extern uint8_t RtcOnNewSecondListenersCount;
extern void (**RtcOnNewSecondListeners)(void);
extern uint8_t RtcDateAndTimeChangeListenersCount;
extern void (**RtcDateAndTimeChangeListeners)(void);
extern RTC_TimeTypeDef CurrentTime;
extern RTC_DateTypeDef CurrentDate;


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
void RTC_AddOnNewSecondListener(void (*listener)(void));

/**
 * Returns weekday based on day number. Monday is 1, Sunday is 7
 */
uint8_t GetWeekdayFromDayNumber(uint8_t dayNumber);

/**
 * Call this function to add new change date or time listener. Listeners are called immediately after RTC_Set_CurrentDate()
 * or RTC_SetCurrentTime() calls.
 */
void RTC_AddOnDateOrTimeChangeListener(void (*listener)(void));


/* Private stuff goes below */

/**
 * Call on date or time change listeners
 */
void RTC_CallOnDateOrTimeChangeListeners(void);

#endif /* INCLUDE_RTC_H_ */
