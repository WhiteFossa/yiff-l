/*
 * SelfDiagnostics.c
 *
 *  Created on: Oct 31, 2021
 *      Author: fossa
 */

#include <SelfDiagnostics.h>
#include <EEPROM.h>
#include <HAL.h>

void SelfDiagnostics_HaltOnFailure(YhlFailureCausesEnum failureCause)
{
	/* Shutting down everything */
	HAL_EmergencyShutdown();

	/* Logging error (if EEPROM is accessable) */
	if (EEPROMContext.IsFound && IsEEPROMHeadersInitialized)
	{
		EEPROM_Header.LastFailure = failureCause;
		EEPROM_WriteHeader(&EEPROM_Header, EEPROM_ConstantHeader.HeaderAddress);
	}

	L2HAL_ERROR_CLOCK_SIGNAL_PORT();

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = L2HAL_ERROR_SIGNAL_PIN;
	HAL_GPIO_Init(L2HAL_ERROR_SIGNAL_PORT, &GPIO_InitStruct);

	/* Halt */
	L2HAL_Error(Generic);
}
