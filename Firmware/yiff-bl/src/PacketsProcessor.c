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

			OnNewCommandToFox(commandPayloadSize, commandPayload);

			return;
		}
	}
}

void OnNewCommandToFox(uint8_t payloadSize, uint8_t* payload)
{
	switch(payload[0])
	{
		case 0x01:
			/* Set date and time */

			break;
	}
}
