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
#include <l2hal_ssd1327.h>
#include <l2hal_buttons.h>
#include <l2hal_encoders.h>
#include <l2hal_24x.h>
#include <l2hal_crc.h>
#include <l2hal_hc06.h>

/**
 * I2C1 interrupt priorities.
 */
#define I2C1_ER_IRQN_PRIORITY 0
#define I2C1_ER_IRQN_SUBPRIORITY 1

#define I2C1_EV_IRQN_PRIORITY 0
#define I2C1_EV_IRQN_SUBPRIORITY 2

/**
 * I2C2 interrupt priorities.
 */
#define I2C2_ER_IRQN_PRIORITY 0
#define I2C2_ER_IRQN_SUBPRIORITY 1

#define I2C2_EV_IRQN_PRIORITY 0
#define I2C2_EV_IRQN_SUBPRIORITY 2

/**
 * Put custom hardware initialization stuff here,
 */
void L2HAL_InitCustomHardware(void);

/**
 * Settig up both I2C buses
 */
void L2HAL_SetupI2C(void);

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

void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);

/**
 * CRC calculator init and de-init
 */
void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc);
void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc);

/**
 * UART-related stuff
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);
void USART1_IRQHandler(void);

#endif /* L2HAL_INCLUDE_L2HAL_CUSTOM_H_ */
