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
 * @brief Polling buttons level 2 HAL driver (private stuff).
 */

#ifndef L2HAL_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_BUTTONS_PRIVATE_H_
#define L2HAL_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_BUTTONS_PRIVATE_H_

#include <l2hal_aux.h>
#include <l2hal_buttons.h>
#include <l2hal_systick.h>


/**
 * Mask data, read from IDR with this mask.
 */
#define L2HAL_BUTTONS_PORT_DATA_MASK 0xFFFFU

/**
 * Metacontext -united context for all button driver instances.
 */
typedef struct
{
	/**
	 * False until first driver initialization, true after it.
	 */
	bool IsInitialized;

	/**
	 * How many instances of button driver we have.
	 */
	uint8_t ContextsCount;

	/**
	 * Array of pointers to contexts.
	 */
	L2HAL_Buttons_ContextStruct** Contexts;

} L2HAL_Buttons_MetacontextStruct;

/**
 * Driver metacontext.
 */
L2HAL_Buttons_MetacontextStruct L2HAL_Buttons_Metacontext = { false, 0, NULL };



/**
 * Loads data from physical ports into context, blocking reading data during update.
 */
void L2HAL_Buttons_GetPortsData(L2HAL_Buttons_ContextStruct* context);

/**
 * Returns port index (see L2HAL_Buttons_IndexesToPorts to reverse conversion).
 */
uint8_t L2HAL_Buttons_GetPortIndex(GPIO_TypeDef* port);

/**
 * SysTick handler, registered during driver first instance initialization.
 */
void L2HAL_Buttons_SysTickHandler();


#endif /* L2HAL_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_BUTTONS_PRIVATE_H_ */
