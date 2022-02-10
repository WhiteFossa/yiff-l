/*
 * Bootloader.h
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_BOOTLOADER_H_
#define INCLUDE_BOOTLOADER_H_

#include <stdint.h>

/**
 * Fox main code entry point
 */
#define YBL_MAIN_CODE_START 0x08004000

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


#endif /* INCLUDE_BOOTLOADER_H_ */
