/*
 * HALPrivate.h
 *
 *  Created on: Jul 3, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_HALPRIVATE_H_
#define INCLUDE_HALPRIVATE_H_

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
float HAL_CurrentU80mCode;

/**
 * Averages counter
 */
volatile uint16_t HAL_ADCAveragesCounter;

/**
 * Current ADC channel
 */
volatile YHL_HAL_ADCChannelEnum HAL_CurrentADCChannel;

/**
 * Call from ADC interrupt handler to add measurement
 */
void HAL_AddNewADCMeasurement(uint16_t measurement);

#endif /* INCLUDE_HALPRIVATE_H_ */
