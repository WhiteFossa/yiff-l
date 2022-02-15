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

	uint8_t response[10];

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

	SendResponse(YBL_GetIdentificationData, 14, response);
}
