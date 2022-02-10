/*
 * GlobalVariables.h
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_GLOBALVARIABLES_H_
#define INCLUDE_GLOBALVARIABLES_H_

#include <l2hal_systick.h>
#include <stm32f1xx_hal.h>

/**
 * SysTick driver context.
 */
L2HAL_SysTick_ContextStruct L2HAL_SysTick_Context = { 0 };

/**
 * I2C bus handle (connected to all other devices).
 */
I2C_HandleTypeDef I2C_Other;

/**
 * UART Handle
 */
UART_HandleTypeDef UART_Handle;

#endif /* INCLUDE_GLOBALVARIABLES_H_ */
