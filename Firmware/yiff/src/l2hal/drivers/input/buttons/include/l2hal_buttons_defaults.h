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
 * @brief Defaults, related to L2HAL polling buttons driver.
 */

#ifndef L2HAL_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_BUTTONS_DEFAULTS_H_
#define L2HAL_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_BUTTONS_DEFAULTS_H_

/**
 * Pin will be initialized in this mode by L2HAL_Buttons_DefaultPinInitializer().
 */
#define L2HAL_BUTTONS_DEFAULT_PIN_MODE GPIO_MODE_INPUT

/**
 * Pin will be initialized in this pull mode by L2HAL_Buttons_DefaultPinInitializer().
 */
#define L2HAL_BUTTONS_DEFAULT_PIN_PULL_MODE GPIO_PULLUP

#endif /* L2HAL_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_BUTTONS_DEFAULTS_H_ */
