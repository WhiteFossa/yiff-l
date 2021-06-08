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

#include "terminusRegular12.h"

uint16_t FMGL_FontTerminusRegular12GetCharacterWidth (uint8_t character)
{
	return FMGL_FONT_TERMINUS_REGULAR_12_CHARACTER_WIDTH;
}

const uint8_t* FMGL_FontTerminusRegular12GetCharacterRaster(uint8_t character)
{
	if (character < FMGL_FONT_TERMINUS_REGULAR_12_FIRST_CHARACTER_CODE)
	{
		return FMGL_FontTerminusRegular12_WrongCharacterCode;
	}

	return FMGL_FontTerminusRegular12Characters[character - FMGL_FONT_TERMINUS_REGULAR_12_FIRST_CHARACTER_CODE];
}

FMGL_API_Font FMGL_FontTerminusRegular12Init(void)
{
	FMGL_FontTerminusRegular12.Height = FMGL_FONT_TERMINUS_REGULAR_12_CHARACTER_HEIGHT;
	FMGL_FontTerminusRegular12.GetCharacterWidth = &FMGL_FontTerminusRegular12GetCharacterWidth;
	FMGL_FontTerminusRegular12.GetCharacterRaster = &FMGL_FontTerminusRegular12GetCharacterRaster;

	return FMGL_FontTerminusRegular12;
}
