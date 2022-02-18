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
}
PendingCommandsFlagsStruct;

#endif /* INCLUDE_COMMANDS_H_ */
