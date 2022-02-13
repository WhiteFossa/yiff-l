/*
 * Bootloader.c
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#include <Bootloader.h>

void JumpToEntryPoint(const uint32_t address)
{
	char textBuffer[33];
	snprintf(textBuffer, 32, "Jumping to OEP");
	Log_AddLine(textBuffer);

	const JumpToEntryPointStruct* jumpStruct = (JumpToEntryPointStruct*)address;

	asm("msr msp, %0; bx %1;" : : "r"(jumpStruct->StackPointer), "r"(jumpStruct->EntryPoint));
}

void EnterDFUMode(void)
{
	char textBuffer[33];
	snprintf(textBuffer, 32, "Entering DFU mode");
	Log_AddLine(textBuffer);

	/* TODO: Implement DFU mode */
	while(true) {}
}
