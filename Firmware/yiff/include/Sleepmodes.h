/*
 * Sleepmodes.h
 *
 *  Created on: Nov 6, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_SLEEPMODES_H_
#define INCLUDE_SLEEPMODES_H_

#include <FoxStatus.h>
#include <RTC.h>

/**
 * Enter sleep after this timeout
 */
#define YHL_SLEEPMODES_SLEEP_TIMEOUT 60U

extern FoxStateStruct FoxState;

/**
 * Initialize various stuff, related to sleepmodes
 */
void Sleepmodes_Init(void);

/**
 * Reset sleep timer
 */
void Sleepmodes_ResetSleepTimer(void);

/**
 * Call this every new second
 */
void Sleepmodes_OnNewSecond(void);

/**
 * Deactivate controls
 */
void Sleepmodes_DeactivateControls(void);

/**
 * Activate controls
 */
void Sleepmodes_ActivateControls(void);

/**
 * Enter deep sleep, when everything except MCU is powered down
 */
void Sleepmodes_EnterDeepsleep(void);

/**
 * Wakes fox up from deep sleep
 */
void Sleepmodes_WakeFromDeepsleep(void);

#endif /* INCLUDE_SLEEPMODES_H_ */
