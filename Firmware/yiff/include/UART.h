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
#include <SelfDiagnostics.h>

extern UART_HandleTypeDef UART_Handle;

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

extern uint8_t* UART_TxBuffer;
extern uint8_t UART_RxByteBuffer;
extern uint8_t UART_RxPacketBuffer[YHL_UART_PACKET_MAX_SIZE];
extern uint8_t UART_RxPacketBufferIndex;
extern uint16_t UART_RxTimeoutTimer;
extern uint8_t UART_ExpectedPacketLength;
extern volatile bool UART_TransmissionInProgress;
extern uint8_t UART_ReceivedPacket[YHL_UART_PACKET_MAX_SIZE];
extern uint8_t UART_ReceivedPacketFullLength;
extern bool UART_IsPacketReady;

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
 * Call it to start to listen for incoming packets.
 */
void UART_StartListen(void);

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
 * Call this every millisecond
 */
void UART_Tick(void);

/**
 * Ask for next byte (private function)
 */
void UART_AskForNextByte(void);

#endif /* INCLUDE_UART_H_ */
