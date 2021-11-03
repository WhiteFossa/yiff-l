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
#include <FoxStatus.h>
#include <MorsePlayer.h>
#include <SelfDiagnostics.h>

extern FoxStateStruct FoxState;

/* Initialize cycle state machine */
void CSM_Init(void);

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
 * Get time inside a cycle (e.g. 0 when new cycle just started). Returns -1 if fox not in cycle
 */
int16_t CSM_GetCycleTime(void);

/**
 * Get current cycle state by time since cycle begin
 */
CycleStateEnum CSM_GetStateByCycleTime(uint32_t timeSinceCycleStart);

/**
 * Prepare fox
 */
void CSM_Cycle_PrepareFox(void);

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

/**
 * Calculates next state change time
 */
void CSM_RecalculateStateChangeTime(uint16_t timeSinceCycleBegin);

#endif /* INCLUDE_CYCLESTATEMACHINE_H_ */
