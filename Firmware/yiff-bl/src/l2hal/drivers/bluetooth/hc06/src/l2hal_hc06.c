/*
 * l2hal_hc06.c
 *
 *  Created on: May 3, 2021
 *      Author: fossa
 */

#include <l2hal_hc06.h>

L2HAL_HC06_ContextStruct L2HAL_HC06_AttachToDevice(UART_HandleTypeDef* uart)
{
	/* Cancelling current UART exchange */
	if (HAL_UART_Abort(uart) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	L2HAL_HC06_ContextStruct context;
	context.IsFound = false;
	context.UART_Handle = uart;

	uint8_t bufferSize = 3;
	char buffer[bufferSize];
	snprintf(buffer, bufferSize, "AT");

	if (HAL_UART_Transmit(context.UART_Handle, (uint8_t*)buffer, 2, L2HAL_HC06_UART_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	/* Immediately expecting an answer (Somehow HC-06 may return either OK or \0OK. Probably we aren't
	 * fast enough */

	/* We MUST NOT check for received status - it might be timeout, because we don't know how many bytes
	 * will be returned */
	HAL_UART_Receive(context.UART_Handle, (uint8_t*)buffer, 3, L2HAL_HC06_UART_TIMEOUT);

	if ((buffer[0] == 'O' && buffer[1] == 'K')
		|| (buffer[0] == 0x00 && buffer[1] == 'O' && buffer[2] == 'K'))
	{
		context.IsFound = true;
	}
	else
	{
		context.IsFound = false;
	}

	return context;

	return context;
}

void L2HAL_HC06_SetName(L2HAL_HC06_ContextStruct* context, const char* name)
{

	if (strlen(name) > L2HAL_HC06_MAX_NAME_LENGTH)
	{
		L2HAL_Error(Generic);
	}

	char processedName[L2HAL_HC06_MAX_NAME_MEMORY_SIZE];
	strncpy(processedName, name, L2HAL_HC06_MAX_NAME_MEMORY_SIZE);

	for (uint8_t i = 0; i < L2HAL_HC06_MAX_NAME_LENGTH; i++)
	{
		if (0x00 == processedName[i])
		{
			break;
		}
		else if (0x20 == processedName[i]) /* Space */
		{
			processedName[i] = 0x5f; /* Underscore */
		}
	}

	uint8_t bufferSize = 32;
	char buffer[bufferSize];
	snprintf(buffer, bufferSize, "AT+NAME%s", processedName);

	uint8_t sendLength = (uint8_t)strlen(buffer);
	if (HAL_UART_Transmit(context->UART_Handle, (uint8_t*)buffer, sendLength , L2HAL_HC06_UART_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	/* Immediately expecting an answer */
	if (HAL_UART_Receive(context->UART_Handle, (uint8_t*)buffer, 9, L2HAL_HC06_UART_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	buffer[9] = 0x00; /* Null-terminating by paws */

	if (strcmp("OKsetname", buffer) != 0)
	{
		L2HAL_Error(Generic);
	}
}

void L2HAL_HC06_SetPIN(L2HAL_HC06_ContextStruct* context, const char pin[L2HAL_HC06_PIN_CODE_LENGTH])
{
	uint8_t bufferSize = 16;
	char buffer[bufferSize];
	snprintf(buffer, bufferSize, "AT+PIN%s", pin);

	uint8_t sendLength = (uint8_t)strlen(buffer);
	if (HAL_UART_Transmit(context->UART_Handle, (uint8_t*)buffer, sendLength, L2HAL_HC06_UART_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	/* Immediately expecting an answer */
	if (HAL_UART_Receive(context->UART_Handle, (uint8_t*)buffer, 8, L2HAL_HC06_UART_TIMEOUT) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	buffer[8] = 0x00; /* Null-terminating by paws */

	if (strcmp("OKsetPIN", buffer) != 0)
	{
		L2HAL_Error(Generic);
	}
}
