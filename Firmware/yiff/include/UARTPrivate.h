/*
 * UARTPrivate.h
 *
 *  Created on: May 31, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_UARTPRIVATE_H_
#define INCLUDE_UARTPRIVATE_H_

/**
 * Packets state machine state
 */
UART_PacketsSMStateEnum UART_PSMState;

/**
 * Rx buffer (we work byte-by-byte, so 1 byte is enough)
 */
uint8_t UART_RxByteBuffer;

/**
 * Packet is being accumulated here
 */
uint8_t UART_RxPacketBuffer[YHL_UART_PACKET_MAX_SIZE];

/**
 * Current byte within UART_RxPacketBuffer
 */
uint8_t UART_RxPacketBufferIndex;

/**
 * Decreases each millisecond, when reaches 0, resets state machine to Listen state.
 */
uint16_t UART_RxTimeoutTimer;

/**
 * When first byte of packet is came, we are able to detect packet length. That length
 * is stored here
 */
uint8_t UART_ExpectedPacketLength;

/**
 * Received packet, ready to be processed
 */
uint8_t UART_ReceivedPacket[YHL_UART_PACKET_MAX_SIZE];

/**
 * Full length of received packet
 */
uint8_t UART_ReceivedPacketFullLength;

/**
 * If true then we have packet, ready to be processed.
 */
bool UART_IsPacketReady;

#endif /* INCLUDE_UARTPRIVATE_H_ */
