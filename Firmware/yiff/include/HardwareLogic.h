/*
 * HardwareLogic.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_HARDWARELOGIC_H_
#define INCLUDE_HARDWARELOGIC_H_

/**
 * Minimal and maximal voltages of 3.5MHz output stage power sources
 */
#define YHL_HL_MIN_U80M 9.0f
#define YHL_HL_MAX_U80M 20.f

/**
 * Allowed U80m set-up error
 */
#define YHL_HL_U80M_TOLERANCY 0.1f

/**
 * Proportional factor for U80m regulation
 */
#define YHL_HL_U80M_PROPORTIONAL_REGULATION_FACTOR 10.0f

/**
 * If U80m is within limits during this count of measurements we assume that we found required U80m regulator code
 */
#define YHL_HL_U80M_LOCK_DURATION 100U

/**
 * Synthesizer clock is multiplied by this value in 144MHz tract
 */
#define YHL_HL_2M_FREQUENCY_MULTIPLICATION_FACTOR 8.0f

/**
 * Fox must be ready for transmission not later than this amount of seconds after
 * HL_PrepareFoxForCycle() call
 */
#define YHL_HL_FOX_PREPARATION_TIME 5U

/**
 * How long to wait for UAnt measurement after changing in antenna matching
 */
#define YHL_HL_FOX_WAIT_FOR_UANT_DELAY 500

/**
 * Initialize hardware logic
 */
void HL_Init(void);

/**
 * Call it when manipulator-related parts of state changes
 */
void HL_ProcessManipulatorFoxStateChange(void);

/**
 * Call it to set-up 3.5MHz output stage voltage
 */
void HL_SetupU80m(float targetVoltage);

/**
 * Call this function to set U80m lock callback.
 * This callback will be called when desired voltage is achieved after HL_SetupU80m() call.
 */
void HL_SetU80mLockCallback(void (*callback)(void));

/**
 * Prepares for for 80m cycle
 */
void HL_PrepareFoxFor80mCycle(void);

/**
 * Un-prepares fox after 80m cycle
 */
void HL_UnPrepareFoxFrom80mCycle(void);

/**
 * Prepares for fox 2m cycle
 */
void HL_PrepareFoxFor2mCycle(void);

/**
 * Un-prepares fox after 2m cycle
 */
void HL_UnPrepareFoxFrom2mCycle(void);

/**
 * Prepares fox for cycle (band-independent)
 */
void HL_PrepareFoxForCycle(void);

/**
 * Un-prepares fox after cycle (band-independent)
 */
void HL_UnPrepareFoxFromCycle(void);

/**
 * Is fox prepared for transmission?
 */
bool HL_CheckIsFoxPrepared(void);

/**
 * Setup antenna matching for optimal output signal
 */
void HL_Setup80mAntenna(void);

/**
 * Prepares for and matches antenna:
 * 1) Prepares fox
 * 2) Waits for U80m lock
 * 3) Matches antenna
 */
void HL_PrepareAndMatch80m(void);

#endif /* INCLUDE_HARDWARELOGIC_H_ */
