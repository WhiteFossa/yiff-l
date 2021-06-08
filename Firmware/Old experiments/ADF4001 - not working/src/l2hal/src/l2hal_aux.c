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

#include <l2hal_aux.h>

void AUX_Mem_SafeFree(void* ptr)
{
	free(ptr);
	ptr = NULL;
}

char* AUX_Str_Substring(char* string, uint16_t start, uint16_t length)
{
	char* result = malloc(length + 1);
	memcpy(result, string + start, length);
	result[length] = '\0';

	return result;
}

char* AUX_Str_MergeStrings(char* str1, char* str2)
{
	size_t needToAllocate = strlen(str1) + strlen(str2) + 1; /* +1 because of null-termination */
	char* result = malloc(needToAllocate);
	strcpy(result, str1);
	strcat(result, str2);

	return result;
}
