/*
 * Bootloader.h
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_BOOTLOADER_H_
#define INCLUDE_BOOTLOADER_H_

#include <stdint.h>
#include <stdio.h>
#include <Log.h>
#include <EEPROM.h>
#include <UART.h>
#include <PacketsProcessor.h>
#include <HAL.h>
#include <Commands.h>


extern EEPROMConstantHeaderStruct EEPROM_ConstantHeader;
extern PendingCommandsFlagsStruct PendingCommandsFlags;
/**
 * Fox main code entry point
 */
#define YBL_MAIN_CODE_START 0x08008000

/**
 * Entry point
 */
typedef void (EntryPoint)(void);

/**
 * Jump to entry point struct
 */
typedef struct
{
	/**
	 * Stack pointer
	 */
	uint32_t StackPointer;

	/**
	 * Entry point
	 */
	EntryPoint* EntryPoint;
}
JumpToEntryPointStruct;

/**
 * Jumps to given entry point
 */
void JumpToEntryPoint(const uint32_t address);

/**
 * Enter Device Firmware Upload mode
 */
void EnterDFUMode(void);

/**
 * Reboots to main firmware if needed
 */
void ProcessRebootToMainFirmware(void);

#endif /* INCLUDE_BOOTLOADER_H_ */
