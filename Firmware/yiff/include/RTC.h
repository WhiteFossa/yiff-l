/*
 * RTC.h
 *
 *  Created on: Apr 15, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_RTC_H_
#define INCLUDE_RTC_H_

#include <l2hal.h>

/**
 * RTC handle
 */
RTC_HandleTypeDef RtcHandle;

/**
 * Second number at the moment of previous call of RTC_Poll();
 */
uint8_t PreviousSecond;

/**
 * Current date and time.
 */
RTC_TimeTypeDef CurrentTime;
RTC_DateTypeDef CurrentDate;

/**
 * Initialize RTC
 */
void InitRTC();

/**
 * Call this function as fast as possible, providing callback function pointer to it.
 * Callback will be called when new second arrive.
 */
void RTC_Poll(void (*callbackPtr)());

#endif /* INCLUDE_RTC_H_ */
