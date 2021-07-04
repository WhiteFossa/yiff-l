/*
 * PacketsProcessor.h
 *
 *  Created on: May 8, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_PACKETSPROCESSOR_H_
#define INCLUDE_PACKETSPROCESSOR_H_

#include <Commands.h>

extern PendingCommandsFlagsStruct PendingCommandsFlags;

/**
 * Packet size minus this value is payload size
 */
#define YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA 5U

/**
 * If first byte of payload is this byte, then it's a command to fox
 */
#define YHL_PACKET_PROCESSOR_COMMAND_TO_FOX 0x00U

/**
 * Start response with this byte
 */
#define YHL_PACKET_PROCESSOR_RESPONSE_FROM_FOX 0x01U

/**
 * Packet payload with command to fox can't be shorter then this
 */
#define YHL_PACKET_PROCESSOR_MIN_COMMAND_TO_FOX_PAYLOAD_LENGTH 2U

/**
 * Possible payload sizes
 */
#define YHL_PACKET_PROCESSOR_MIN_PAYLOAD_SIZE 1U
#define YHL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE 59U

/**
 * Responses to requests
 */
#define YHL_PACKET_PROCESSOR_SUCCESS 0x00
#define YHL_PACKET_PROCESSOR_FAILURE 0x01

/**
 * Possible commands to fox
 */
typedef enum
{
	SetDateAndTime = 0x00,

	SetName = 0x01,

	GetName = 0x02,

	GetProfilesCount = 0x03,

	GetProfileName = 0x04,

	AddNewProfile = 0x05,

	GetCurrentProfileId = 0x06,

	SwitchProfile = 0x07,

	SetProfileName = 0x08,

	GetFrequency = 0x09,

	SetFrequency = 0x0A,

	GetCode = 0x0B,

	SetCode = 0x0C,

	GetSpeed = 0x0D,

	SetSpeed = 0x0E,

	GetCycle = 0x0F,

	SetCycle = 0x10,

	GetEndingToneDuration = 0x11,

	SetEndingToneDuration = 0x12,

	GetBeginAndEndTimes = 0x13,

	IsFoxArmed = 0x14,

	ArmFox = 0x15,

	DisarmFox = 0x16,

	SetBeginAndEndTimes = 0x17,

	GetFoxPower = 0x18,

	SetFoxPower = 0x19,

	GetBatteryLevel = 0x1A,

	GetUAntADC = 0x1B,

	GetUBattADC = 0x1C,

	GetU80mADC = 0x1D
}
CommandToFoxEnum;

/**
 * Call this when new raw packet came from UART
 */
void OnNewRawPacket(uint8_t size, uint8_t* packet);

/**
 * Called when new packet with correct CRC arrives
 */
void OnNewPacket(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when new command to fox came. Payload is not a packet payload, but packet payload except first byte
 * (which is always YHL_PACKET_PROCESSOR_COMMAND_TO_FOX).
 */
void OnNewCommandToFox(uint8_t payloadSize, uint8_t* payload);

/**
 * Sends packet to smartphone
 */
void SendPacket(uint8_t payloadSize, uint8_t* payload);

/**
 * Send response to given command
 */
void SendResponse(CommandToFoxEnum responseTo, uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set date and time" comes
 */
void OnSetDateAndTime(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set fox name" comes
 */
void OnSetName(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get fox name" comes
 */
void OnGetName(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get profiles count" comes
 */
void OnGetProfilesCount(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get profile name" comes
 */
void OnGetProfileName(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Add new profile" comes
 */
void OnAddNewProfile(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get current profile ID" comes
 */
void OnGetCurrentProfileId(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Switch profile" comes
 */
void OnSwitchProfile(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set profile name" comes
 */
void OnSetProfileName(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get frequency" comes
 */
void OnGetFrequency(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set frequency" comes
 */
void OnSetFrequency(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get code" comes
 */
void OnGetCode(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set speed" comes
 */
void OnSetSpeed(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set code" comes
 */
void OnSetCode(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get speed" comes
 */
void OnGetSpeed(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get cycle parameters" comes
 */
void OnGetCycle(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set cycle parameters" comes
 */
void OnSetCycle(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get ending tone duration" comes
 */
void OnGetEndingToneDuration(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set ending tone duration" comes
 */
void OnSetEndingToneDuration(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get begin and end times" comes
 */
void OnGetBeginAndEndTimes(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Is fox armed" comes
 */
void OnIsFoxArmed(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Arm fox" comes
 */
void OnArmFox(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Disarm fox" comes
 */
void OnDisarmFox(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set begin and end times" comes
 */
void OnSetBeginAndEndTimes(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get fox power" comes
 */
void OnGetFoxPower(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set fox power" comes
 */
void OnSetFoxPower(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get battery level" command comes
 */
void OnGetBatteryLevel(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get UAnt ADC" command comes
 */
void OnGetUAntADC(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get UBatt ADC" command comes
 */
void OnGetUBattADC(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get U80m ADC" command comes
 */
void OnGetU80mADC(uint8_t payloadSize, uint8_t* payload);

/**
 * Returns 0x00 if false, 0x01 if true
 */
uint8_t FromBool(bool data);

/**
 * If 0x00 then returns false, otherwise - true
 */
bool ToBool(uint8_t data);

/**
 * If 0x00 or 0x01 returns true, otherwise false
 */
bool IsBool(uint8_t data);

#endif /* INCLUDE_PACKETSPROCESSOR_H_ */
