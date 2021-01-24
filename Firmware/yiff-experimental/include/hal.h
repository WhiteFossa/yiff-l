/*
 * Hardware abstractions layer
 */

#ifndef INCLUDE_HAL_H_
#define INCLUDE_HAL_H_

#include <stddef.h>
#include <stdint.h>
#include <l2hal.h>
#include <l2hal_ad9835.h>

/**
 * !!! HARDWARE CONNECTIONS !!!
 *
 * PA0 - Encoder 1
 * PA1 - Encoder 2
 * PA2 - Encoder button
 *
 * PA3 - Detector output (ADC12_IN3)
 *
 * PA5 - AD9835 SCLK
 * PA7 - AD9835 SDATA
 * PA8 - AD9835 FSYNC
 *
 *
 */

/**
 * Averaging over 256 detector measurements.
 */
#define HAL_DETECTOR_AVERAGING 10000

/**
 * Here we accumulating values from detector.
 */
uint32_t HalDetectorAccumulator;

/**
 * Counter for averaging
 */
uint16_t HalDetectorAveragingCounter;

/**
 * Averaged value of detector
 */
uint16_t HalDetectorAverage;

/**
 * True when detector value updated, user have to reset in manually.
 */
bool HalIsDetectorValueUpdated;

/**
 * Initialize hardware
 */
void HalInitHardware(void);

/**
 * Initialize PLL and set after initialization frequency.
 */
void HalInitPll(uint32_t frequency);

/**
 * Set PLL frequency.
 */
void HalSetPllFrequency(uint32_t frequency);

/**
 * Set up detector ADC
 */
void HalSetupADC(void);

/**
 * Initialize antenna matching unit
 */
void HalInitAntennaMatching(void);

/**
 * Set antenna matching level
 */
void HalSetAntennaMatching(uint8_t matching);

/**
 * Initialize manipulation output
 */
void HalInitManipulator(void);

/**
 * Switch manipulator.
 */
void HalSwitchManipulator(bool isOn);

/**
 * Called when detector value is updated.
 */
void HalOnDetectorValueUpdated(uint16_t detectorValue);

#endif /* INCLUDE_HAL_H_ */
