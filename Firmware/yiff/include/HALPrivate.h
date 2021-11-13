/*
 * HALPrivate.h
 *
 *  Created on: Jul 3, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_HALPRIVATE_H_
#define INCLUDE_HALPRIVATE_H_

/**
 * Interval in milliseconds, during what continuous events will be ignored
 */
#define HAL_BUTTONS_NOISE_SUPPRESSION_INTERVAL 200U

/**
 * Averaged antenna voltage (in ADC measurements)
 */
volatile float HAL_AntennaLevelADC;

/**
 * Averaged battery level (in ADC measurements)
 */
volatile float HAL_BatteryLevelADC;

/**
 * Averaged power voltage for 3.5MHz output stage (in ADC measurements)S
 */
volatile float HAL_80mLevelADC;

/**
 * Accumulating measurements here
 */
volatile float HAL_ADCAccumulator;

/**
 * Callback, called when new measurement of 3.5MHz output stage voltage arrives
 */
void (*HAL_U80mNewMeasurementCallback)(void);

/**
 * Current U80m regulator code
 */
uint8_t HAL_CurrentU80mCode;

/**
 * Averages counter
 */
volatile uint16_t HAL_ADCAveragesCounter;

/**
 * Current ADC channel
 */
volatile YHL_HAL_ADCChannelEnum HAL_CurrentADCChannel;

/**
 * Current position of encoder
 */
volatile int8_t HAL_EncoderPosition;

/**
 * Downcounters to supress noise from buttons
 */
uint16_t HAL_LeftButtonNoiseSuppressionCounter;
uint16_t HAL_EncoderButtonNoiseSuppressionCounter;
uint16_t HAL_RightButtonNoiseSuppressionCounter;

/**
 * True, if display bus initialized
 */
bool HAL_IsDisplayBusInitialized;

/**
 * Callback, called when left button pressed
 */
void (*HAL_LeftButtonCallback)(void);

/**
 * Callback, called when right button pressed
 */
void (*HAL_RightButtonCallback)(void);

/**
 * Callback, called when encoder button pressed
 */
void (*HAL_EncoderButtonCallback)(void);

/**
 * Callback, called when encoder rotated
 */
void (*HAL_EncoderRotationCallback)(int8_t direction);

/**
 * True if we are in economy mode
 */
bool HAL_IsInEconomyMode;

/**
 * True if UBatt measurement circuit is on
 */
bool HAL_IsUBattCheckOn;

/**
 * Call from ADC interrupt handler to add measurement
 */
void HAL_AddNewADCMeasurement(uint16_t measurement);

/**
 * Called when left button is pressed
 */
void HAL_OnLeftButtonPressed(void);

/**
 * Called when right button is pressed
 */
void HAL_OnRightButtonPressed(void);

/**
 * Called when encoder button is pressed
 */
void HAL_OnEncoderButtonPressed(void);

/**
 * Called when encoder turned
 */
void HAL_OnEncoderTurned(void);

#endif /* INCLUDE_HALPRIVATE_H_ */
