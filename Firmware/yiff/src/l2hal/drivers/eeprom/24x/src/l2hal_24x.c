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

void L2HAL_24x_ReadData(L2HAL_24x_ContextStruct* context, uint16_t address, uint8_t* destination, uint16_t size)
{
	if (HAL_OK != HAL_I2C_Mem_Read(context->I2CHandle,
			context->BusAddress,
			address,
			context->MemAddressSize,
			destination,
			size,
			L2HAL_24X_BUS_TIMEOUT))
	{
		L2HAL_Error(Generic);
	}
}

void L2HAL_24x_WriteData(L2HAL_24x_ContextStruct* context, uint16_t address, uint8_t* source, uint16_t size)
{
	uint16_t remainToWrite = size;
	uint16_t startAddress = address;
	uint8_t* sourcePartPtr = source;

	while(remainToWrite > 0)
	{
		uint16_t writeSize;
		if (remainToWrite >= context->WritePageSize)
		{
			writeSize = context->WritePageSize;
		}
		else
		{
			writeSize = remainToWrite;
		}

		if (HAL_OK != HAL_I2C_Mem_Write(context->I2CHandle,
				context->BusAddress,
				startAddress,
				context->MemAddressSize,
				sourcePartPtr,
				writeSize,
				L2HAL_24X_BUS_TIMEOUT))
		{
			L2HAL_Error(Generic);
		}

		remainToWrite = (uint16_t)(remainToWrite - writeSize);
		startAddress = (uint16_t)(startAddress + writeSize);
		sourcePartPtr += writeSize;


		/* Time to write data */
		HAL_Delay(L2HAL_24X_INTERWRITE_PAUSE);
	}
}
