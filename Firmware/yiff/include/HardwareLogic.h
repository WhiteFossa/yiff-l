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
 * If U80m is within limits during this count of measurements we assume that we found required U80m regulator code
 */
#define YHL_HL_U80M_LOCK_DURATION 100U

/**
 * Initialize hardware logic
 */
void HL_Init(void);

/**
 * Call it when manipulator-related parts of state changes
 */
void ProcessManipulatorFoxStateChange(void);

/**
 * Call it to set-up 3.5MHz output stage voltage
 */
void HL_SetupU80m(float targetVoltage);

#endif /* INCLUDE_HARDWARELOGIC_H_ */
