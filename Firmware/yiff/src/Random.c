/*
 * Random.c
 *
 *  Created on: Nov 11, 2021
 *      Author: fossa
 */

#include <Random.h>

void Rand_GatherEnthropy(void)
{
	bool currentUBattCheckState = HAL_GetUBattCheck();

	if (!currentUBattCheckState)
	{
		HAL_SwitchUBattCheck(true);
	}

	HAL_SetupADCForUBattMeasurement();
	Rand_GetRandom(); /* Blocking till initial enthropy gathered */

	HAL_SwitchUBattCheck(currentUBattCheckState);
}

void Rand_SetSeed(uint32_t seed)
{
	Rand_CurrentValue = seed;

	if (Rand_CurrentValue >= RAND_MODULUS)
	{
		Rand_CurrentValue -= RAND_MODULUS;
	}

	Rand_IsInitialized = true;
}

uint32_t Rand_GetRandom(void)
{
	while (!Rand_IsInitialized) {}

	Rand_CurrentValue = (RAND_MULTIPLIER * Rand_CurrentValue + RAND_INCREMENT) % RAND_MODULUS;

	return Rand_CurrentValue;
}
