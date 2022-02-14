/*
 * HAL.c
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#include <HAL.h>
#include <HALPrivate.h>

void HAL_InitHardware(void)
{
	/**********
	 * PORT B *
	 **********/
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Disabling JTAG to free some pins */
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_SWJ_NOJTAG();

	GPIO_InitTypeDef GPIO_InitStruct;

	/* Manipulator (0 is off for 144MHz)*/
	GPIO_InitStruct.Pin = HAL_MANIPULATOR_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_MANIPULATOR_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_MANIPULATOR_PORT, HAL_MANIPULATOR_PIN, GPIO_PIN_RESET);

	/* Display power (1 is on) */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_DISPLAY_POWER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_DISPLAY_POWER_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ACTIVATE_DISPLAY_POWER_PORT, HAL_ACTIVATE_DISPLAY_POWER_PIN, GPIO_PIN_SET);

	/* Fox activation (1 is on, needed for bluetooth) */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_FOX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_FOX_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ACTIVATE_FOX_PORT, HAL_ACTIVATE_FOX_PIN, GPIO_PIN_SET);

	/* Bluetooth power (1 is on)*/
	GPIO_InitStruct.Pin = HAL_ENABLE_BLUETOOTH_POWER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ENABLE_BLUETOOTH_POWER_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ENABLE_BLUETOOTH_POWER_PORT, HAL_ENABLE_BLUETOOTH_POWER_PIN, GPIO_PIN_SET);

	/* Activate 3.5MHz (0 is off) */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_80M_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_80M_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ACTIVATE_80M_PORT, HAL_ACTIVATE_80M_PIN, GPIO_PIN_RESET);

	/* Activate 144MHz (0 is off) */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_2M_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_2M_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ACTIVATE_2M_PORT, HAL_ACTIVATE_2M_PIN, GPIO_PIN_RESET);

	/* Battery voltage check (0 is off)*/
	GPIO_InitStruct.Pin = HAL_ENABLE_UBATT_CHECK_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ENABLE_UBATT_CHECK_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ENABLE_UBATT_CHECK_PORT, HAL_ENABLE_UBATT_CHECK_PIN, GPIO_PIN_RESET);

	/* Synthesizer fsync (0 is off, just to avoid random signals) */
	GPIO_InitStruct.Pin = HAL_SYNTHESIZER_FSYNC_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; /* Note me! High speed leads to lots of noise */
	HAL_GPIO_Init(HAL_SYNTHESIZER_FSYNC_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_SYNTHESIZER_FSYNC_PORT, HAL_SYNTHESIZER_FSYNC_PIN, GPIO_PIN_RESET);

	/* Antenna matching (except 3th and 4th channels - they are SWDIO/SCLK) - everything is 0 = off */
	GPIO_InitStruct.Pin = HAL_AM_CHAN0_PIN | HAL_AM_CHAN1_PIN | HAL_AM_CHAN2_PIN | HAL_AM_CHAN5_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_AM_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_AM_PORT, HAL_AM_CHAN0_PIN | HAL_AM_CHAN1_PIN | HAL_AM_CHAN2_PIN | HAL_AM_CHAN5_PIN, GPIO_PIN_RESET);

	/* Antenna matching (3th and 4th channels) - everything is 0 = off */
	HAL_GPIO_DeInit(HAL_AM_PORT, HAL_AM_CHAN3_PIN | HAL_AM_CHAN4_PIN);
	GPIO_InitStruct.Pin = HAL_AM_CHAN3_PIN | HAL_AM_CHAN4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_AM_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_AM_PORT, HAL_AM_CHAN3_PIN | HAL_AM_CHAN4_PIN, GPIO_PIN_RESET);

	/* Left button */
	GPIO_InitStruct.Pin = HAL_LEFT_BUTTON_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_LEFT_BUTTON_PORT, &GPIO_InitStruct);

	/* Right button */
	GPIO_InitStruct.Pin = HAL_RIGHT_BUTTON_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_RIGHT_BUTTON_PORT, &GPIO_InitStruct);

	/* Encoder button */
	GPIO_InitStruct.Pin = HAL_ENCODER_BUTTON_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_ENCODER_BUTTON_PORT, &GPIO_InitStruct);

	/* Encoder right */
	GPIO_InitStruct.Pin = HAL_ENCODER_RIGHT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_ENCODER_RIGHT_PORT, &GPIO_InitStruct);

	/* Encoder left */
	GPIO_InitStruct.Pin = HAL_ENCODER_LEFT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_ENCODER_LEFT_PORT, &GPIO_InitStruct);

	HAL_NVIC_DisableIRQ(HAL_RIGHT_BUTTON_EXTI_LINE);
	HAL_NVIC_DisableIRQ(HAL_LEFT_BUTTON_EXTI_LINE);
	HAL_NVIC_DisableIRQ(HAL_ENCODER_BUTTON_EXTI_LINE);


	/* Enable UART (for bluetooth) */
	HAL_EnableUART();

	HAL_IsDisplayBusInitialized = false;
}

void HAL_EnableUART(void)
{
	/* UART (for bluetooth) */
	UART_Handle.Instance = HAL_BLUETOOTH_UART;
	UART_Handle.Init.BaudRate = HAL_BLUETOOTH_UART_BAUDRATE;
	UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART_Handle.Init.StopBits = UART_STOPBITS_1;
	UART_Handle.Init.Parity = UART_PARITY_NONE;
	UART_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART_Handle.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_DeInit(&UART_Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	if(HAL_UART_Init(&UART_Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void HAL_DisableUART(void)
{
	UART_Handle.Instance = HAL_BLUETOOTH_UART;
	if(HAL_UART_DeInit(&UART_Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

bool HAL_GetDisplayBusInitializationStatus(void)
{
	return HAL_IsDisplayBusInitialized;
}

void HAL_DeInitializeDisplayBus(void)
{
	if (!HAL_IsDisplayBusInitialized)
	{
		L2HAL_Error(Generic);
	}

	I2C_Display.Instance = L2HAL_DISPLAY_BUS;
	if(HAL_I2C_DeInit(&I2C_Display) != HAL_OK)
	{
		/* De-initialization Error */
		L2HAL_Error(Generic);
	}

	HAL_IsDisplayBusInitialized = false;
}

void HAL_SupressDisplayParasiticPower(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = L2HAL_DISPLAY_BUS_SCL | L2HAL_DISPLAY_BUS_SDA;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(L2HAL_DISPLAY_BUS_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(L2HAL_DISPLAY_BUS_PORT, L2HAL_DISPLAY_BUS_SCL, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L2HAL_DISPLAY_BUS_PORT, L2HAL_DISPLAY_BUS_SDA, GPIO_PIN_RESET);
}

void HAL_SwitchDisplayPower(bool isOn)
{
	if (isOn)
	{
		HAL_GPIO_WritePin(HAL_ACTIVATE_DISPLAY_POWER_PORT, HAL_ACTIVATE_DISPLAY_POWER_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HAL_ACTIVATE_DISPLAY_POWER_PORT, HAL_ACTIVATE_DISPLAY_POWER_PIN, GPIO_PIN_RESET);
	}
}

void HAL_InitializeDisplayBus(void)
{
	if (HAL_IsDisplayBusInitialized)
	{
		L2HAL_Error(Generic);
	}

	HAL_GPIO_DeInit(L2HAL_DISPLAY_BUS_PORT, L2HAL_DISPLAY_BUS_SCL | L2HAL_DISPLAY_BUS_SDA);

	L2HAL_DISPLAY_BUS_CLOCK_ENABLE();

	I2C_Display.Instance = L2HAL_DISPLAY_BUS;
	I2C_Display.Init.ClockSpeed = 400000; /* 400 KHz */
	I2C_Display.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2C_Display.Init.OwnAddress1 = 0x00;
	I2C_Display.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2C_Display.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2C_Display.Init.OwnAddress2 = 0x00;
	I2C_Display.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2C_Display.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if(HAL_I2C_Init(&I2C_Display) != HAL_OK)
	{
		/* Initialization Error */
		L2HAL_Error(Generic);
	}

	HAL_IsDisplayBusInitialized = true;
}

bool HAL_IsLeftButtonPressed(void)
{
	return GPIO_PIN_RESET == HAL_GPIO_ReadPin(HAL_LEFT_BUTTON_PORT, HAL_LEFT_BUTTON_PIN);
}

bool HAL_IsRightButtonPressed(void)
{
	return GPIO_PIN_RESET == HAL_GPIO_ReadPin(HAL_RIGHT_BUTTON_PORT, HAL_RIGHT_BUTTON_PIN);
}
