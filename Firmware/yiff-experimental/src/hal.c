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
