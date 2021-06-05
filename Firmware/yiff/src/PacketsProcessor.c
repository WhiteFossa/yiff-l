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

		case GetName:
			/* Get fox name */
			OnGetName(payloadSize, payload);
			break;

		case GetProfilesCount:
			/* Get profiles count */
			OnGetProfilesCount(payloadSize, payload);
			break;

		case GetProfileName:
			/* Get profile name */
			OnGetProfileName(payloadSize, payload);
			break;

		case AddNewProfile:
			/* Add new profile */
			OnAddNewProfile(payloadSize, payload);
			break;

		case GetCurrentProfileId:
			/* Get current profile ID*/
			OnGetCurrentProfileId(payloadSize, payload);
			break;

		case SwitchProfile:
			/* Switch profile */
			OnSwitchProfile(payloadSize, payload);
			break;

		case SetProfileName:
			/* Set profile name */
			OnSetProfileName(payloadSize, payload);
			break;

		case GetFrequency:
			/* Get frequency */
			OnGetFrequency(payloadSize, payload);
			break;

		case SetFrequency:
			/* Set frequency */
			OnSetFrequency(payloadSize, payload);
			break;

		case GetCode:
			/* Get fox code */
			OnGetCode(payloadSize, payload);
			break;

		case SetCode:
			/* Set fox code */
			OnSetCode(payloadSize, payload);
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
		result = YHL_PACKET_PROCESSOR_SUCCESS;
	}
	else
	{
		/* Validation failed */
		result = YHL_PACKET_PROCESSOR_FAILURE;
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
		PendingCommandsFlags.FoxStateNameChanged = true;
	}
	else
	{
		/* Validation failed */
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetName, 1, &result);
	}
}

void OnGetName(uint8_t payloadSize, uint8_t* payload)
{
	uint8_t nameLength = (uint8_t)strlen(FoxState.Name);
	uint8_t responseSize = (uint8_t)(nameLength + 1U);

	uint8_t* response = malloc(responseSize);
	response[0] = nameLength;
	memcpy(&response[1], FoxState.Name, nameLength);

	SendResponse(GetName, responseSize, response);
	free(response);
}

void OnGetProfilesCount(uint8_t payloadSize, uint8_t* payload)
{
	uint8_t response = EEPROM_Header.NumberOfProfiles;
	SendResponse(GetProfilesCount, 1, &response);
}

void OnGetProfileName(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (payloadSize != 2)
	{
		isValid = false;
	}

	uint8_t profileId = payload[1];
	if (!EEPROM_IsProfileIdValid(profileId))
	{
		isValid = false;
	}

	if (isValid)
	{
		EEPROMProfileStruct profile = EEPROM_GetProfile(profileId);
		uint8_t nameLength = strlen(profile.Name);
		uint8_t responseLength = nameLength + 3;

		uint8_t* response = malloc(responseLength);

		response[0] = YHL_PACKET_PROCESSOR_SUCCESS;
		response[1] = profileId;
		response[2] = nameLength;
		memcpy(&response[3], profile.Name, nameLength);

		SendResponse(GetProfileName, responseLength, response);
		free(response);
	}
	else
	{
		uint8_t response[4];
		response[0] = YHL_PACKET_PROCESSOR_FAILURE; /* Not successful */
		response[1] = 0x00; /* Meaningless profile */
		response[2] = 1; /* Which name is 1 character long */
		response[3] = ' '; /* And it's a space */

		SendResponse(GetProfileName, 4, response);
	}
}

void OnAddNewProfile(uint8_t payloadSize, uint8_t* payload)
{
	bool canWeAdd = true;

	if (payloadSize != 1)
	{
		canWeAdd = false;
	}

	if (YHL_MAX_PROFILES_COUNT == EEPROM_Header.NumberOfProfiles)
	{
		canWeAdd = false;
	}

	if (!canWeAdd)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(AddNewProfile, 1, &result);
		return;
	}

	/* Response will be sent from main thread */
	PendingCommandsFlags.NeedToAddNewProfile = true;
}

void OnGetCurrentProfileId(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		/* Just ignoring incorrect command */
		return;
	}

	uint8_t response = EEPROM_Header.ProfileInUse;
	SendResponse(GetCurrentProfileId, 1, &response);
}

void OnSwitchProfile(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (payloadSize != 2)
	{
		isValid = false;
	}

	uint8_t profileId = payload[1];
	if (!EEPROM_IsProfileIdValid(profileId))
	{
		isValid = false;
	}

	if (isValid)
	{
		SwitchToThisProfileId = profileId;
		PendingCommandsFlags.NeedToSwitchProfile = true;
	}
	else
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SwitchProfile, 1, &result);
	}
}

void OnSetProfileName(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	uint8_t nameLength = payload[1];

	if (nameLength < 1 || nameLength > 16)
	{
		isValid = false;
	}

	if (payloadSize != nameLength + 2)
	{
		isValid = false;
	}

	if (isValid)
	{
		memcpy(SetThisProfileName, &payload[2], nameLength);
		SetThisProfileName[nameLength] = 0x00;

		/* Response will be sent from main thread */
		PendingCommandsFlags.NeedToSetProfileName = true;
	}
	else
	{
		/* Validation failed */
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetProfileName, 1, &result);
	}
}

void OnGetFrequency(uint8_t payloadSize, uint8_t* payload)
{
	uint8_t response[5];

	response[0] = FromBool(FoxState.Frequency.Is144MHz);

	memcpy(&response[1], (uint8_t*)&FoxState.Frequency.FrequencyHz, 4);
	SendResponse(GetFrequency, 5, response);
}

void OnSetFrequency(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (payloadSize != 6)
	{
		isValid = false;
	}

	bool is144MHz = ToBool(payload[1]);
	uint32_t frequency = *((uint32_t*)&payload[2]);

	if (!FoxState_IsFrequencyValid(is144MHz, frequency))
	{
		isValid = false;
	}

	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetFrequency, 1, &result);
	}

	FoxState.Frequency.Is144MHz = is144MHz;
	FoxState.Frequency.FrequencyHz = frequency;

	PendingCommandsFlags.NeedToSetFrequency = true;
}

void OnGetCode(uint8_t payloadSize, uint8_t* payload)
{
	uint8_t response = FoxState.Code;
	SendResponse(GetCode, 1, &response);
}

void OnSetCode(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (payloadSize != 2)
	{
		isValid = false;
	}

	uint8_t code = payload[1];

	if (code > Beacon) /* Always set to last member of FoxCodeEnum */
	{
		isValid = false;
	}

	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetFrequency, 1, &result);
	}

	FoxState.Code = (FoxCodeEnum)code;
	PendingCommandsFlags.NeedToSetCode = true;
}

uint8_t FromBool(bool data)
{
	if (data)
	{
		return 0x01;
	}
	else
	{
		return 0x00;
	}
}

bool ToBool(uint8_t data)
{
	if (0x00 == data)
	{
		return false;
	}
	else
	{
		return true;
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
	uint8_t* fullPayload = malloc(fullPayloadSize);

	fullPayload[0] = YHL_PACKET_PROCESSOR_RESPONSE_FROM_FOX;
	fullPayload[1] = responseTo;

	memcpy(fullPayload + 2, payload, payloadSize);

	SendPacket(fullPayloadSize, fullPayload);
	free(fullPayload);
}
