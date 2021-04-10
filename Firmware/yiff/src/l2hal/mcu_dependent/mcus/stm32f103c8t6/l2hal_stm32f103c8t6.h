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
 * @brief MCU-specific stuff for STM32F103C8T6.
 */

#ifndef L2HAL_INCLUDE_DEVICE_DEPENDENT_DEVICES_L2HAL_STM32F103C8T6_H_
#define L2HAL_INCLUDE_DEVICE_DEPENDENT_DEVICES_L2HAL_STM32F103C8T6_H_

#include <stm32f1xx_hal.h>
#include <mcus/stm32f103c8t6/drivers/input/buttons/include/l2hal_stm32f103c8t6_buttons.h>

/**
 * Maximal ports count for given device. A-D for STM32F103C8T6
 */
#define L2HAL_MAX_PORTS_COUNT 4

/**
 * Indexes to ports, i.e. 0 -> GPIOA, 1 -> GPIOB etc.
 */
static const GPIO_TypeDef* L2HAL_IndexesToPorts[L2HAL_MAX_PORTS_COUNT] = { GPIOA, GPIOB, GPIOC, GPIOD };


#endif /* L2HAL_INCLUDE_DEVICE_DEPENDENT_DEVICES_L2HAL_STM32F103C8T6_H_ */
