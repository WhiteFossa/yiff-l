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
 * Averages counter
 */
volatile uint16_t HAL_ADCAveragesCounter;

/**
 * Current ADC channel
 */
volatile HAL_ADCChannelEnum HAL_CurrentADCChannel;

/**
 * Call from ADC interrupt handler to add measurement
 */
void HAL_AddNewADCMeasurement(uint16_t measurement);


#endif /* INCLUDE_HALPRIVATE_H_ */