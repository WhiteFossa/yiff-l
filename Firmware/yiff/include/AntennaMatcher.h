/*
 * AntennaMatcher.h
 *
 *  Created on: Dec 27, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_ANTENNAMATCHER_H_
#define INCLUDE_ANTENNAMATCHER_H_

#include <SelfDiagnostics.h>
#include <HardwareLogic.h>
#include <PacketsProcessor.h>

/**
 * How long to wait between switching relays and turning on carrier
 */
#define AMSM_ON_COMMUTATION_DELAY 100U

/**
 * How long to keep carrier on to wait for antenna voltage measurement
 */
#define AMSM_ON_MEASUREMENT_DELAY 300U

/**
 * How long to wait between turning carrier off and switching relays
 */
#define AMSM_OFF_COMMUTATION_DELAY 100U

/**
 * Global fox state
 */
typedef enum
{
	/**
	 * We aren't in process of matching
	 */
	AMSM_Inactive,

	/**
	 * Waiting for U80m voltage lock
	 */
	AMSM_WaitingForU80mLock,

	/**
	 * Waiting for relays to switch on
	 */
	AMSM_WaitingForOnCommutation,

	/**
	 * Waiting for antenna voltage measurement
	 */
	AMSM_WaitingForMeasurement,

	/**
	 * Waiting for relays to switch off
	 */
	AMSM_WaitingForOffCommutation
}
AntennaMatcherStateEnum;

extern FoxStateStruct FoxState;

/**
 * Initialize antenna matcher
 */
void AMSM_Init(void);

/**
 * Move state machine to next state
 */
void AMSM_MoveToNextState(void);

/**
 * Starts antenna matching procedure after some checks.
 */
void AMSM_StartMatching(bool isArmFoxAfterMatching);

/**
 * Call it each SysTick
 */
void AMSM_SysTickHandler(void);

/**
 * Returns optimal matcher position
 */
uint8_t AMSM_GetOptimalMatching(void);


#endif /* INCLUDE_ANTENNAMATCHER_H_ */
