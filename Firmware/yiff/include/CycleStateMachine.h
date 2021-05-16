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

/* Private stuff goes below */

/**
 * Calculates ending tone start time if fox in cyclic mode and current cycle state is TX.
 */
void CSM_CalculateEndingToneStartTime(void);

/**
 * Get time inside a cycle (e.g. 0 when new cycle just started). Returns -1 if fox not in cycle
 */
int16_t CSM_GetCycleTime(void);

/**
 * Get current cycle state by time since cycle begin
 */
CycleStateEnum CSM_GetStateByCycleTime(int16_t cycleTime);

/**
 * Start transmission
 */
void CSM_Cycle_StartTx(uint16_t timeSinceCycleBegin);

/**
 * Start ending tone
 */
void CSM_Cycle_StartEndingTone(uint16_t timeSinceCycleBegin);

/**
 * Start pause
 */
void CSM_Cycle_StartPause(uint16_t timeSinceCycleBegin);

#endif /* INCLUDE_CYCLESTATEMACHINE_H_ */
