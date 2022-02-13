/*
 * UART.c
 *
 *  Created on: Feb 13, 2022
 *      Author: fossa
 */
#include <UART.h>
#include <UARTPrivate.h>

void UART_Tick(void)
{
	if (UART_RxTimeoutTimer > 0)
	{
		UART_RxTimeoutTimer --;
	}
	else
	{
		if (InProgress == UART_PSMState)
		{
			/* Timeout, aborting ongoing packet reception. */
			UART_PSMState = Listen;
			UART_RxPacketBufferIndex = 0;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	switch (UART_PSMState)
	{
		case BeforeListen:
			return;

		case Listen:
			/* First byte came */
			UART_RxTimeoutTimer = YBL_UART_NON_BLOCKING_RX_TIMEOUT;
			UART_RxPacketBuffer[UART_RxPacketBufferIndex] = UART_RxByteBuffer;

			/* Checking packet length */
			UART_ExpectedPacketLength = UART_RxPacketBuffer[0]; /* Packet length is always in first byte*/
			if (UART_ExpectedPacketLength < YBL_UART_PACKET_MIN_SIZE || UART_ExpectedPacketLength > YBL_UART_PACKET_MAX_SIZE)
			{
				UART_AskForNextByte();
				return; /* Invalid packet */
			}

			/* Moving to next state*/
			UART_RxPacketBufferIndex ++;
			UART_PSMState = InProgress;

			UART_AskForNextByte();

			break;

		case InProgress:
			UART_RxTimeoutTimer = YBL_UART_NON_BLOCKING_RX_TIMEOUT;
			UART_RxPacketBuffer[UART_RxPacketBufferIndex] = UART_RxByteBuffer;
			UART_RxPacketBufferIndex ++;

			if (UART_RxPacketBufferIndex == UART_ExpectedPacketLength)
			{
				/* We have a new packet */
				if (UART_IsPacketReady)
				{
					/* Previous packet isn't processed yet, dying */
					L2HAL_Error(Generic);
				}

				UART_PSMState = Listen;
				UART_RxPacketBufferIndex = 0;

				UART_ReceivedPacketFullLength = UART_ExpectedPacketLength;
				memcpy(UART_ReceivedPacket, UART_RxPacketBuffer, UART_ReceivedPacketFullLength);
				UART_IsPacketReady = true;
			}

			UART_AskForNextByte();

			break;

		default:
			L2HAL_Error(Generic);
			break;
	}
}

void UART_StartListen(void)
{
	UART_PSMState = Listen;
	UART_RxPacketBufferIndex = 0;
	UART_ExpectedPacketLength = 0;
	UART_RxTimeoutTimer = YBL_UART_NON_BLOCKING_RX_TIMEOUT;

	UART_AskForNextByte();
}

void UART_AbortListen(void)
{
	UART_PSMState = BeforeListen;

	if (HAL_UART_AbortReceive(&UART_Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void UART_SendBlocking(uint8_t* message, uint8_t size)
{
	// To be sure that data wouldn't be changed via interrupt
	uint8_t tmpBuffer[YBL_UART_PACKET_MAX_SIZE];
	memcpy(tmpBuffer, message, size);

	if (HAL_UART_Transmit(&UART_Handle, tmpBuffer, size, YBL_UART_BLOCKING_TRANSFER_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void UART_ReadBlocking(uint8_t* buffer, uint8_t size)
{
	if (HAL_UART_Receive(&UART_Handle, buffer, size, YBL_UART_BLOCKING_TRANSFER_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void UART_Init(void)
{
	UART_PSMState = BeforeListen;
	UART_IsPacketReady = false;

	L2HAL_SysTick_RegisterHandler(&UART_Tick);
}

void UART_AskForNextByte(void)
{
	if(HAL_UART_Receive_IT(&UART_Handle, &UART_RxByteBuffer, 1) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}
