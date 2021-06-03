/*
 * Commands.h
 *
 *  Created on: Jun 1, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_COMMANDS_H_
#define INCLUDE_COMMANDS_H_

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
}
PendingCommandsFlagsStruct;



#endif /* INCLUDE_COMMANDS_H_ */
