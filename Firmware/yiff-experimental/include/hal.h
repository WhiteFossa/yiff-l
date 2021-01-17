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
void HalSetupADC();

#endif /* INCLUDE_HAL_H_ */
