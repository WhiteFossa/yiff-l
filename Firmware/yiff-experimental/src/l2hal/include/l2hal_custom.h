/*
	This file is part of Fossa's STM32 level 2 HAL.

	STM32 level 2 HAL is free software:
	you can redistribute it and/or modify it under the terms of the
	GNU General Public License as published by the Free Software
	Foundation, either version 3 of the License, or (at your option)
	any later version.

	STM32 level 2 HAL is distributed in the hope
	that it will be useful, but WITHOUT ANY WARRANTY; without even
	the implied warranty of MERCHANTABILITY or FITNESS FOR A
	PARTICULAR PURPOSE.  See the GNU General Public License for more
	details.

	You should have received a copy of the GNU General Public License
	along with STM32 level 2 HAL.

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

/**
 * @file
 * @biref File for custom hardware-related stuff.
 */

#ifndef L2HAL_INCLUDE_L2HAL_CUSTOM_H_
#define L2HAL_INCLUDE_L2HAL_CUSTOM_H_
#include <l2hal_ssd1306.h>
#include <l2hal_buttons.h>
#include <l2hal_encoders.h>
#include <l2hal_ad9835.h>

/**
 * I2C1 interrupt priorities.
 */
#define I2C1_ER_IRQN_PRIORITY 0
#define I2C1_ER_IRQN_SUBPRIORITY 1

#define I2C1_EV_IRQN_PRIORITY 0
#define I2C1_EV_IRQN_SUBPRIORITY 2

/**
 * I2C bus handle.
 */
I2C_HandleTypeDef I2CHandle;

/**
 * SPI bus handle.
 */
SPI_HandleTypeDef SPIHandle;

/**
 * Display driver context.
 */
L2HAL_SSD1306_ContextStruct L2HAL_SSD1306_Context;

/**
 * Buttons driver context.
 */
L2HAL_Buttons_ContextStruct L2HAL_Buttons_Context;

/**
 * Encoders driver context.
 */
L2HAL_Encoders_ContextStruct L2HAL_Encoders_Context;

/**
 * DDS context.
 */
L2HAL_AD9835_ContextStruct L2HAL_AD9835_Context;

/**
 * Put custom hardware initialization stuff here,
 */
void L2HAL_InitCustomHardware(void);

/**
 * Settig up I2C for display.
 */
void L2HAL_SetupI2C(void);

/**
 * Setting up
 */
void L2HAL_SetupSPI(void);

/**
 * Setting up buttons driver.
 */
void L2HAL_SetupButtons(void);

/**
 * Setting up encoders driver.
 */
void L2HAL_SetupEncoders(void);

/**
 * I2C init.
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);

/**
 * I2C shutdown.
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c);

/**
 * I2C interrput handlers.
 */
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c);

/**
 * SPI init
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);

/**
 * SPI de-init
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);

#endif /* L2HAL_INCLUDE_L2HAL_CUSTOM_H_ */
