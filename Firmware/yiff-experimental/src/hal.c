/*
 * hal.c
 *
 *  Created on: Jan 17, 2021
 *      Author: fossa
 */

#include <hal.h>

void HalInitPll(uint32_t frequency)
{
	L2HAL_AD9835_Context.SPIHandle = &SPIHandle;
	L2HAL_AD9835_Context.FSYNCPort = GPIOA;
	L2HAL_AD9835_Context.FSYNCPin = GPIO_PIN_8;
	L2HAL_AD9835_Init(&L2HAL_AD9835_Context);

	L2HAL_AD9835_WriteFrequencyWord(&L2HAL_AD9835_Context, Freg0, frequency);
}

void HalSetPllFrequency(uint32_t frequency)
{
	L2HAL_AD9835_WriteFrequencyWord(&L2HAL_AD9835_Context, Freg0, frequency);
}

void HalSetupADC()
{
	ADCHandle.Instance = ADC1;

	ADCHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	ADCHandle.Init.ScanConvMode          = ADC_SCAN_DISABLE;
	ADCHandle.Init.ContinuousConvMode    = ENABLE;
	ADCHandle.Init.NbrOfConversion       = 1;
	ADCHandle.Init.DiscontinuousConvMode = DISABLE;
	ADCHandle.Init.NbrOfDiscConversion   = 1;
	ADCHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;

	if (HAL_ADC_Init(&ADCHandle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	ADC_ChannelConfTypeDef adcChannelConfig;
	adcChannelConfig.Channel = ADC_CHANNEL_3;
	adcChannelConfig.Rank = ADC_REGULAR_RANK_1;
	adcChannelConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;

	if (HAL_ADC_ConfigChannel(&ADCHandle, &adcChannelConfig) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	HAL_ADC_Start_IT(&ADCHandle);
}
