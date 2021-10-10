/*
 * l2hal_ad9835.c
 *
 *  Created on: Dec 20, 2020
 *      Author: fossa
 */

#include <l2hal_ad9835.h>

void L2HAL_AD9835_Init(L2HAL_AD9835_ContextStruct* context)
{
	/* Clocking port in */
	if (GPIOA == context->FSYNCPort)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (GPIOB == context->FSYNCPort)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (GPIOC == context->FSYNCPort)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (GPIOD == context->FSYNCPort)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin       = context->FSYNCPin;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(context->FSYNCPort, &GPIO_InitStruct);

	/* Resetting */
	HAL_GPIO_WritePin(context->FSYNCPort, context->FSYNCPin, GPIO_PIN_SET);
	HAL_Delay(1);
	L2HAL_AD9835_PowerControl(context, true	, true, true);
	HAL_Delay(1);
	L2HAL_AD9835_PowerControl(context, false, false, false);
	HAL_Delay(1);
}

void L2HAL_AD9835_SmallDelay()
{
	for (volatile uint32_t counter = 0; counter < L2HAL_AD9835_SMALLDELAY; counter ++)
	{
	}
}

void L2HAL_AD9835_WriteToDDS(L2HAL_AD9835_ContextStruct* context, L2HAL_AD9835_DataStruct* data)
{
	HAL_GPIO_WritePin(context->FSYNCPort, context->FSYNCPin, GPIO_PIN_RESET);
	L2HAL_AD9835_SmallDelay();

	uint8_t buffer[2];
	buffer[0] = data->Msb;
	buffer[1] = data->Lsb;
	HAL_SPI_Transmit(context->SPIHandle, buffer, 2, 1000);

	L2HAL_AD9835_SmallDelay();
	HAL_GPIO_WritePin(context->FSYNCPort, context->FSYNCPin, GPIO_PIN_SET);

	L2HAL_AD9835_SmallDelay();
}

void L2HAL_AD9835_SelectPFSource(L2HAL_AD9835_ContextStruct* context, bool sync, bool useBits)
{
	L2HAL_AD9835_DataStruct data;
	data.Msb = (1 << 7); /* D15 = 1, D14 = 0 */
	data.Lsb = 0x00;

	if (sync)
	{
		data.Msb |= (1 << 5); /* D13 = FSYNC */
	}

	if (useBits)
	{
		data.Msb |= (1 << 4); /* D12 = SELSRC */
	}

	L2HAL_AD9835_WriteToDDS(context, &data);
}

void L2HAL_AD9835_PowerControl(L2HAL_AD9835_ContextStruct* context, bool sleep, bool reset, bool clear)
{
	L2HAL_AD9835_DataStruct data;
	data.Msb = (1 << 7) | (1 << 6); /* D15 = 1, D14 = 1 */
	data.Lsb = 0x00;

	if (sleep)
	{
		data.Msb |= (1 << 5); /* D13 = SLEEP */
	}

	if (reset)
	{
		data.Msb |= (1 << 4); /* D12 = RESET */
	}

	if (clear)
	{
		data.Msb |= (1 << 3); /* D11 = CLR */
	}

	L2HAL_AD9835_WriteToDDS(context, &data);
}

void L2HAL_AD9835_SelectPhaseRegister(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_PhaseRegisters phaseRegister)
{
	L2HAL_AD9835_DataStruct data;
	data.Msb = 0x00;
	data.Lsb = 0x00;

	data.Msb |= SelectPhaseReg;
	data.Msb |= phaseRegister;

	L2HAL_AD9835_WriteToDDS(context, &data);
}

void L2HAL_AD9835_SelectFrequencyRegister(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_FrequencyRegisters frequencyRegister)
{
	L2HAL_AD9835_DataStruct data;
	data.Msb = 0x00;
	data.Lsb = 0x00;

	data.Msb |= SelectFrequencyReg;
	data.Msb |= frequencyRegister;

	L2HAL_AD9835_WriteToDDS(context, &data);
}

void L2HAL_AD9835_SelectFrequencyAndPhaRegisters(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_FrequencyRegisters frequencyRegister,
		enum L2HAL_AD9835_PhaseRegisters phaseRegister)
{
	L2HAL_AD9835_DataStruct data;
	data.Msb = 0x00;
	data.Lsb = 0x00;

	data.Msb |= SelectFrequencyReg;
	data.Msb |= frequencyRegister;
	data.Msb |= phaseRegister;

	L2HAL_AD9835_WriteToDDS(context, &data);
}

void L2HAL_AD9835_Write8FrequencyBitsToDeferRegister(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_Registers frequencyRegister, uint8_t data)
{
	L2HAL_AD9835_DataStruct dataStruct;
	dataStruct.Msb = 0x00;
	dataStruct.Lsb = data;

	dataStruct.Msb |= Write8FrequencyBitsToDefer;
	dataStruct.Msb |= frequencyRegister;

	L2HAL_AD9835_WriteToDDS(context, &dataStruct);
}


void L2HAL_AD9835_Write16FrequencyBits(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_Registers frequencyRegister, uint8_t data)
{
	L2HAL_AD9835_DataStruct dataStruct;
	dataStruct.Msb = 0x00;
	dataStruct.Lsb = data;

	dataStruct.Msb |= Write16FrequencyBits;
	dataStruct.Msb |= frequencyRegister;

	L2HAL_AD9835_WriteToDDS(context, &dataStruct);
}

void L2HAL_AD9835_Write8PhaseBitsToDeferRegister(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_Registers phaseRegister, uint8_t data)
{
	L2HAL_AD9835_DataStruct dataStruct;
	dataStruct.Msb = 0x00;
	dataStruct.Lsb = data;

	dataStruct.Msb |= Write8PhaseBitsToDefer;
	dataStruct.Msb |= phaseRegister;

	L2HAL_AD9835_WriteToDDS(context, &dataStruct);
}

void L2HAL_AD9835_Write16PhaseBits(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_Registers phaseRegister, uint8_t data)
{
	L2HAL_AD9835_DataStruct dataStruct;
	dataStruct.Msb = 0x00;
	dataStruct.Lsb = data;

	dataStruct.Msb |= Write16PhaseBits;
	dataStruct.Msb |= phaseRegister;

	L2HAL_AD9835_WriteToDDS(context, &dataStruct);
}

void L2HAL_AD9835_WriteFrequencyWord(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_FrequencyRegisters frequencyRegister, uint32_t data)
{
	uint8_t byte3 = (uint8_t)((data & 0xFF000000UL) >> 24);
	uint8_t byte2 = (uint8_t)((data & 0x00FF0000UL) >> 16);
	uint8_t byte1 = (uint8_t)((data & 0x0000FF00UL) >> 8);
	uint8_t byte0 = (uint8_t)(data & 0x000000FFUL);

	switch (frequencyRegister)
	{
		case Freg0:
			L2HAL_AD9835_Write8FrequencyBitsToDeferRegister(context, Freg08HMSBs, byte3); /* 3rd byte */
			L2HAL_AD9835_Write16FrequencyBits(context, Freg08LMSBs, byte2); /* 2nd byte */
			L2HAL_AD9835_Write8FrequencyBitsToDeferRegister(context, Freg08HLSBs, byte1); /* 1st byte */
			L2HAL_AD9835_Write16FrequencyBits(context, Freg08LLSBs, byte0); /* 0th byte */
		break;
		case Freg1:
			L2HAL_AD9835_Write8FrequencyBitsToDeferRegister(context, Freg18HMSBs, byte3); /* 3rd byte */
			L2HAL_AD9835_Write16FrequencyBits(context, Freg18LMSBs, byte2); /* 2nd byte */
			L2HAL_AD9835_Write8FrequencyBitsToDeferRegister(context, Freg18HLSBs, byte1); /* 1st byte */
			L2HAL_AD9835_Write16FrequencyBits(context, Freg18LLSBs, byte0); /* 0th byte */
		break;
	}
}

void L2HAL_AD9835_WritePhaseWord(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_PhaseRegisters phaseRegister, uint16_t data)
{
	uint8_t byte1 = (uint8_t)((data & 0x0000FF00UL) >> 8);
	uint8_t byte0 = (uint8_t)(data & 0x000000FFUL);

	switch (phaseRegister)
	{
		case Phase0:
			L2HAL_AD9835_Write8PhaseBitsToDeferRegister(context, Phase08MSBs, byte1);
			L2HAL_AD9835_Write16PhaseBits(context, Phase08LSBs, byte0);
		break;
		case Phase1:
			L2HAL_AD9835_Write8PhaseBitsToDeferRegister(context, Phase18MSBs, byte1);
			L2HAL_AD9835_Write16PhaseBits(context, Phase18LSBs, byte0);
		break;
		case Phase2:
			L2HAL_AD9835_Write8PhaseBitsToDeferRegister(context, Phase28MSBs, byte1);
			L2HAL_AD9835_Write16PhaseBits(context, Phase28LSBs, byte0);
		break;
		case Phase3:
			L2HAL_AD9835_Write8PhaseBitsToDeferRegister(context, Phase38MSBs, byte1);
			L2HAL_AD9835_Write16PhaseBits(context, Phase38LSBs, byte0);
		break;
	}
}
