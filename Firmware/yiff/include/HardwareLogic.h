/*
 * HardwareLogic.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_HARDWARELOGIC_H_
#define INCLUDE_HARDWARELOGIC_H_

#include <FoxStatus.h>
#include <HAL.h>
#include <MatchingDisplay.h>
#include <PacketsProcessor.h>
#include <SelfDiagnostics.h>
#include <Sleepmodes.h>

extern FoxStateStruct FoxState;
extern L2HAL_HC06_ContextStruct HC06_Context;
extern L2HAL_SSD1327_ContextStruct L2HAL_SSD1327_Context;
extern L2HAL_HC06_ContextStruct HC06_Context;

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
 * Fox must be ready for transmission not later than this amount of seconds antenna matching initiation
 */
#define YHL_HL_FOX_PREPARATION_AND_MATCHING_TIME 120U

/**
 * Switch bluetooth power off for this duration when renaming device
 */
#define YHL_HL_BLUETOOTH_POWERCYCLE_LENGTH 1000

/**
 * Bluetooth device pin
 */
#define YHL_HL_BLUETOOTH_PIN "0000"

/**
 * Time to wait after powering display down before attempt to turn it on
 */
#define YHL_HL_DISPLAY_POWERDOWN_TIME 1000

/**
 * Time to spin display power regulator up
 */
#define YHL_HL_DISPLAY_REGULATOR_SPINUP_TIME 1000

/**
 * Try to initialize display this times before reporting a failure
 */
#define YHL_HL_INITIALIZE_DISPLAY_ATTEMPS_COUNT 5

/**
 * Time to spin bluettoth power regulator up
 */
#define YHL_HL_BLUETOOTH_REGULATOR_SPINUP_TIME 2000

/**
 * How long to wait between powering bluetooth down and powering it up again when renaming device
 */
#define YHL_HL_BLUETOOTH_RENAME_POWERDOWN_TIME 2000

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
 * Terminates 3.5MHz output stage voltage control loop and setting
 * a voltage to a lowest possible level
 */
void HL_DeSetupU80m(void);

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
 * Renames bluetooth device (will lead to bluetooth device power cycling)
 */
void HL_RenameBluetoothDevice(char* newName);

/**
 * Turns display on and initializes it
 */
void HL_TurnDisplayOn(void);

/**
 * Powers display down
 */
void HL_TurnDisplayOff(void);

/**
 * Turns bluetooth on and initializes it
 */
void HL_TurnBluetoothOn(void);

/**
 * Powers bluetooth down
 */
void HL_TurnBluetoothOff(void);

/**
 * Is U80m locked?
 */
bool HL_CheckIfU80mLocked(void);

/**
 * Forces continuous transmission, assuming that fox is prepared for cycle
 */
void HL_ForceTxOn(void);

#endif /* INCLUDE_HARDWARELOGIC_H_ */
