/*
 * HardwareLogicPrivate.h
 *
 *  Created on: Jul 5, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_HARDWARELOGICPRIVATE_H_
#define INCLUDE_HARDWARELOGICPRIVATE_H_

/**
 * Target voltage for U80m
 */
float HL_U80mTarget;

/**
 * If true, then U80m voltage feedback is active
 */
bool HL_IsU80mFeedbackActive;

/**
 * How many measurements U80m is within the required limits
 */
uint16_t HL_U80mLockCounter;

/**
 * Is fox prepared for transmission?
 */
bool HL_IsFoxPrepared;

/**
 * This callback is called when target U80m is achieved
 */
void (*HL_U80mLockCallback)(void);

/**
 * Call it when new measurement of U80m arrives
 */
void HL_U80mMeasurementCallback(void);

#endif /* INCLUDE_HARDWARELOGICPRIVATE_H_ */
