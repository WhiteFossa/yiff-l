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
	snprintf(textBuffer, 33, "Jumping to OEP");
	Log_AddLine(textBuffer);

	__disable_irq();
	SCB->VTOR = YBL_MAIN_CODE_START;

	const JumpToEntryPointStruct* jumpStruct = (JumpToEntryPointStruct*)address;
	asm("msr msp, %0; bx %1;" : : "r"(jumpStruct->StackPointer), "r"(jumpStruct->EntryPoint));
}

void EnterDFUMode(void)
{
	char textBuffer[33];
	snprintf(textBuffer, 33, "Entering DFU mode");
	Log_AddLine(textBuffer);

	snprintf(textBuffer, 33, "HW revision: %d", EEPROM_ConstantHeader.HardwareRevision);
	Log_AddLine(textBuffer);

	snprintf(textBuffer, 33, "FW version: %d", EEPROM_ConstantHeader.FirmwareVersion);
	Log_AddLine(textBuffer);

	snprintf(textBuffer, 33, "EEPROM version: %d", EEPROM_ConstantHeader.EEPROMVersion);
	Log_AddLine(textBuffer);

	/* Setting up UART */
	UART_Init();
	UART_StartListen();

	snprintf(textBuffer, 33, "UART ready");
	Log_AddLine(textBuffer);

	while(true)
	{
		if (UART_IsPacketReady)
		{
			OnNewRawPacket(UART_ReceivedPacketFullLength, UART_ReceivedPacket);

			UART_IsPacketReady = false;
		}
	}
}
