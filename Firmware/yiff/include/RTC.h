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

/**
 * How much new second listeners we can have
 */
#define YHL_MAX_ON_NEW_SECOND_LISTENERS 4U

/**
 * How much date and time change listeners we can have
 */
#define YHL_MAX_ON_DATE_AND_TIME_CHANGE_LISTENERS 4U

extern RTC_HandleTypeDef RtcHandle;
extern uint8_t PreviousSecond;
extern uint8_t RtcOnNewSecondListenersCount;
extern void (*RtcOnNewSecondListeners[YHL_MAX_ON_NEW_SECOND_LISTENERS])(void);
extern uint8_t RtcDateAndTimeChangeListenersCount;
extern void (*RtcDateAndTimeChangeListeners[YHL_MAX_ON_DATE_AND_TIME_CHANGE_LISTENERS])(void);
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

/**
 * Set calibration value. For parameter values see HAL_SetRTCCalibrationValue
 */
void RTC_SetCalibrationValue(uint8_t calibrationValue);

/* Private stuff goes below */

/**
 * Call on date or time change listeners
 */
void RTC_CallOnDateOrTimeChangeListeners(void);

#endif /* INCLUDE_RTC_H_ */
