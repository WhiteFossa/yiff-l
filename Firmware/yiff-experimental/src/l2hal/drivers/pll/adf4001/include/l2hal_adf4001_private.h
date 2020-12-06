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
	uint8_t most;
	uint8_t middle;
	uint8_t least;
}
L2HAL_ADF4001_BytesToSendStruct;

/**
 * Writes bytes to device
 */
void WriteToPll(L2HAL_ADF4001_BytesToSendStruct bytes);

#endif /* L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_PRIVATE_H_ */
