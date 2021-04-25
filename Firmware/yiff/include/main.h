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

/**
 * EEPROM-related stuff
 */
#define YHL_EEPROM_ADDRESS 0xA0
#define YHL_EEPROM_PAGE_SIZE 8


#endif /* __MAIN_H */
