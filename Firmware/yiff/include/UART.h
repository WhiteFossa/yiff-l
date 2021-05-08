/*
 * UART.h
 *
 *  Created on: Apr 27, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

#include <l2hal.h>
#include <stdlib.h>

/**
 * Timeout for blocking transfers
 */
#define YHL_UART_BLOCKING_TRANSFER_TIMEOUT 1000000U

/**
 * If packet isn't received during this time, reset state machine
 */
#define YHL_UART_NON_BLOCKING_RX_TIMEOUT 1000U

/**
 * Possible packet sizes
 */
#define YHL_UART_PACKET_MIN_SIZE 6
#define YHL_UART_PACKET_MAX_SIZE 64

/**
 * UART packets state machine state
 */
typedef enum
{
	/**
	 * Listening for commands not initiated
	 */
	BeforeListen,

	/**
	 * Listening for commands, no bytes came yet
	 */
	Listen,

	/**
	 * Some bytes came
	 */
	InProgress

}
UART_PacketsSMStateEnum;

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

/**
 * TX completed handler
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);

/**
 * RX completed handler
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);

/**
 * Call it before working with UART
 */
void UART_Init(void);

/**
 * Call it to start to listen for incoming packets. Feed it with packet processing function.
 * DO NOT FORGET TO CALL free() ON PACKET AFTER PROCESSING
 */
void UART_StartListen(void (*onNewPacketFunction)(uint8_t packetLength, uint8_t* packet));

/**
 * Cancels listening
 */
void UART_AbortListen(void);

/**
 * Send message in a blocking mode. Message is copied into internal buffer, so changing data in message during transfer
 * (by interrupts, for example) is not a problem
 */
void UART_SendBlocking(uint8_t* message, uint8_t size);

/**
 * Receive message in blocking mode
 */
void UART_ReadBlocking(uint8_t* buffer, uint8_t size);

/**
 * Send message in semiblocking mode. If no second call of this function during transmission, then it will be no blocking,
 * otherwise second call will wait for transmission completion.
 */
void UART_SendSemiBlocking(uint8_t* message, uint8_t size);

/**
 * Call this every millisecond
 */
void UART_Tick(void);

/**
 * Ask for next byte (private function)
 */
void UART_AskForNextByte(void);

#endif /* INCLUDE_UART_H_ */
