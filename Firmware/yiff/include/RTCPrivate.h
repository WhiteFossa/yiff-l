/*
 * RTCPrivate.h
 *
 *  Created on: May 31, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_RTCPRIVATE_H_
#define INCLUDE_RTCPRIVATE_H_


/**
 * RTC handle
 */
RTC_HandleTypeDef RtcHandle;

/**
 * Second number at the moment of previous call of RTC_Poll();
 */
uint8_t PreviousSecond;

/**
 * How many listeners we have.
 */
uint8_t RtcOnNewSecondListenersCount;

/**
 * This functions listens for new second event.
 */
void (**RtcOnNewSecondListeners)(void);

/**
 * How many date and time change listeners we have.
 */
uint8_t RtcDateAndTimeChangeListenersCount;

/**
 * This functions listens for date and time change.
 */
void (**RtcDateAndTimeChangeListeners)(void);


#endif /* INCLUDE_RTCPRIVATE_H_ */
