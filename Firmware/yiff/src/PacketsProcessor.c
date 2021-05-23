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
	if (YHL_PACKET_PROCESSOR_COMMAND_TO_FOX == payload[0])
	{
		if (payloadSize >= YHL_PACKET_PROCESSOR_MIN_COMMAND_TO_FOX_PAYLOAD_LENGTH)
		{
			uint8_t commandPayloadSize = (uint8_t)(payloadSize - 1U);
			uint8_t* commandPayload = malloc(commandPayloadSize);
			memcpy(commandPayload, payload + 1, commandPayloadSize);
			free(payload);

			OnNewCommandToFox(commandPayloadSize, commandPayload);

			return;
		}
	}

	free(payload);
}

void OnNewCommandToFox(uint8_t payloadSize, uint8_t* payload)
{
	switch(payload[0])
	{
		case SetDateAndTime:

			/* Set date and time */
			OnSetDateAndTime(payloadSize, payload);
			break;

		case SetName:

			/* Set fox name */
			OnSetName(payloadSize, payload);
			break;
	}

	free(payload);
}

void OnSetDateAndTime(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (payloadSize != 8U)
	{
		isValid = false;
	}

	uint8_t year = payload[1];
	if (year > 99U)
	{
		isValid = false;
	}

	uint8_t month = payload[2];
	if (month < 1U || month > 12U)
	{
		isValid = false;
	}

	uint8_t dayOfMonth = payload[3];
	if (dayOfMonth < 1U || dayOfMonth > 31U)
	{
		isValid = false;
	}

	uint8_t dayOfWeekRaw = payload[4];
	if (dayOfWeekRaw < 1U || dayOfWeekRaw > 7U)
	{
		isValid = false;
	}

	uint8_t hour = payload[5];
	if (hour > 23U)
	{
		isValid = false;
	}

	uint8_t minute = payload[6];
	if (minute > 59U)
	{
		isValid = false;
	}

	uint8_t second = payload[7];
	if (second > 59U)
	{
		isValid = false;
	}

	uint8_t result;
	if (isValid)
	{
		RTC_DateTypeDef date;
		date.Year = year;
		date.Month = month;
		date.Date = dayOfMonth;
		date.WeekDay = GetWeekdayFromDayNumber(dayOfWeekRaw);
		RTC_SetCurrentDate(date);

		RTC_TimeTypeDef time;
		time.Hours = hour;
		time.Minutes = minute;
		time.Seconds = second;
		RTC_SetCurrentTime(time);

		/* Everything is OK */
		result = 0x00;
	}
	else
	{
		/* Validation failed */
		result = 0x01;
	}

	SendResponse(SetDateAndTime, 1, &result);
}

void OnSetName(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	uint8_t nameLength = payload[1];

	if (nameLength < 1 || nameLength > 32)
	{
		isValid = false;
	}

	if (payloadSize != nameLength + 2)
	{
		isValid = false;
	}

	if (isValid)
	{
		memcpy(FoxState.Name, &payload[2], nameLength);
		FoxState.Name[nameLength] = 0x00;

		/* Response will be sent from main thread */
		FoxStateNameChanged = true;
	}
	else
	{
		/* Validation failed */
		uint8_t result = 0x01;
		SendResponse(SetName, 1, &result);
	}
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

void SendResponse(CommandToFoxEnum responseTo, uint8_t payloadSize, uint8_t* payload)
{
	uint8_t fullPayloadSize = payloadSize + 2; /* +2 because one byte is response marker, another is command to what we respond */
	uint8_t* fullPayload = malloc(fullPayload);

	fullPayload[0] = YHL_PACKET_PROCESSOR_RESPONSE_FROM_FOX;
	fullPayload[1] = responseTo;

	memcpy(fullPayload + 2, payload, payloadSize);

	SendPacket(fullPayloadSize, fullPayload);
	free(fullPayload);
}
