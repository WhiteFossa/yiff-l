/*
 * PacketsProcessor.h
 *
 *  Created on: Feb 13, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_PACKETSPROCESSOR_H_
#define INCLUDE_PACKETSPROCESSOR_H_

#include <stdint.h>
#include <l2hal_crc.h>
#include <Version.h>
#include <EEPROM.h>
#include <Commands.h>
#include <Bootloader.h>

extern L2HAL_CRCContextStruct CRC_Context;
extern EEPROMConstantHeaderStruct EEPROM_ConstantHeader;
extern PendingCommandsFlagsStruct PendingCommandsFlags;

/**
 * Packet size minus this value is payload size
 */
#define YBL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA 5U

/**
 * If first byte of payload is this byte, then it's a command to fox
 */
#define YBL_PACKET_PROCESSOR_COMMAND_TO_FOX 0x00U

/**
 * Start response with this byte
 */
#define YBL_PACKET_PROCESSOR_RESPONSE_FROM_FOX 0x01U

/**
 * Packet payload with command to fox can't be shorter then this
 */
#define YBL_PACKET_PROCESSOR_MIN_COMMAND_TO_FOX_PAYLOAD_LENGTH 2U

/**
 * Possible payload sizes
 */
#define YBL_PACKET_PROCESSOR_MIN_PAYLOAD_SIZE 1U
#define YBL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE 59U

/**
 * Responses to requests
 */
#define YBL_PACKET_PROCESSOR_SUCCESS 0x00
#define YBL_PACKET_PROCESSOR_FAILURE 0x01

/**
 * Possible commands
 */
typedef enum
{
	YBL_GetIdentificationData = 0xFF,

	YBL_RebootToMainFirmware = 0xFE,

	YBL_ReadFlashPage = 0xFD
}
Commands;

/**
 * Call this when new raw packet came from UART
 */
void OnNewRawPacket(uint8_t size, uint8_t* packet);

/**
 * Called when new packet with correct CRC arrives
 */
void OnNewPacket(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when new command came. Payload is not a packet payload, but packet payload except first byte
 * (which is always YHL_PACKET_PROCESSOR_COMMAND_TO_FOX).
 */
void OnNewCommand(uint8_t payloadSize, uint8_t* payload);

/**
 * Sends response to given command
 */
void SendResponse(Commands responseTo, uint8_t payloadSize, uint8_t* payload);

/**
 * Sends packet to smartphone
 */
void SendPacket(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get identification data" command comes
 */
void OnGetIdentificationData(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Reboot to main firmware" command comes
 */
void OnRebootToMainFirmware(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Read FLASH page" command comes
 */
void OnReadFlashPage(uint8_t payloadSize, uint8_t* payload);

#endif /* INCLUDE_PACKETSPROCESSOR_H_ */
