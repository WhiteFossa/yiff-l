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

	/* We might press buttons to jump to OEP. Clearing interrupt flags */
	__HAL_GPIO_EXTI_CLEAR_IT(HAL_LEFT_BUTTON_PIN);
	__HAL_GPIO_EXTI_CLEAR_IT(HAL_RIGHT_BUTTON_PIN);
	__HAL_GPIO_EXTI_CLEAR_IT(HAL_ENCODER_BUTTON_PIN);
	__HAL_GPIO_EXTI_CLEAR_IT(HAL_ENCODER_RIGHT_PIN);

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

	snprintf(textBuffer, 33, "Both buttons to exit");
	Log_AddLine(textBuffer);

	while(true)
	{
		if (HAL_IsLeftButtonPressed() && HAL_IsRightButtonPressed())
		{
			JumpToEntryPoint(YBL_MAIN_CODE_START);
		}

		if (UART_IsPacketReady)
		{
			OnNewRawPacket(UART_ReceivedPacketFullLength, UART_ReceivedPacket);

			UART_IsPacketReady = false;
		}
	}
}
