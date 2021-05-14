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
void GSM_Tick(void);

/**
 * Cancel current operation and move SM to Standby
 */
void GSM_Cancel(void);

/**
 * Program fox to start at given time and finish at given time
 */
void GSM_Program(Time startTime, Time endTime);

/**
 * Call this to fix global state machine state after sudden time change (i.e. setting fox time from phone)
 */
void GSM_FixStateAfterTimeChange(void);


/* Private stuff goes below */

/**
 * Start the fox and move machine into BeforeFinish state.
 */
void GSM_StartFox(void);

/**
 * Stop the fox and move machine into Standby state.
 */
void GSM_StopFox(void);

#endif /* INCLUDE_GLOBALSTATEMACHINE_H_ */
