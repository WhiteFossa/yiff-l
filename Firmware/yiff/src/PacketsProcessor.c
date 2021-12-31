/*
 * PacketsProcessor.c
 *
 *  Created on: May 8, 2021
 *      Author: fossa
 */

#include <PacketsProcessor.h>

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

	uint8_t payloadSize = (uint8_t)(size - YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA);
	uint8_t payload[YHL_UART_PACKET_MAX_SIZE - YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA];

	memcpy(payload, packet + 1, payloadSize); /* Skip first byte of packet, where is packet size stored */

	OnNewPacket(payloadSize, payload);
}

void OnNewPacket(uint8_t payloadSize, uint8_t* payload)
{
	/* We have a packet with correct payload, preventing fox from sleeping */
	Sleepmodes_PreventSleep();

	if (YHL_PACKET_PROCESSOR_COMMAND_TO_FOX == payload[0])
	{
		if (payloadSize >= YHL_PACKET_PROCESSOR_MIN_COMMAND_TO_FOX_PAYLOAD_LENGTH)
		{
			uint8_t commandPayloadSize = (uint8_t)(payloadSize - 1U);
			uint8_t commandPayload[YHL_UART_PACKET_MAX_SIZE - YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA - 1U];
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

		case GetSpeed:
			/* Get fox speed */
			OnGetSpeed(payloadSize, payload);
			break;

		case SetSpeed:
			/* Set fox speed */
			OnSetSpeed(payloadSize, payload);
			break;

		case GetCycle:
			/* Get cycle parameters */
			OnGetCycle(payloadSize, payload);
			break;

		case SetCycle:
			/* Set cycle parameters */
			OnSetCycle(payloadSize, payload);
			break;

		case GetEndingToneDuration:
			/* Get ending tone duration */
			OnGetEndingToneDuration(payloadSize, payload);
			break;

		case SetEndingToneDuration:
			/* Set ending tone duration */
			OnSetEndingToneDuration(payloadSize, payload);
			break;

		case GetBeginAndEndTimes:
			/* Get fox begin and end times */
			OnGetBeginAndEndTimes(payloadSize, payload);
			break;

		case IsFoxArmed:
			/* Is fox armed? */
			OnIsFoxArmed(payloadSize, payload);
			break;

		case ArmFox:
			/* Arm fox */
			OnArmFox(payloadSize, payload);
			break;

		case DisarmFox:
			/* Disarm fox */
			OnDisarmFox(payloadSize, payload);
			break;

		case SetBeginAndEndTimes:
			/* Set begin and end times */
			OnSetBeginAndEndTimes(payloadSize, payload);
			break;

		case GetFoxPower:
			/* Get fox power */
			OnGetFoxPower(payloadSize, payload);
			break;

		case SetFoxPower:
			/* Set fox power */
			OnSetFoxPower(payloadSize, payload);
			break;

		case GetBatteryLevel:
			/* Get battery level */
			OnGetBatteryLevel(payloadSize, payload);
			break;

		case GetUAntADC:
			/* Get UAnt ADC*/
			OnGetUAntADC(payloadSize, payload);
			break;

		case GetUBattADC:
			/* Get UBatt ADC*/
			OnGetUBattADC(payloadSize, payload);
			break;

		case GetU80mADC:
			/* Get U80m ADC*/
			OnGetU80mADC(payloadSize, payload);
			break;

		case GetUbattVolts:
			/* Get UBatt in volts*/
			OnGetUBattVolts(payloadSize, payload);
			break;

		case GetU80mVolts:
			/* Get U80m in volts */
			OnGetU80mVolts(payloadSize, payload);
			break;

		case GetLastFailureCode:
			/* Get last failure code */
			OnGetLastFailureCode(payloadSize, payload);
			break;

		case NoOperation:
			/* No operation (to keep fox awake) */
			OnNoOperation(payloadSize, payload);
			break;

		case GetIdentificationData:
			/* Get identification data */
			OnGetIdentificationData(payloadSize, payload);
			break;

		case GetAntennaMatchingStatus:
			/* Get antenna matching status */
			OnGetAntennaMatchingStatus(payloadSize, payload);
			break;

		case GetAntennaMatchingData:
			/* Get antenna matching data */
			OnGetAntennaMatchingData(payloadSize, payload);
			break;

		case MarkMatchingAsSeen:
			/* Mark antenna matching data as seen */
			OnMarkMatchingAsSeen(payloadSize, payload);
			break;

		case CheckForProfileSettingsChanges:
			/* Check for profile settings changes */
			OnCheckForProfileSettingsChanges(payloadSize, payload);
			break;
	}
}

void OnSetDateAndTime(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

	if (payloadSize != 8U)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

	uint8_t year = payload[1];
	if (year > 99U)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

	uint8_t month = payload[2];
	if (month < 1U || month > 12U)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

	uint8_t dayOfMonth = payload[3];
	if (dayOfMonth < 1U || dayOfMonth > 31U)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

	uint8_t dayOfWeekRaw = payload[4];
	if (dayOfWeekRaw < 1U || dayOfWeekRaw > 7U)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

	uint8_t hour = payload[5];
	if (hour > 23U)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

	uint8_t minute = payload[6];
	if (minute > 59U)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

	uint8_t second = payload[7];
	if (second > 59U)
	{
		isValid = false;
		goto OnSetDateAndTime_Validate;
	}

OnSetDateAndTime_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetDateAndTime, 1, &result);
		return;
	}

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
	uint8_t result = YHL_PACKET_PROCESSOR_SUCCESS;
	SendResponse(SetDateAndTime, 1, &result);
}

void OnSetName(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetName_Validate;
	}

	uint8_t nameLength = payload[1];

	if (nameLength < 1 || nameLength > 32)
	{
		isValid = false;
		goto OnSetName_Validate;
	}

	if (payloadSize != nameLength + 2)
	{
		isValid = false;
		goto OnSetName_Validate;
	}

OnSetName_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetName, 1, &result);
		return;
	}

	memcpy(FoxState.Name, &payload[2], nameLength);
	FoxState.Name[nameLength] = 0x00;

	/* Response will be sent from main thread */
	PendingCommandsFlags.FoxStateNameChanged = true;
}

void OnGetName(uint8_t payloadSize, uint8_t* payload)
{
	uint8_t nameLength = (uint8_t)strlen(FoxState.Name);
	uint8_t responseSize = (uint8_t)(nameLength + 1U);

	uint8_t response[YHL_FOX_NAME_MAX_LENGTH + 1U];
	response[0] = nameLength;
	memcpy(&response[1], FoxState.Name, nameLength);

	SendResponse(GetName, responseSize, response);
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
		goto OnGetProfileName_Validate;
	}

	uint8_t profileId = payload[1];
	if (!EEPROM_IsProfileIdValid(profileId))
	{
		isValid = false;
		goto OnGetProfileName_Validate;
	}

OnGetProfileName_Validate:
	if (!isValid)
	{
		uint8_t response[4];
		response[0] = YHL_PACKET_PROCESSOR_FAILURE; /* Not successful */
		response[1] = 0x00; /* Meaningless profile */
		response[2] = 1; /* Which name is 1 character long */
		response[3] = ' '; /* And it's a space */

		SendResponse(GetProfileName, 4, response);
		return;
	}

	EEPROMProfileStruct profile = EEPROM_GetProfile(profileId);
	uint8_t nameLength = (uint8_t)strlen(profile.Name);
	uint8_t responseLength = nameLength + 3;

	uint8_t response[YHL_PROFILE_NAME_MAX_LENGTH + 3];

	response[0] = YHL_PACKET_PROCESSOR_SUCCESS;
	response[1] = profileId;
	response[2] = nameLength;
	memcpy(&response[3], profile.Name, nameLength);

	SendResponse(GetProfileName, responseLength, response);
}

void OnAddNewProfile(uint8_t payloadSize, uint8_t* payload)
{
	bool canWeAdd = true;

	if (FoxState.GlobalState.IsArmed)
	{
		canWeAdd = false;
		goto OnAddNewProfile_Validate;
	}


	if (payloadSize != 1)
	{
		canWeAdd = false;
		goto OnAddNewProfile_Validate;
	}

	if (YHL_MAX_PROFILES_COUNT == EEPROM_Header.NumberOfProfiles)
	{
		canWeAdd = false;
		goto OnAddNewProfile_Validate;
	}

OnAddNewProfile_Validate:
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

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSwitchProfile_Validate;
	}

	if (payloadSize != 2)
	{
		isValid = false;
		goto OnSwitchProfile_Validate;
	}

	uint8_t profileId = payload[1];
	if (!EEPROM_IsProfileIdValid(profileId))
	{
		isValid = false;
		goto OnSwitchProfile_Validate;
	}

OnSwitchProfile_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SwitchProfile, 1, &result);
		return;
	}

	SwitchToThisProfileId = profileId;
	PendingCommandsFlags.NeedToSwitchProfile = true;
}

void OnSetProfileName(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetProfileName_Validate;
	}

	uint8_t nameLength = payload[1];

	if (nameLength < 1 || nameLength > 16)
	{
		isValid = false;
		goto OnSetProfileName_Validate;
	}

	if (payloadSize != nameLength + 2)
	{
		isValid = false;
		goto OnSetProfileName_Validate;
	}

OnSetProfileName_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetProfileName, 1, &result);
		return;
	}

	/* Response will be sent from main thread */
	memset(SetThisProfileName, 0x00U, YHL_PROFILE_NAME_MEMORY_SIZE);
	memcpy(SetThisProfileName, &payload[2], nameLength);

	PendingCommandsFlags.NeedToSetProfileName = true;
}

void OnGetFrequency(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response[5];

	response[0] = FromBool(FoxState.Frequency.Is144MHz);

	memcpy(&response[1], (uint8_t*)&FoxState.Frequency.FrequencyHz, 4);
	SendResponse(GetFrequency, 5, response);
}

void OnSetFrequency(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetFrequency_Validate;
	}

	if (payloadSize != 6)
	{
		isValid = false;
		goto OnSetFrequency_Validate;
	}

	bool is144MHz = ToBool(payload[1]);
	uint32_t frequency = *((uint32_t*)&payload[2]);

	if (!FoxState_IsFrequencyValid(is144MHz, frequency))
	{
		isValid = false;
		goto OnSetFrequency_Validate;
	}

OnSetFrequency_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetFrequency, 1, &result);
		return;
	}

	FoxState_SetFrequency(is144MHz, frequency);
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

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetCode_Validate;
	}

	if (payloadSize != 2)
	{
		isValid = false;
		goto OnSetCode_Validate;
	}

	uint8_t code = payload[1];

	if (code > Beacon) /* Always set to last member of FoxCodeEnum */
	{
		isValid = false;
		goto OnSetCode_Validate;
	}

OnSetCode_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetFrequency, 1, &result);
		return;
	}

	FoxState.Code = (FoxCodeEnum)code;
	PendingCommandsFlags.NeedToSetCode = true;
}

void OnGetSpeed(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t result = ToBool(FoxState.IsFast);
	SendResponse(GetSpeed, 1, &result);
}

void OnSetSpeed(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetSpeed_Validate;
	}

	if (payloadSize != 2)
	{
		isValid = false;
		goto OnSetSpeed_Validate;
	}

	uint8_t speedByte = payload[1];

	if (!IsBool(speedByte))
	{
		isValid = false;
		goto OnSetSpeed_Validate;
	}

	FoxState.IsFast = ToBool(speedByte);
	PendingCommandsFlags.NeedToSetSpeed = true;

OnSetSpeed_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetSpeed, 1, &result);
		return;
	}
}

void OnGetCycle(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response[5];

	response[0] = FromBool(FoxState.Cycle.IsContinuous);

	uint16_t txTimeInSeconds = (uint16_t)FoxState.Cycle.TxTime;
	memcpy(&response[1], (uint8_t*)&txTimeInSeconds, 2);

	uint16_t pauseTimeInSeconds = (uint16_t)FoxState.Cycle.PauseTime;
	memcpy(&response[3], (uint8_t*)&pauseTimeInSeconds, 2);

	SendResponse(GetCycle, 5, response);
}

void OnSetCycle(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetCycle_Validate;
	}

	if (payloadSize != 6)
	{
		isValid = false;
		goto OnSetCycle_Validate;
	}

	uint8_t isContinuousByte = payload[1];

	if (!IsBool(isContinuousByte))
	{
		isValid = false;
		goto OnSetCycle_Validate;
	}

	uint16_t txTimeInSeconds;
	memcpy(&txTimeInSeconds, &payload[2], 2);

	uint16_t pauseTimeInSeconds;
	memcpy(&pauseTimeInSeconds, &payload[4], 2);

	if (!FoxState_IsCycleDurationsValid(txTimeInSeconds, pauseTimeInSeconds))
	{
		isValid = false;
		goto OnSetCycle_Validate;
	}

OnSetCycle_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetCycle, 1, &result);
		return;
	}

	FoxState.Cycle.IsContinuous = ToBool(isContinuousByte);
	FoxState_SetCycleDurations(txTimeInSeconds, pauseTimeInSeconds);

	PendingCommandsFlags.NeedToSetCycle = true;
}

void OnGetEndingToneDuration(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response = FoxState.EndingToneLength;
	SendResponse(GetEndingToneDuration, 1, &response);
}

void OnSetEndingToneDuration(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetEndingToneDuration_Validate;
	}

	if (payloadSize != 2)
	{
		isValid = false;
		goto OnSetEndingToneDuration_Validate;
	}

	uint8_t endingToneDuration = payload[1];
	if (!FoxState_IsEndingtoneDurationValid(endingToneDuration))
	{
		isValid = false;
		goto OnSetEndingToneDuration_Validate;
	}

OnSetEndingToneDuration_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetEndingToneDuration, 1, &result);
		return;
	}

	FoxState_SetEndingtoneDuration(endingToneDuration);

	PendingCommandsFlags.NeedToSetEndingToneDuration = true;
}

void OnGetBeginAndEndTimes(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response[8];
	memcpy(&response[0], &FoxState.GlobalState.StartTimespan, 4);
	memcpy(&response[4], &FoxState.GlobalState.EndTimespan, 4);

	SendResponse(GetBeginAndEndTimes, 8, response);
}

void OnIsFoxArmed(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response = FromBool(FoxState.GlobalState.IsArmed);
	SendResponse(IsFoxArmed, 1, &response);
}

void OnArmFox(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	PendingCommandsFlags.NeedToArmFox = true;
}

void OnDisarmFox(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	bool canDisarm = true;
	if (!FoxState.GlobalState.IsArmed)
	{
		canDisarm = false;
		goto OnDisarmFox_Validate;
	}

	/* If antenna matching in progress we can't disarm fox */
	if (AntennaMatching_InProgress == FoxState.AntennaMatching.Status)
	{
		canDisarm = false;
		goto OnDisarmFox_Validate;
	}

OnDisarmFox_Validate:
	if (!canDisarm)
	{
		uint8_t response = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(DisarmFox, 1, &response);
		return;
	}

	GSM_Disarm();

	uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
	SendResponse(DisarmFox, 1, &response);
}

void OnSetBeginAndEndTimes(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetBeginAndEndTimes_Validate;
	}

	if (payloadSize != 9)
	{
		isValid = false;
		goto OnSetBeginAndEndTimes_Validate;
	}

	uint32_t startTimespan;
	memcpy(&startTimespan, &payload[1], 4);

	uint32_t endTimespan;
	memcpy(&endTimespan, &payload[5], 4);

	if (!FoxState_IsBeginAndEndTimespansValid(startTimespan, endTimespan))
	{
		isValid = false;
		goto OnSetBeginAndEndTimes_Validate;
	}

OnSetBeginAndEndTimes_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetBeginAndEndTimes, 1, &result);
		return;
	}

	GSM_Disarm();

	FoxState_SetBeginAndEndTimespans(startTimespan, endTimespan);

	PendingCommandsFlags.NeedToSetBeginAndEndTimes = true;
}

void OnGetFoxPower(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	float response = FoxState.Power;
	SendResponse(GetFoxPower, 4, (uint8_t*)&response);
}

void OnSetFoxPower(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (FoxState.GlobalState.IsArmed)
	{
		isValid = false;
		goto OnSetFoxPower_Validate;
	}

	if (payloadSize != 5)
	{
		isValid = false;
		goto OnSetFoxPower_Validate;
	}

	float power;
	memcpy(&power, &payload[1], 4);

	if (!FoxState_IsPowerValid(power))
	{
		isValid = false;
		goto OnSetFoxPower_Validate;
	}

OnSetFoxPower_Validate:
	if (!isValid)
	{
		uint8_t result = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(SetFoxPower, 1, &result);
		return;
	}

	FoxState_SetPower(power);
	PendingCommandsFlags.NeedToSetPower = true;
}

void OnGetBatteryLevel(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	float response = FoxState.BatteryLevel;
	SendResponse(GetBatteryLevel, 4, (uint8_t*)&response);
}

void OnGetUAntADC(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	float response = HAL_GetUAntADC();
	SendResponse(GetUAntADC, 4, (uint8_t*)&response);
}

void OnGetUBattADC(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	float response = HAL_GetUBattADC();
	SendResponse(GetUBattADC, 4, (uint8_t*)&response);
}

void OnGetU80mADC(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	float response = HAL_GetU80mADC();
	SendResponse(GetU80mADC, 4, (uint8_t*)&response);
}

void OnGetUBattVolts(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	float response = HAL_GetUbattVolts();
	SendResponse(GetUbattVolts, 4, (uint8_t*)&response);
}

void OnGetU80mVolts(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	float response = HAL_GetU80mVolts();
	SendResponse(GetU80mVolts, 4, (uint8_t*)&response);
}

void OnGetLastFailureCode(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint32_t response = EEPROM_Header.LastFailure;
	SendResponse(GetLastFailureCode, 4, (uint8_t*)&response);
}

void OnNoOperation(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	SendResponse(NoOperation, 0, NULL);
}

void OnGetIdentificationData(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response[14];

	/* Signature */
	uint32_t tmp32 = YHL_VER_FOX_SIGNATURE;
	memcpy(&response[0], &tmp32, 4);

	/* Protocol version */
	uint16_t tmp16 = YHL_VER_PROTOCOL_VERSION;
	memcpy(&response[4], &tmp16, 2);

	/* Hardware revision */
	tmp16 = EEPROM_Header.HardwareRevision;
	memcpy(&response[6], &tmp16, 2);

	/* Software version */
	tmp16 = EEPROM_Header.SoftwareVersion;
	memcpy(&response[8], &tmp16, 2);

	/* Serial number */
	tmp32 = EEPROM_Header.SerialNumber;
	memcpy(&response[10], &tmp32, 4);

	SendResponse(GetIdentificationData, 14, response);
}

void OnGetAntennaMatchingStatus(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response[13];

	/* Status */
	response[0] = (uint8_t)FoxState.AntennaMatching.Status;

	/* Is matching new for app */
	response[1] = FromBool(FoxState.AntennaMatching.IsNewForApp);

	/* Total matcher positions */
	response[2] = HAL_AM_MAX_VALUE + 1;

	/* Current matcher position */
	response[3] = FoxState.AntennaMatching.CurrentPosition;

	/* Current antenna voltage */
	memcpy(&response[4], &FoxState.AntennaMatching.CurrentVoltage, 4);

	/* Current best position */
	response[8] = FoxState.AntennaMatching.BestMatchPosition;

	/* Current best voltage*/
	memcpy(&response[9], &FoxState.AntennaMatching.BestMatchVoltage, 4);

	SendResponse(GetAntennaMatchingStatus, 13, response);
}

void OnGetAntennaMatchingData(uint8_t payloadSize, uint8_t* payload)
{
	bool isValid = true;

	if (payloadSize != 2)
	{
		isValid = false;
		goto OnGetAntennaMatchingData_Validate;
	}

	uint8_t matcherPosition = payload[1];

	if (matcherPosition > HAL_AM_MAX_VALUE)
	{
		isValid = false;
		goto OnGetAntennaMatchingData_Validate;
	}

	OnGetAntennaMatchingData_Validate:
	if (!isValid)
	{
		uint8_t response[5];
		memset(response, 0x00, 5);
		response[0] = YHL_PACKET_PROCESSOR_FAILURE;
		SendResponse(GetAntennaMatchingData, 5, response);
		return;
	}

	uint8_t response[5];
	response[0] = YHL_PACKET_PROCESSOR_SUCCESS;
	memcpy(&response[1], &FoxState.AntennaMatching.MatchingVoltages[matcherPosition], 4);
	SendResponse(GetAntennaMatchingData, 5, response);
}

void OnMarkMatchingAsSeen(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	FoxState.AntennaMatching.IsNewForApp = false;

	SendResponse(MarkMatchingAsSeen, 0, NULL);
}

void OnCheckForProfileSettingsChanges(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize != 1)
	{
		return;
	}

	uint8_t response = FromBool(FoxState.IsNotReportedManualProfileChanges);
	SendResponse(CheckForProfileSettingsChanges, 1, &response);
}

void EmitEnteringSleepmodeEvent(void)
{
	SendEvent(EnteringSleepmode, 0, NULL);
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

bool IsBool(uint8_t data)
{
	if (0x00 == data || 0x01 == data)
	{
		return true;
	}

	return false;
}

void SendPacket(uint8_t payloadSize, uint8_t* payload)
{
	if (payloadSize < YHL_PACKET_PROCESSOR_MIN_PAYLOAD_SIZE || payloadSize > YHL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongPayloadSizeInSendPacket);
	}

	uint8_t fullPacketSize = (uint8_t)(payloadSize + YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA);

	uint8_t fullPacket[YHL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE + YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA];
	fullPacket[0] = fullPacketSize; /* Length */
	memcpy(fullPacket + 1, payload, payloadSize); /* Payload */

	uint32_t crc = L2HAL_CRC_Calculate(&CRC_Context, fullPacket, fullPacketSize - 4U);
	memcpy(fullPacket + fullPacketSize - 4U, (uint8_t*)&crc, 4);

	UART_SendBlocking(fullPacket, fullPacketSize);
}

void SendResponse(CommandToFoxEnum responseTo, uint8_t payloadSize, uint8_t* payload)
{
	uint8_t fullPayloadSize = payloadSize + 2; /* +2 because one byte is response marker, another is command to what we respond */
	uint8_t fullPayload[YHL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE];

	fullPayload[0] = YHL_PACKET_PROCESSOR_RESPONSE_FROM_FOX;
	fullPayload[1] = responseTo;

	if (payloadSize > 0)
	{
		memcpy(fullPayload + 2, payload, payloadSize);
	}

	SendPacket(fullPayloadSize, fullPayload);
}

void SendEvent(EventsFromFoxEnum event, uint8_t payloadSize, uint8_t* payload)
{
	uint8_t fullPayloadSize = payloadSize + 2; /* +2 because one byte is event marker, another is event type */
	uint8_t fullPayload[YHL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE];

	fullPayload[0] = YHL_PACKET_PROCESSOR_EVENT_FROM_FOX;
	fullPayload[1] = event;

	if (payloadSize > 0)
	{
		memcpy(fullPayload + 2, payload, payloadSize);
	}

	SendPacket(fullPayloadSize, fullPayload);
}
