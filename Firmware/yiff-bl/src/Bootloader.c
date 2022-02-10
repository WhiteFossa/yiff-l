/*
 * Bootloader.c
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#include <Bootloader.h>

void JumpToEntryPoint(const uint32_t address)
{
	const JumpToEntryPointStruct* jumpStruct = (JumpToEntryPointStruct*)address;

	asm("msr msp, %0; bx %1;" : : "r"(jumpStruct->StackPointer), "r"(jumpStruct->EntryPoint));
}
