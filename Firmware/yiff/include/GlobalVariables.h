/*
 * GlobalVariables.h
 *
 *  Created on: May 30, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_GLOBALVARIABLES_H_
#define INCLUDE_GLOBALVARIABLES_H_

#include <FoxStatus.h>
#include <Buttons.h>
#include <MorsePlayer.h>
#include <EEPROM.h>
#include <UART.h>


/******************************
* Hardware handles goes below *
*******************************/

/**
 * I2C bus handle (connected to display).
 */
I2C_HandleTypeDef I2C_Display;

/**
 * I2C bus handle (connected to all other devices).
 */
I2C_HandleTypeDef I2C_Other;

/**
 * Display driver context.
 */
L2HAL_SSD1327_ContextStruct L2HAL_SSD1327_Context;

/**
 * EEPROM context
 */
L2HAL_24x_ContextStruct EEPROMContext;

/**
 * CRC calculator context
 */
L2HAL_CRCContextStruct CRC_Context;

/**
 * UART Handle
 */
UART_HandleTypeDef UART_Handle;

/**
 * HC-06 context
 */
L2HAL_HC06_ContextStruct HC06_Context;


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
 * Inverted common font settings.
 */
FMGL_API_FontSettings invertedCommonFont;

/**
 * Big font for displaying frequency settings.
 */
FMGL_API_FontSettings frequencyFont;

/**
 * Off color.
 */
FMGL_API_ColorStruct OffColor;

/**
 * On color.
 */
FMGL_API_ColorStruct OnColor;


/***********************
* Fox stuff goes below *
************************/

/**
 * Fox state
 */
FoxStateStruct FoxState;

/**
 * If true, then fox name just changed, we need to write it to EEPROM from the main thread
 */
bool FoxStateNameChanged;

/**
 * Buttons text
 */
FoxButtonStruct LeftButton;
FoxButtonStruct RightButton;

/**
 * EEPROM constant header in its actual state
 */
EEPROMConstantHeaderStruct EEPROM_ConstantHeader;

/**
 * EEPROM header in its actual state
 */
EEPROMHeaderStruct EEPROM_Header;

/**
 * Current profile
 */
EEPROMProfileStruct EEPROM_CurrentProfile;


#endif /* INCLUDE_GLOBALVARIABLES_H_ */