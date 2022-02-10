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

#include <stdbool.h>

/**
 * Place ISRs here. Synchronize this value with FLASH section from mem.ld
 */
#define VTOR_ADDRESS 0x08004000

/**
 * Set default buttons actions
 */
void Main_SetDefaultButtonsActions(void);

/**
 * Checking for new UART packets and processes them
 */
void Main_ProcessIncomingPackets(void);

/**
 * Part of arming procedure
 */
void Main_PrepareFoxFoxTransmission(bool isArmFoxAfterMatching, bool forceTxAfterPreparation);

#endif /* __MAIN_H */
