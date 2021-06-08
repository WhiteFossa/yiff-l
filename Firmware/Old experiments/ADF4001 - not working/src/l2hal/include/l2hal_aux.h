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
 * @brief Auxiliary stuff for level 2 HAL.
 */

#ifndef L2HAL_INCLUDE_L2HAL_AUX_H_
#define L2HAL_INCLUDE_L2HAL_AUX_H_

#include <stddef.h>
#include <stdint.h>

/**
 * Boolean type and values.
 */
#define bool uint8_t
#define true 0xFF
#define false 0x00

/**
 * Macroses for minimal and maximal values.
 */
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/************************
 * Memory-related stuff *
 ************************/

/**
 * Frees memory and set pointer to NULL.
 * @param ptr Pointer, pointing to memory to be freed.
 */
void AUX_Mem_SafeFree(void* ptr);


/*************************
 * Strings-related stuff *
 *************************/

/**
 * Returns substring of string. Returned value points to newly-allocated memory, so don't forget to free it.
 * @param string Pointer to string.
 * @param start First substring character index in string.
 * @param length Substring length.
 * @return Pointer to substring (points to newly-allocated memory).
 */
char* AUX_Str_Substring(char* string, uint16_t start, uint16_t length);

/**
 * Merges two strings into one, allocating memory for it. Don't forget to free result memory.
 * @param str1 Pointer to first string.
 * @param str2 Pointer to second string
 * @return Pointer to newly-allocated memory, containing merged string.
 */
char* AUX_Str_MergeStrings(char* str1, char* str2);


#endif /* L2HAL_INCLUDE_L2HAL_AUX_H_ */
