/*
 * l2hal_adf4001_private.h
 *
 *  Created on: Dec 6, 2020
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_PRIVATE_H_
#define L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_PRIVATE_H_

/**
 * Private stuff for ADF4001 PLL
 */

#include <stdint.h>

/**
 * Send this bytes to device. Most significant bit of most significant byte first.
 */
typedef struct
{
	uint8_t Most;
	uint8_t Middle;
	uint8_t Least;
}
L2HAL_ADF4001_BytesToSendStruct;



#endif /* L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_PRIVATE_H_ */
