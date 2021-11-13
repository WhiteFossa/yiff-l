/*
 * l2hal_crc.c
 *
 *  Created on: Apr 22, 2021
 *      Author: fossa
 */
#include <l2hal_crc.h>

L2HAL_CRCContextStruct L2HAL_CRC_Init(void)
{
	L2HAL_CRCContextStruct context = { 0 };
	context.Handle.Instance = CRC;

	if (HAL_CRC_Init(&context.Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	return context;
}

uint32_t L2HAL_CRC_Calculate(L2HAL_CRCContextStruct* context, uint8_t* buffer, uint32_t size)
{
	if (0 == size % 4)
	{
		// Buffer is aligned, just calculating
		return HAL_CRC_Calculate(&context->Handle, (uint32_t*)buffer, size / 4);
	}
	else
	{
		uint32_t sizeIn32 = size / 4 + 1;
		uint32_t sizeIn8 = sizeIn32 * 4;

		uint32_t* alignedBuffer = malloc(sizeIn8);
		memset(alignedBuffer, 0x00, sizeIn8);
		memcpy(alignedBuffer, buffer, size);

		volatile uint32_t result = HAL_CRC_Calculate(&context->Handle, alignedBuffer, sizeIn32);

		free(alignedBuffer);

		return result;
	}
}
