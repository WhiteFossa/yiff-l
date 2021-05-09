/*
 * PacketsProcessor.c
 *
 *  Created on: May 8, 2021
 *      Author: fossa
 */

#include <main.h>

void OnNewRawPacket(uint8_t size, uint8_t* packet)
{

	/* Checking packet CRC */
	uint8_t crcBuffer[4];
	memcpy(crcBuffer, packet + size - 4U, 4); /* Four last bytes are always CRC */

	uint32_t expectedCRC = ((uint32_t*)crcBuffer)[0];
	uint32_t calculatedCRC = L2HAL_CRC_Calculate(&CRC_Context, packet, size - 4U);

	if (expectedCRC != calculatedCRC)
	{
		free(packet);
		return;
	}

	uint8_t payloadSize = (uint8_t)(size - YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA);
	uint8_t* payload = malloc(payloadSize);

	memcpy(payload, packet + 1, payloadSize); /* Skip first byte of packet, there is packet size stored */
	free(packet);

	OnNewPacket(payloadSize, payload);
}

void OnNewPacket(uint8_t payloadSize, uint8_t* payload)
{
	SendPacket(payloadSize, payload);

	free(payload);
}

void SendPacket(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize < YHL_PACKET_PROCESSOR_MIN_PAYLOAD_SIZE || payloadSize > YHL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE)
	{
		L2HAL_Error(Generic);
	}

	uint8_t fullPacketSize = (uint8_t)(payloadSize + YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA);

	uint8_t* fullPacket = malloc(fullPacketSize);
	fullPacket[0] = fullPacketSize; /* Length */
	memcpy(fullPacket + 1, payload, payloadSize); /* Payload */

	uint32_t crc = L2HAL_CRC_Calculate(&CRC_Context, fullPacket, fullPacketSize - 4U);
	memcpy(fullPacket + fullPacketSize - 4U, (uint8_t*)&crc, 4);

	UART_SendSemiBlocking(fullPacket, fullPacketSize);
	free(fullPacket);
}
