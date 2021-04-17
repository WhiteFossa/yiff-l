/*
 * GlobalStateMachine.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_GLOBALSTATEMACHINE_H_
#define INCLUDE_GLOBALSTATEMACHINE_H_

#include <Time.h>

/**
 * When fox will turn on.
 */
Time GsmStartTime;

/**
 * When fox will turn off.
 */
Time GsmEndTime;

/**
 * Call it every second
 */
void GSM_Tick();

/**
 * Cancel current operation and move SM to Standby
 */
void GSM_Cancel();

/**
 * Program fox to start at given time and finish at given time
 */
void GSM_Program(Time startTime, Time endTime);


#endif /* INCLUDE_GLOBALSTATEMACHINE_H_ */
