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
#include <EEPROM.h>

/*****************************
* Hardware handlers go below *
******************************/

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

/**
 * I2C bus handle (connected to display).
 */
I2C_HandleTypeDef I2C_Display;

/**
 * Display driver context.
 */
L2HAL_SSD1327_ContextStruct L2HAL_SSD1327_Context;

/**
 * CRC calculator context
 */
L2HAL_CRCContextStruct CRC_Context;

/**
 * EEPROM context
 */
L2HAL_24x_ContextStruct EEPROMContext;

/****************************
* Graphics stuff goes below *
*****************************/

/**
 * Font to use
 */
FMGL_API_Font FMGL_FontTerminusRegular12;

/**
 * FMGL context.
 */
FMGL_API_DriverContext fmglContext;

/**
 * Common font settings.
 */
FMGL_API_FontSettings commonFont;

/**
 * Off color.
 */
FMGL_API_ColorStruct OffColor;

/**
 * On color.
 */
FMGL_API_ColorStruct OnColor;

/**
 * Transparency mode (for fonts)
 */
FMGL_API_XBMTransparencyMode transparencyMode;


/**************************
* EEPROM stuff goes below *
***************************/

/**
 * EEPROM constant header
 */
EEPROMConstantHeaderStruct EEPROM_ConstantHeader;

#endif /* INCLUDE_GLOBALVARIABLES_H_ */
