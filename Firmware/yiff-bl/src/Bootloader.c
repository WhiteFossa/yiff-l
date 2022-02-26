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

	/* De-initializing UART */
	HAL_DisableUART();

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

		ProcessRebootToMainFirmware();
		ProcessFlashPageErase();
	}
}

void ProcessRebootToMainFirmware(void)
{
	if (PendingCommandsFlags.IsRebootToMainFirmware)
	{
		SendResponse(YBL_RebootToMainFirmware, 0, NULL);

		/* Giving fox time to transmit a response */
		HAL_Delay(1000);

		JumpToEntryPoint(YBL_MAIN_CODE_START);
	}
}

uint32_t GetReadFlashAddressByPageNumber(uint32_t pageNumber)
{
	return YBL_FLASH_START + YBL_READ_FLASH_PAGE_SIZE * pageNumber;
}

void ProcessFlashPageErase(void)
{
	if (PendingCommandsFlags.IsEraseFlashPage)
	{
		uint8_t result;
		bool isSuccessful = true;

		if (HAL_FLASH_Unlock() != HAL_OK)
		{
			isSuccessful = false;
			goto ProcessFlashPageEraseSendResponse;
		}

		FLASH_EraseInitTypeDef eraseInitStruct;
		eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
		eraseInitStruct.Banks = FLASH_BANK_1;
		eraseInitStruct.PageAddress = PendingCommandsData.EraseThisFlashPageStartAddress;
		eraseInitStruct.NbPages = 1;

		uint32_t eraseResult;
		if (HAL_FLASHEx_Erase(&eraseInitStruct, &eraseResult) != HAL_OK)
		{
			isSuccessful = false;
			goto ProcessFlashPageEraseSendResponse;
		}

		if (eraseResult != 0xFFFFFFFF)
		{
			isSuccessful = false;
			goto ProcessFlashPageEraseSendResponse;
		}

		if (HAL_FLASH_Lock() != HAL_OK)
		{
			isSuccessful = false;
			goto ProcessFlashPageEraseSendResponse;
		}

ProcessFlashPageEraseSendResponse:

		if (isSuccessful)
		{
			result = YBL_PACKET_PROCESSOR_SUCCESS;
		}
		else
		{
			result = YBL_PACKET_PROCESSOR_FAILURE;
		}

		SendResponse(YBL_EraseFlashPage, 1, &result);

		PendingCommandsFlags.IsEraseFlashPage = false;
	}
}
