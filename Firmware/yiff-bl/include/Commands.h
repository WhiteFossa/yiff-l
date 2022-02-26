/*
 * Commands.h
 *
 *  Created on: Feb 18, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_COMMANDS_H_
#define INCLUDE_COMMANDS_H_

#include <stdbool.h>

/**
 * Flags for various commands, which bootloader can get
 */
typedef struct
{
	/**
	 * If true, then we need to reboot to main firmware
	 */
	bool IsRebootToMainFirmware;

	/**
	 * If true, then we need to erase FLASH page
	 */
	bool IsEraseFlashPage;
}
PendingCommandsFlagsStruct;

/**
 * Parameters for various commands, which bootloader can get
 */
typedef struct
{
	/**
	 * Start address of FLASH page, which will be erased if IsEraseFlashPage flag is set
	 */
	uint32_t EraseThisFlashPageStartAddress;
}
PendingCommandsDataStruct;

#endif /* INCLUDE_COMMANDS_H_ */
