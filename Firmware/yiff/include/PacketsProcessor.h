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
 * Possible payload sizes
 */
#define YHL_PACKET_PROCESSOR_MIN_PAYLOAD_SIZE 1U
#define YHL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE 59U

/**
 * Call this when new raw packet came from UART
 */
void OnNewRawPacket(uint8_t size, uint8_t* packet);

/**
 * Called when new packet with correct CRC arrives
 */
void OnNewPacket(uint8_t payloadSize, uint8_t* payload);

/**
 * Sends packet to smartphone
 */
void SendPacket(uint8_t payloadSize, uint8_t* payload);

#endif /* INCLUDE_PACKETSPROCESSOR_H_ */
