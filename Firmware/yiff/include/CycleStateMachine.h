/*
 * CycleStateMachine.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_CYCLESTATEMACHINE_H_
#define INCLUDE_CYCLESTATEMACHINE_H_

#include <Time.h>
#include <HardwareLogic.h>

/**
 * Since this moment ending tone must be transmitted.
 */
Time EndingToneStartTime;

/**
 * Start cycle immediately.
 */
void CSM_Start(void);

/**
 * Stops cycle (needed to terminate transmission)
 */
void CSM_Stop(void);

/**
 * Call it every second
 */
void CSM_Tick(void);

/**
 * Calculates ending tone start time if fox in cyclic mode and current cycle state is TX.
 */
void CSM_CalculateEndingToneStartTime(void);


#endif /* INCLUDE_CYCLESTATEMACHINE_H_ */
