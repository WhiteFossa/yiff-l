/*
 * CycleStateMachine.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_CYCLESTATEMACHINE_H_
#define INCLUDE_CYCLESTATEMACHINE_H_

#include <Time.h>

/**
 * Since this moment ending tone must be transmitted.
 */
Time EndingToneStartTime;

/**
 * Start cycle immediately.
 */
void CSM_Start();

/**
 * Call it every second
 */
void CSM_Tick();

/**
 * Calculates ending tone start time if fox in cyclic mode and current cycle state is TX.
 */
void CSM_CalculateEndingToneStartTime();


#endif /* INCLUDE_CYCLESTATEMACHINE_H_ */
