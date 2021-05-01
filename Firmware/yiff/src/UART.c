/*
 * UART.c
 *
 *  Created on: Apr 27, 2021
 *      Author: fossa
 */

#include <main.h>

void UART_Tick()
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

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UART_TransmissionInProgress)
	{
		free(UART_TxBuffer);
		UART_TxBuffer = NULL;
		UART_TransmissionInProgress = false;
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
			UART_RxTimeoutTimer = YHL_UART_NON_BLOCKING_RX_TIMEOUT;
			UART_RxPacketBuffer[UART_RxPacketBufferIndex] = UART_RxByteBuffer;
			UART_RxPacketBufferIndex ++;

			UART_PSMState = InProgress;

			/* Asking for a next byte */
			if(HAL_UART_Receive_IT(&UART_Handle, &UART_RxByteBuffer, 1) != HAL_OK)
			{
				L2HAL_Error(Generic);
			}

			break;

		case InProgress:
			UART_RxTimeoutTimer = YHL_UART_NON_BLOCKING_RX_TIMEOUT;
			UART_RxPacketBuffer[UART_RxPacketBufferIndex] = UART_RxByteBuffer;
			UART_RxPacketBufferIndex ++;

			if (UART_RxPacketBufferIndex == YHL_UART_PACKET_SIZE)
			{
				/* We have a new packet */
				UART_PSMState = Listen;
				UART_RxPacketBufferIndex = 0;

				(*UART_OnNewPacket)(UART_RxPacketBuffer);
			}

			/* Asking for next (or first) byte */
			if(HAL_UART_Receive_IT(&UART_Handle, &UART_RxByteBuffer, 1) != HAL_OK)
			{
				L2HAL_Error(Generic);
			}

			break;

		default:
			L2HAL_Error(Generic);
			break;
	}
}

void UART_StartListen(void (*onNewPacketFunction)(uint8_t packet[YHL_UART_PACKET_SIZE]))
{
	UART_PSMState = Listen;
	UART_RxPacketBufferIndex = 0;
	UART_RxTimeoutTimer = YHL_UART_NON_BLOCKING_RX_TIMEOUT;
	UART_OnNewPacket = onNewPacketFunction;

	if(HAL_UART_Receive_IT(&UART_Handle, &UART_RxByteBuffer, 1) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void UART_AbortListen(void)
{
	UART_PSMState = BeforeListen;
	UART_RxPacketBufferIndex = 0;

	if (HAL_UART_AbortReceive(&UART_Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void UART_SendBlocking(uint8_t* message, uint8_t size)
{
	uint8_t* tmpBuffer = malloc(size);
	memcpy(tmpBuffer, message, size);

	if (HAL_UART_Transmit(&UART_Handle, tmpBuffer, size, YHL_UART_BLOCKING_TRANSFER_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	free(tmpBuffer);
}

void UART_ReadBlocking(uint8_t* buffer, uint8_t size)
{
	if (HAL_UART_Receive(&UART_Handle, buffer, size, YHL_UART_BLOCKING_TRANSFER_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void UART_Init(void)
{
	UART_TransmissionInProgress = false;
	UART_TxBuffer = NULL;
	UART_PSMState = BeforeListen;

	L2HAL_SysTick_RegisterHandler(&UART_Tick);
}

void UART_SendSemiBlocking(uint8_t* message, uint8_t size)
{
	while(UART_TransmissionInProgress) {}

	UART_TransmissionInProgress = true;
	UART_TxBuffer = malloc(size);
	memcpy(UART_TxBuffer, message, size);

	if (HAL_UART_Transmit_IT(&UART_Handle, UART_TxBuffer, size) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}
