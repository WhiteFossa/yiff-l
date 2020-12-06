/*
 * l2hal_adf4001_defines.h
 *
 *  Created on: Dec 6, 2020
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_DEFINES_H_
#define L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_DEFINES_H_

/**
 * Various bits defines for ADF4001
 */

/**
 * Command bits (define what register is addressed)
 */
#define L2HAL_ADF4001_CONTROLBITS_R 0b00
#define L2HAL_ADF4001_CONTROLBITS_N 0b01
#define L2HAL_ADF4001_CONTROLBITS_FUNCTION 0b10
#define L2HAL_ADF4001_CONTROLBITS_INITIALIZATION 0b11

/**
 * Antibacklash pulse width (in nanoseconds).
 */
enum L2HAL_ADF4001_AntiBacklashWidth
{
	PulseWidth2_9,
	PulseWidth1_3,
	PulseWidth6_0,
};

/**
 * Positions of antibacklash width bits in most significant byte
 */
#define L2HAL_ADF4001_ANTIBACKLASH0 0
#define L2HAL_ADF4001_ANTIBACKLASH1 1


/**
 * Lock detect precision (in cycles)
 */
enum L2HAL_ADF4001_LockDetectPrecision
{
	Cycles3,
	Cycles5
};

/**
 * Position of lock detect precision bit in most significant byte
 */
#define L2HAL_ADF4001_LOCK_DETECT_PRECISION 4

#endif /* L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_DEFINES_H_ */
