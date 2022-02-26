/*
 * PacketsProcessor.c
 *
 *  Created on: Feb 13, 2022
 *      Author: fossa
 */

#include <PacketsProcessor.h>
#include <UART.h>

void OnNewRawPacket(uint8_t size, uint8_t* packet)
{

	/* Checking packet CRC */
	uint8_t crcBuffer[4];
	memcpy(crcBuffer, packet + size - 4U, 4); /* Four last bytes are always CRC */

	uint32_t expectedCRC = ((uint32_t*)crcBuffer)[0];
	volatile uint32_t calculatedCRC = L2HAL_CRC_Calculate(&CRC_Context, packet, size - 4U);

	if (expectedCRC != calculatedCRC)
	{
		return;
	}

	uint8_t payloadSize = (uint8_t)(size - YBL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA);
	uint8_t payload[YBL_UART_PACKET_MAX_SIZE - YBL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA];

	memcpy(payload, packet + 1, payloadSize); /* Skip first byte of packet, where is packet size stored */

	OnNewPacket(payloadSize, payload);
}

void OnNewPacket(uint8_t payloadSize, uint8_t* payload)
{
	if (YBL_PACKET_PROCESSOR_COMMAND_TO_FOX == payload[0])
	{
		if (payloadSize >= YBL_PACKET_PROCESSOR_MIN_COMMAND_TO_FOX_PAYLOAD_LENGTH)
		{
			uint8_t commandPayloadSize = (uint8_t)(payloadSize - 1U);
			uint8_t commandPayload[YBL_UART_PACKET_MAX_SIZE - YBL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA - 1U];
			memcpy(commandPayload, payload + 1, commandPayloadSize);

			OnNewCommand(commandPayloadSize, commandPayload);

			return;
		}
	}
}

void OnNewCommand(uint8_t payloadSize, uint8_t* payload)
{
	switch(payload[0])
	{
		case YBL_GetIdentificationData:
			/* Identify herself */
			OnGetIdentificationData(payloadSize, payload);
			break;

		case YBL_RebootToMainFirmware:
			/* Reboot to main firmware */
			OnRebootToMainFirmware(payloadSize, payload);
			break;

		case YBL_ReadFlashPage:
			/* Read flash page */
			OnReadFlashPage(payloadSize, payload);
			break;

		case YBL_EraseFlashPage:
			/* Erase flash page */
			OnEraseFlashPage(payloadSize, payload);
			break;
	}
}

void SendResponse(Commands responseTo, uint8_t payloadSize, uint8_t* payload)
{
	uint8_t fullPayloadSize = payloadSize + 2; /* +2 because one byte is response marker, another is command to what we respond */
	uint8_t fullPayload[YBL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE];

	fullPayload[0] = YBL_PACKET_PROCESSOR_RESPONSE_FROM_FOX;
	fullPayload[1] = responseTo;

	if (payloadSize > 0)
	{
		memcpy(fullPayload + 2, payload, payloadSize);
	}

	SendPacket(fullPayloadSize, fullPayload);
}

void SendPacket(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize < YBL_PACKET_PROCESSOR_MIN_PAYLOAD_SIZE || payloadSize > YBL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE)
	{
		L2HAL_Error(Generic);
	}

	uint8_t fullPacketSize = (uint8_t)(payloadSize + YBL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA);

	uint8_t fullPacket[YBL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE + YBL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA];
	fullPacket[0] = fullPacketSize; /* Length */
	memcpy(fullPacket + 1, payload, payloadSize); /* Payload */

	uint32_t crc = L2HAL_CRC_Calculate(&CRC_Context, fullPacket, fullPacketSize - 4U);
	memcpy(fullPacket + fullPacketSize - 4U, (uint8_t*)&crc, 4);

	UART_SendBlocking(fullPacket, fullPacketSize);
}

void OnGetIdentificationData(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response[22];

	/* Signature */
	uint32_t tmp32 = YBL_VER_FOX_SIGNATURE;
	memcpy(&response[0], &tmp32, 4);

	/* Protocol version */
	uint16_t tmp16 = YBL_VER_PROTOCOL_VERSION;
	memcpy(&response[4], &tmp16, 2);

	/* Hardware revision */
	tmp16 = EEPROM_ConstantHeader.HardwareRevision;
	memcpy(&response[6], &tmp16, 2);

	/* Software version */
	tmp16 = EEPROM_ConstantHeader.FirmwareVersion;
	memcpy(&response[8], &tmp16, 2);

	/* FLASH start address */
	tmp32 = YBL_FLASH_START;
	memcpy(&response[10], &tmp32, 4);

	/* Main firmware start address */
	tmp32 = YBL_MAIN_CODE_START;
	memcpy(&response[14], &tmp32, 4);

	/* FLASH end address */
	tmp32 = YBL_FLASH_END;
	memcpy(&response[18], &tmp32, 4);

	SendResponse(YBL_GetIdentificationData, 22, response);
}

void OnRebootToMainFirmware(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	PendingCommandsFlags.IsRebootToMainFirmware = true;
}

void OnReadFlashPage(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 5)
	{
		return;
	}

	uint32_t pageNumber;
	memcpy(&pageNumber, &payload[1], 4);

	uint32_t pageStartAddress = GetReadFlashAddressByPageNumber(pageNumber);

	uint8_t result[33];
	memset(result, 0x00, 33);

	if (
		pageStartAddress < YBL_FLASH_START
		||
		pageStartAddress > YBL_FLASH_END
		)
	{
		/* Out of FLASH */
		result[0] = YBL_PACKET_PROCESSOR_FAILURE;
		SendResponse(YBL_ReadFlashPage, 33, result);
		return;
	}

	result[0] = YBL_PACKET_PROCESSOR_SUCCESS;

	uint32_t pageEndAddress = pageStartAddress + YBL_READ_FLASH_PAGE_SIZE;
	if (pageEndAddress > YBL_FLASH_END)
	{
		pageEndAddress = YBL_FLASH_END;
	}

	uint8_t readLength = (uint8_t)(pageEndAddress - pageStartAddress);
	memcpy(&result[1], (uint8_t*)pageStartAddress, readLength);

	SendResponse(YBL_ReadFlashPage, 33, result);
}


void OnEraseFlashPage(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 5)
	{
		return;
	}

	uint32_t pageStartAddress;
	memcpy(&pageStartAddress, &payload[1], 4);

	if (
		pageStartAddress < YBL_MAIN_CODE_START
		||
		pageStartAddress > YBL_FLASH_END)
	{
		/* Out of main firmware */
		uint8_t result = YBL_PACKET_PROCESSOR_FAILURE;
		SendResponse(YBL_EraseFlashPage, 1, &result);
		return;
	}

	PendingCommandsData.EraseThisFlashPageStartAddress = pageStartAddress;
	PendingCommandsFlags.IsEraseFlashPage = true;
}
