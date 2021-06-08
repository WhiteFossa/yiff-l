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
 * @brief Incremental encoder level 2 HAL driver (private stuff).
 */

#ifndef L2HAL_DRIVERS_INPUT_ENCODERS_INCLUDE_L2HAL_ENCODERS_PRIVATE_H_
#define L2HAL_DRIVERS_INPUT_ENCODERS_INCLUDE_L2HAL_ENCODERS_PRIVATE_H_

#include <l2hal_encoders.h>


/**
 * Driver attaches this function as callback handler to buttons, which are parts of encoder. See l2hal_buttons.h for callback parameters.
 */
void L2HAL_Encoders_ButtonsCallback(L2HAL_Buttons_ButtonStruct* button, GPIO_PinState newPinState, uint16_t* portsData);



#endif /* L2HAL_DRIVERS_INPUT_ENCODERS_INCLUDE_L2HAL_ENCODERS_PRIVATE_H_ */
