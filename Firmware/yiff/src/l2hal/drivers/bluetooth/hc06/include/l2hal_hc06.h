/*
 * hc06.h
 *
 *  Created on: May 3, 2021
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_BLUETOOTH_HC06_INCLUDE_L2HAL_HC06_H_
#define L2HAL_DRIVERS_BLUETOOTH_HC06_INCLUDE_L2HAL_HC06_H_

#include <stdint.h>
#include <string.h>
#include <stm32f1xx_hal.h>
#include <l2hal_aux.h>
#include <l2hal_errors.h>

/**
 * Timeout for blocking operations over UART
 */
#define L2HAL_HC06_UART_TIMEOUT 1000

/**
 * Maximal length of bluetooth name
 */
#define L2HAL_HC06_MAX_NAME_LENGTH 16

/**
 * Maximal memory size for bluetooth name
 */
#define L2HAL_HC06_MAX_NAME_MEMORY_SIZE (L2HAL_HC06_MAX_NAME_LENGTH + 1)

/**
 * PIN code length (4 characters + null-termination)
 */
#define L2HAL_HC06_PIN_CODE_LENGTH 5

/**
 * Driver context struct
 */
typedef struct
{
	/**
	 * True, if HC06 is connected to UART
	 */
	bool IsFound;

	/**
	 * Pointer to UART handle to what HC-06 is connected
	 */
	UART_HandleTypeDef* UART_Handle;
}
L2HAL_HC06_ContextStruct;


/**
 * Tries to attach to HC-06, connected to given UART. Issues "AT" command to test if device present or not
 */
L2HAL_HC06_ContextStruct L2HAL_HC06_AttachToDevice(UART_HandleTypeDef* uart);

/**
 * Set bluetooth name. Spaces are replaced with underscores
 */
void L2HAL_HC06_SetName(L2HAL_HC06_ContextStruct* context, const char* name);

/**
 * Set PIN code
 */
void L2HAL_HC06_SetPIN(L2HAL_HC06_ContextStruct* context, const char pin[L2HAL_HC06_PIN_CODE_LENGTH]);


#endif /* L2HAL_DRIVERS_BLUETOOTH_HC06_INCLUDE_L2HAL_HC06_H_ */
