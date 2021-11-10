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
#include <PacketsProcessor.h>
#include <UART.h>

/**
 * Awake -> Sleep after this timeout
 */
#define YHL_SLEEPMODES_SLEEP_TIMEOUT 60U

/**
 * Sleep -> Deep sleep after this timeout
 */
#define YHL_SLEEPMODES_DEEP_SLEEP_TIMEOUT 10U

extern FoxStateStruct FoxState;

/**
 * Initialize various stuff, related to sleepmodes
 */
void Sleepmodes_Init(void);

/**
 * Call this to prevent ordinary sleep. If fox is sleeping or deep sleeping, it will be awaked
 */
void Sleepmodes_PreventSleep(void);

/**
 * Call this to prevent deep sleep. If fox awakened or sleeping it will do nothing, if it is deep sleeping, it will
 * change fox state to ordinary sleep.
 */
void Sleepmodes_PreventDeepSleep(void);

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
 * Enter sleep, display and bluetooth are disabled here
 */
void Sleepmodes_EnterSleep(void);

/**
 * Enter deep sleep, everything except MCU is powered down. Available only from sleep mode
 */
void Sleepmodes_EnterDeepSleep(void);

/**
 * Wakes fox up from sleep
 */
void Sleepmodes_WakeFromSleep(void);

/**
 * Wakes fox up from deep sleep
 */
void Sleepmodes_WakeFromDeepSleep(void);

#endif /* INCLUDE_SLEEPMODES_H_ */
