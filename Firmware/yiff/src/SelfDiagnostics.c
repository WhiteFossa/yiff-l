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
	if (EEPROMContext.IsFound && FoxState.IsEEPROMHeadersInitialized)
	{
		EEPROM_Header.LastFailure = failureCause;
		EEPROM_WriteHeader(&EEPROM_Header, EEPROM_ConstantHeader.HeaderAddress);
	}

	/* Halt */
	L2HAL_Error(Generic);
}
