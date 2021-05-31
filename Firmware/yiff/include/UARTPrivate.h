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
 * Transmission buffer (of dynamic size)
 */
uint8_t* UART_TxBuffer;

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
 * Pointer to function, called when new packet received. DO NOT FORGET
 * TO CALL free() ON PACKET AFTER PROCESSING
 */
void (*UART_OnNewPacket)(uint8_t packetFullLength, uint8_t* packet);

/**
 * True if there is ongoing semiblocking transmission
 */
volatile bool UART_TransmissionInProgress;

#endif /* INCLUDE_UARTPRIVATE_H_ */
