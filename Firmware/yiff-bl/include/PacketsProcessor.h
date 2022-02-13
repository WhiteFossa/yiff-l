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

extern L2HAL_CRCContextStruct CRC_Context;

/**
 * Packet size minus this value is payload size
 */
#define YBL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA 5U

/**
 * If first byte of payload is this byte, then it's a command to fox
 */
#define YBL_PACKET_PROCESSOR_COMMAND_TO_FOX 0x00U

/**
 * Packet payload with command to fox can't be shorter then this
 */
#define YBL_PACKET_PROCESSOR_MIN_COMMAND_TO_FOX_PAYLOAD_LENGTH 2U

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

#endif /* INCLUDE_PACKETSPROCESSOR_H_ */
