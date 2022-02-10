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

#include <l2hal_24x.h>

L2HAL_24x_ContextStruct L2HAL_24x_DetectEepromAtAddress(I2C_HandleTypeDef* i2CHandle,
		uint8_t busAddress,
		bool isLongAddress,
		uint16_t writePageSize)
{
	L2HAL_24x_ContextStruct context;
	context.IsFound = false;

	if (HAL_OK == HAL_I2C_IsDeviceReady(i2CHandle, busAddress, 1, L2HAL_24X_BUS_TIMEOUT))
		{
			context.I2CHandle = i2CHandle;
			context.BusAddress = busAddress;
			context.IsFound = true;
			context.WritePageSize = writePageSize;

			if (isLongAddress)
			{
				context.MemAddressSize = I2C_MEMADD_SIZE_16BIT;
			}
			else
			{
				context.MemAddressSize = I2C_MEMADD_SIZE_8BIT;
			}
		}

	return context;
}

void L2HAL_24x_ReadData(L2HAL_24x_ContextStruct* context, uint16_t eepromAddress, uint8_t* destination, uint16_t size)
{
	if (HAL_OK != HAL_I2C_Mem_Read(context->I2CHandle,
			context->BusAddress,
			eepromAddress,
			context->MemAddressSize,
			destination,
			size,
			L2HAL_24X_BUS_TIMEOUT))
	{
		L2HAL_Error(Generic);
	}
}

uint16_t L2HAL_24x_GetPageNumber(L2HAL_24x_ContextStruct* context, uint16_t eepromAddress)
{
	return eepromAddress / context->WritePageSize;
}

uint16_t L2HAL_24x_GetPageStartAddress(L2HAL_24x_ContextStruct* context, uint16_t pageNumber)
{
	return context->WritePageSize * pageNumber;
}

uint16_t L2HAL_24x_GetPageEndAddress(L2HAL_24x_ContextStruct* context, uint16_t pageNumber)
{
	return L2HAL_24x_GetPageStartAddress(context, pageNumber + 1) - 1;
}

void L2HAL_24x_PageWrite(L2HAL_24x_ContextStruct* context, uint16_t eepromAddress, uint8_t* dataPtr, uint16_t size)
{
	if (0 == size)
	{
		L2HAL_Error(Generic);
	}

	uint16_t startPageNumber = L2HAL_24x_GetPageNumber(context, eepromAddress);
	uint16_t endPageNumber = L2HAL_24x_GetPageNumber(context, eepromAddress + size - 1);

	if (startPageNumber != endPageNumber)
	{
		/* Interpage write attempt */
		L2HAL_Error(Generic);
	}

	if (HAL_OK != HAL_I2C_Mem_Write(context->I2CHandle,
			context->BusAddress,
			eepromAddress,
			context->MemAddressSize,
			dataPtr,
			size,
			L2HAL_24X_BUS_TIMEOUT))
	{
		L2HAL_Error(Generic);
	}

	/* Time to write data */
	HAL_Delay(L2HAL_24X_INTERWRITE_PAUSE);
}

void L2HAL_24x_WriteData(L2HAL_24x_ContextStruct* context, uint16_t eepromAddress, uint8_t* source, uint16_t size)
{
	uint16_t currentEEPROMAddress = eepromAddress;
	uint16_t remainsToWrite = size;
	uint8_t* currentSourcePtr = source;

	while(remainsToWrite > 0)
	{
		uint16_t currentPageNumber = L2HAL_24x_GetPageNumber(context, currentEEPROMAddress);
		uint16_t endPageAddress = L2HAL_24x_GetPageEndAddress(context, currentPageNumber);

		uint16_t startAddressInPage = currentEEPROMAddress;

		uint16_t endAddressInPage;
		uint16_t writeAmount;
		if (startAddressInPage + remainsToWrite > endPageAddress)
		{
			/* Will be more pages, writing current page till the end */
			endAddressInPage = endPageAddress;
			writeAmount = endAddressInPage - startAddressInPage + 1;
		}
		else
		{
			/* Current page is the last one */
			endAddressInPage = startAddressInPage + remainsToWrite;
			writeAmount = remainsToWrite;
		}

		L2HAL_24x_PageWrite(context, startAddressInPage, currentSourcePtr, writeAmount);

		currentSourcePtr += writeAmount;
		currentEEPROMAddress += writeAmount;
		remainsToWrite -= writeAmount;
	}
}
