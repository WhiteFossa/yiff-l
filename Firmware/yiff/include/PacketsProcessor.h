/*
 * PacketsProcessor.h
 *
 *  Created on: May 8, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_PACKETSPROCESSOR_H_
#define INCLUDE_PACKETSPROCESSOR_H_

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
 * Possible commands to fox
 */
typedef enum
{
	SetDateAndTime = 0x00
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
 * Called when command "Set date and time came"
 */
void OnSetDateAndTime(uint8_t payloadSize, uint8_t* payload);

#endif /* INCLUDE_PACKETSPROCESSOR_H_ */
