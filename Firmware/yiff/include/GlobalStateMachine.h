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
 * Initialize state machine
 */
void GSM_Init(void);

/**
 * Call it every second
 */
void GSM_Tick(void);

/**
 * Cancel current operation and move SM to Standby
 */
void GSM_Disarm(void);

/**
 * Program fox to start and finish at FoxState.GlobalState.StartTime and FoxState.GlobalState.EndTime
 */
void GSM_Arm(void);

/**
 * Returns true if fox is armed, false otherwise
 */
bool GSM_IsArmed(void);


/* Private stuff goes below */

/**
 * Stops the fox and move machine into BeforeStart state
 */
void GSM_MoveToBeforeStart(void);

/**
 * Start the fox and move machine into BeforeFinish state.
 */
void GSM_StartFox(void);

/**
 * Stop the fox and move machine into Standby state.
 */
void GSM_StopFox(void);

/**
 * Returns expected state based on current time
 */
GlobalFoxStateEnum GSM_GetExpectedState(void);

#endif /* INCLUDE_GLOBALSTATEMACHINE_H_ */
