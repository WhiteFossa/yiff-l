/*
 * l2hal_crc.h
 *
 *  Created on: Apr 22, 2021
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_INTERNAL_CRC_INCLUDE_L2HAL_CRC_H_
#define L2HAL_DRIVERS_INTERNAL_CRC_INCLUDE_L2HAL_CRC_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f1xx_hal.h>
#include <l2hal_errors.h>

/**
 * Maximal buffer size
 */
#define L2HAL_CRC_MAX_BUFFER_SIZE 128

/**
 * CRC context
 */
typedef struct
{
	/**
	 * CRC Handle
	 */
	CRC_HandleTypeDef Handle;
}
L2HAL_CRCContextStruct;

/**
 * Initialize CRC calculator and return context.
 */
L2HAL_CRCContextStruct L2HAL_CRC_Init(void);

/**
 * Calculates CRC for the buffer of arbitrary size in bytes (i.e. alignment by 32bits is not necessary).
 * If buffer is aligned, then just calculate CRC, otherwise copies data to temporary aligned buffer and
 * calculate CRC over it.
 *
 * @param context Pointer to context
 * @param buffer Pointer to data buffer
 * @param size Data buffer size in BYTES
 * @return Calculated CRC
 */
uint32_t L2HAL_CRC_Calculate(L2HAL_CRCContextStruct* context, uint8_t* buffer, uint32_t size);

#endif /* L2HAL_DRIVERS_INTERNAL_CRC_INCLUDE_L2HAL_CRC_H_ */
