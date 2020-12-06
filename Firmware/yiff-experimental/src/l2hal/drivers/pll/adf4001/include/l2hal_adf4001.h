/*
 * l2hal_adf4001.h
 *
 *  Created on: Dec 6, 2020
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_H_
#define L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_H_

#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <l2hal_adf4001_defines.h>

/**
 * PLL context, SPI connection and various stuff is stored here
 */
typedef struct
{

}
L2HAL_ADF4001_ContextStruct;

/**
 * Fill this struct to generate reference counter latch content.
 */
typedef struct
{
	 /**
	  * Refence counter value
	  */
	uint16_t ReferenceCounter;

	/**
	 * Anti-backlash width.
	 */
	enum L2HAL_ADF4001_AntiBacklashWidth AntiBacklashWidth;

	/**
	 * Lock detect precision.
	 */
	enum L2HAL_ADF4001_LockDetectPrecision LockDetectPrecision;
}
L2HAL_ADF4001_ReferenceCounterStruct;


/**
 * Write reference counter data into PLL
 */
uint8_t L2HAL_ADF4001_WriteReferenceCounter(L2HAL_ADF4001_ReferenceCounterStruct* data);

#endif /* L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_H_ */
