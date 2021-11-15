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
#include <UART.h>
#include <Commands.h>
#include <EEPROM.h>

/*************************
 * Current date and time *
 *************************/
RTC_TimeTypeDef CurrentTime;
RTC_DateTypeDef CurrentDate;

/***************************
 * EEPROM structs go below *
 ***************************/
EEPROMConstantHeaderStruct EEPROM_ConstantHeader;
EEPROMHeaderStruct EEPROM_Header;
EEPROMProfileStruct EEPROM_CurrentProfile;

/****************************
* Hardware handles go below *
*****************************/

/**
 * SysTick driver context.
 */
L2HAL_SysTick_ContextStruct L2HAL_SysTick_Context = { 0 };

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

/**
 * ADC handle
 */
ADC_HandleTypeDef ADC_Handle;

/**
 * Context for 3.5MHz output stage power regulator
 */
L2HAL_AD5245_ContextStruct U80mRegulatorContext;

/**
 * SPI bus handle.
 */
SPI_HandleTypeDef SPIHandle;

/**
 * Frequency synthesizer context
 */
L2HAL_AD9835_ContextStruct SynthesizerContext;

/**
 * Tone timer handle
 */
TIM_HandleTypeDef ToneTimerHandle;

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

/**
 * Transparency mode (for fonts)
 */
FMGL_API_XBMTransparencyMode transparencyMode;


/***********************
* Fox stuff goes below *
************************/

/**
 * Fox state
 */
FoxStateStruct FoxState;

/**
 * Buttons text
 */
FoxButtonStruct LeftButton;
FoxButtonStruct RightButton;

/**
 * Events from hardware controls
 */
HardwareControlsEventsStruct HardwareControlsEvents;


/********************
* Commands go below *
*********************/

/**
 * Flags, set when commands are pending
 */
PendingCommandsFlagsStruct PendingCommandsFlags;

/**
 * When SwitchToProfile command issued, take new profile ID from this variable
 */
uint8_t SwitchToThisProfileId;

/**
 * When set profile name command issued, take new profile name from this variable
 */
char SetThisProfileName[YHL_PROFILE_NAME_MEMORY_SIZE];

#endif /* INCLUDE_GLOBALVARIABLES_H_ */
