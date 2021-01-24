/*
 * hal.c
 *
 *  Created on: Jan 17, 2021
 *      Author: fossa
 */

#include <hal.h>

void HalInitHardware(void)
{
	HalInitPll(1);
	HalSetupADC();
	HalInitAntennaMatching();
}

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
	adcChannelConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

	if (HAL_ADC_ConfigChannel(&ADCHandle, &adcChannelConfig) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	HAL_ADC_Start_IT(&ADCHandle);
}

/**
 * ADC callback
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	if (AdcHandle != &ADCHandle)
	{
		return;
	}

	uint16_t currentLevel = HAL_ADC_GetValue(&ADCHandle);

	HalDetectorAccumulator += currentLevel;
	HalDetectorAveragingCounter ++;

	if (HAL_DETECTOR_AVERAGING == HalDetectorAveragingCounter)
	{
		uint16_t detectorValue = (uint16_t)((float)HalDetectorAccumulator / (float)HAL_DETECTOR_AVERAGING);
		HalDetectorAveragingCounter = 0;
		HalDetectorAccumulator = 0;

		HalOnDetectorValueUpdated(detectorValue);
	}
}

void HalOnDetectorValueUpdated(uint16_t detectorValue)
{
	HalDetectorAverage = detectorValue;
}

void HalInitAntennaMatching()
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void HalSetAntennaMatching(uint8_t matching)
{
	uint16_t tmp = ((uint16_t)matching) << 10;
	GPIOB->ODR = tmp & 0b0111110000000000;
}
