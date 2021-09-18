/*
	This file is part of Fossa's MCU Graphics Library (FMGL).

	FMGL is free software:
	you can redistribute it and/or modify it under the terms of the
	GNU General Public License as published by the Free Software
	Foundation, either version 3 of the License, or (at your option)
	any later version.

	FMGL is distributed in the hope
	that it will be useful, but WITHOUT ANY WARRANTY; without even
	the implied warranty of MERCHANTABILITY or FITNESS FOR A
	PARTICULAR PURPOSE.  See the GNU General Public License for more
	details.

	You should have received a copy of the GNU General Public License
	along with FMGL.

	If not, see <http://www.gnu.org/licenses/>.

	-------------------------------------------------------------------------

	Created by White Fossa, also known as Artyom Vetrov.

	Feel free to contact: whitefossa@gmail.com

	Repository: https://github.com/WhiteFossa/stm32-l2hal

	If repository above is not available, try my LiveJournal:
	fossa-white.livejournal.com

	or as last measure try to search for #WhiteFossa hashtag.

	-------------------------------------------------------------------------
 */

#ifndef __MAIN_H
#define __MAIN_H

#include <l2hal.h>
#include <StatusDisplay.h>
#include <Buttons.h>
#include <RTC.h>
#include <GlobalStateMachine.h>
#include <CycleStateMachine.h>
#include <HAL.h>
#include <HardwareLogic.h>
#include <MorsePlayer.h>
#include <math.h>
#include <EEPROM.h>
#include <UART.h>
#include <PacketsProcessor.h>
#include <terminusRegular12.h>
#include <printf.h>

/**
 * EEPROM-related stuff
 */
#define YHL_EEPROM_ADDRESS 0xA0
#define YHL_EEPROM_PAGE_SIZE 8

extern I2C_HandleTypeDef I2C_Display;
extern I2C_HandleTypeDef I2C_Other;
extern L2HAL_SSD1327_ContextStruct L2HAL_SSD1327_Context;
extern L2HAL_24x_ContextStruct EEPROMContext;
extern L2HAL_CRCContextStruct CRC_Context;
extern UART_HandleTypeDef UART_Handle;
extern L2HAL_HC06_ContextStruct HC06_Context;
extern FoxStateStruct FoxState;
extern PendingCommandsFlagsStruct PendingCommandsFlags;
extern uint8_t SwitchToThisProfileId;
extern char SetThisProfileName[YHL_PROFILE_NAME_MEMORY_SIZE];
extern HardwareControlsEventsStruct HardwareControlsEvents;

/**
 * Checks if fox name changed and if so, then saves it into EEPROM and bluetooth module
 */
void Main_ProcessFoxNameChange(void);

/**
 * Checks if we need to add new profile and if so adds it.
 */
void Main_ProcessNewProfileAdd(void);

/**
 * Checks if we need to switch profile and if so - does it
 */
void Main_ProcessProfileSwitch(void);

/**
 * Checks if we need to set profile name and sets it if needed
 */
void Main_ProcessSetProfileName(void);

/**
 * Checks if we need to set frequency and sets it if needed
 */
void Main_ProcessSetFrequency(void);

/**
 * Updates fox code if we need to update it
 */
void Main_ProcessSetCode(void);

/**
 * Updates fox speed if needed
 */
void Main_ProcessSetSpeed(void);

/**
 * Updates fox cycle if needed
 */
void Main_ProcessSetCycle(void);

/**
 * Updates ending tone duration if needed
 */
void Main_ProcessSetEndingToneDuration(void);

/**
 * Updates start/end times if needed
 */
void Main_ProcessSetBeginAndEndTimes(void);

/**
 * Updates fox power if needed
 */
void Main_ProcessSetPower(void);

/**
 * Flushes current profile changes into EEPROM if needed
 */
void Main_FlushProfileToEEPROM(void);

/**
 * Measures battery level and store it into fox state.
 */
void Main_MeasureBatteryLevel(void);

/**
 * Armes fox if needed
 */
void Main_ProcessFoxArming(void);

/**
 * Prepares fox for activation and matches antenna (if needed)
 */
void Main_PrepareAndMatchAntenna(void);

/**
 * Left button pressed event
 */
void Main_OnLeftButtonPressedInterrupt(void);
void Main_CheckLeftButtonPressedEvent(void);
void Main_OnLeftButtonPressed(void);

/**
 * Right button pressed event
 */
void Main_OnRightButtonPressedInterrupt(void);
void Main_CheckRightButtonPressedEvent(void);
void Main_OnRightButtonPressed(void);

/**
 * Encoder button pressed event
 */
void Main_OnEncoderButtonPressedInterrupt(void);
void Main_CheckEncoderButtonPressedEvent(void);
void Main_OnEncoderButtonPressed(void);

/**
 * Encoder rotation event
 */
void Main_OnEncoderRotationInterrupt(int8_t direction);
void Main_CheckEncoderRotationEvent(void);
void Main_OnEncoderRotation(int8_t direction);

/**
 * Call this to enter menu mode
 */
void Main_EnterMenu(void);

#endif /* __MAIN_H */
