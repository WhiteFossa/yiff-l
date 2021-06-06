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

	GetSpeed = 0x0D
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
 * Called when command "Set code" comes
 */
void OnSetCode(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get speed" comes
 */
void OnGetSpeed(uint8_t payloadSize, uint8_t* payload);

/**
 * Returns 0x00 if false, 0x01 if true
 */
uint8_t FromBool(bool data);

/**
 * If 0x00 then returns false, otherwise - true
 */
bool ToBool(uint8_t data);

#endif /* INCLUDE_PACKETSPROCESSOR_H_ */
