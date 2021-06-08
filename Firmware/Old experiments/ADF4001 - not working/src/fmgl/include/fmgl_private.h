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

/**
 * @file
 * @brief Internal Fossa's MCU Graphics Library stuff.
 */

#ifndef FMGL_INCLUDE_FMGL_PRIVATE_H_
#define FMGL_INCLUDE_FMGL_PRIVATE_H_

#include <l2hal_aux.h>
#include <fmgl.h>

/**
 * To avoid magic 8 in code.
 */
#define FMGL_PRIV_BITS_PER_BYTE 8

/**
 * For internal use in FMGL_RenderXBM.
 */
#define FMGL_PRIV_XBM_ACTIVE_COLOR_INDEX 0
#define FMGL_PRIV_XBM_INACTIVE_COLOR_INDEX 1
#define FMGL_PRIV_XBM_COLORS_NUMBER 2


/**
 * Returns true if pixel at given coordinates is active, false otherwise.
 * If coordinates are out of image, then result is undefined.
 * @param image Pointer to XBM image struct.
 * @param x Pixel X coordinate.
 * @param y Pixel Y coordinate.
 * @return True if pixel at given coordinates is active.
 */
bool FMGL_Priv_IsActiveXBMPixel(FMGL_API_XBMImage* image, uint16_t x, uint16_t y);

/**
 * Draws one character at given position.
 * @param context Pointer to FMGL library context.
 * @param fontSettings Pointer to font settings.
 * @param x Character top left corner X position.
 * @param y Character top left corner Y position.
 * @param character Character to draw.
 */
void FMGL_Priv_RenderCharacter(FMGL_API_DriverContext* context, FMGL_API_FontSettings* fontSettings, uint16_t x, uint16_t y, char character);

/**
 * For internal use in FMGL_RenderTextWithLineBreaks().
 * @param context Pointer to FMGL library context.
 * @param fontSettings Pointer to font settings.
 * @param startPos Start position in string from what substring to render begins.
 * @param length Substring to render length.
 * @param x X coordinate of substring top left pixel.
 * @param y Pointer to Y coordinate of top substring top left pixel. Will be incremented to scaledLineHeight.
 * @param scaledLineHeight Scaled line height (could be calculated, but have to be provided to improve speed).
 * @param width Pointer to variable, where width (in pixels) is stored. If substring width is higher than width, then width will be updated
 * to substring width.
 * @param isDryRun If true, then doesn't draw anything, just calculating width.
 * @param string String, from what substring will be rendered.
 */
void FMGL_Priv_RenderSubstring(FMGL_API_DriverContext* context, FMGL_API_FontSettings* fontSettings, uint16_t startPos, uint16_t length,
		uint16_t x, uint16_t* y, uint16_t scaledLineHeight, uint16_t* width, bool isDryRun, char* string);

#endif /* FMGL_INCLUDE_FMGL_PRIVATE_H_ */
