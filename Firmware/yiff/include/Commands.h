/*
 * Commands.h
 *
 *  Created on: Jun 1, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_COMMANDS_H_
#define INCLUDE_COMMANDS_H_

#include <l2hal_aux.h>

/**
 * Flags for various commands, which fox can get
 */
typedef struct
{
	/**
	 * If true, then fox name just changed, we need to write it to EEPROM from the main thread
	 */
	bool FoxStateNameChanged;

	/**
	 * If true, then we need to add new profile in main thread
	 */
	bool NeedToAddNewProfile;

	/**
	 * If true, then we need to switch profile to one in SwitchToThisProfileId
	 */
	bool NeedToSwitchProfile;

	/**
	 * If true, then current profile have changes, which must be saved to EEPROM
	 */
	bool NeedToFlushCurrentProfileToEEPROM;

	/**
	 * If true, then we need to set current profile name to SetThisProfileName
	 */
	bool NeedToSetProfileName;

	/**
	 * If true, then we need to update frequency
	 */
	bool NeedToSetFrequency;

	/**
	 * If true, then we need to update fox code
	 */
	bool NeedToSetCode;

	/**
	 * If true, then we need to update fox speed
	 */
	bool NeedToSetSpeed;

	/**
	 * If true, then we need to update fox cycle
	 */
	bool NeedToSetCycle;

	/**
	 * If true, then we need to update ending tone duration
	 */
	bool NeedToSetEndingToneDuration;

	/**
	 * If true, then we need to update start and end times
	 */
	bool NeedToSetBeginAndEndTimes;

	/**
	 * If true, then we need to set fox power
	 */
	bool NeedToSetPower;

	/**
	 * If true, then we need to arm fox
	 */
	bool NeedToArmFox;
}
PendingCommandsFlagsStruct;



#endif /* INCLUDE_COMMANDS_H_ */
