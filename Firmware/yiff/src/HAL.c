/*
 * HAL.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

void HAL_IntiHardware(void)
{
	/**********
	 * PORT B *
	 **********/
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Disabling JTAG to free some pins */
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_SWJ_NOJTAG();

	GPIO_InitTypeDef GPIO_InitStruct;

	/* Manipulator */
	GPIO_InitStruct.Pin = HAL_MANIPULATOR_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_MANIPULATOR_PORT, &GPIO_InitStruct);

	/* Display power*/
	GPIO_InitStruct.Pin = HAL_ACTIVATE_DISPLAY_POWER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_DISPLAY_POWER_PORT, &GPIO_InitStruct);

	/* Fox activation */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_FOX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_FOX_PORT, &GPIO_InitStruct);

	/* Bluetooth power */
	GPIO_InitStruct.Pin = HAL_ENABLE_BLUETOOTH_POWER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ENABLE_BLUETOOTH_POWER_PORT, &GPIO_InitStruct);

	/* Activate 3.5MHz */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_80M_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_80M_PORT, &GPIO_InitStruct);

	/* Battery voltage check */
	GPIO_InitStruct.Pin = HAL_ENABLE_UBATT_CHECK_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ENABLE_UBATT_CHECK_PORT, &GPIO_InitStruct);
}

void HAL_SwitchManipulator(bool isTxOn)
{
	if (isTxOn)
	{
		HAL_GPIO_WritePin(HAL_MANIPULATOR_PORT, HAL_MANIPULATOR_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HAL_MANIPULATOR_PORT, HAL_MANIPULATOR_PIN, GPIO_PIN_RESET);
	}

	FoxState.IsTXOn = isTxOn;
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

void HAL_ActivateFox(bool isActivate)
{
	if (isActivate)
	{
		HAL_GPIO_WritePin(HAL_ACTIVATE_FOX_PORT, HAL_ACTIVATE_FOX_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HAL_ACTIVATE_FOX_PORT, HAL_ACTIVATE_FOX_PIN, GPIO_PIN_RESET);
	}
}

void HAL_SwitchBluetoothPower(bool isOn)
{
	if (isOn)
	{
		HAL_GPIO_WritePin(HAL_ENABLE_BLUETOOTH_POWER_PORT, HAL_ENABLE_BLUETOOTH_POWER_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HAL_ENABLE_BLUETOOTH_POWER_PORT, HAL_ENABLE_BLUETOOTH_POWER_PIN, GPIO_PIN_RESET);
	}
}

void HAL_Activate80M(bool isActivate)
{
	if (isActivate)
	{
		HAL_GPIO_WritePin(HAL_ACTIVATE_80M_PORT, HAL_ACTIVATE_80M_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HAL_ACTIVATE_80M_PORT, HAL_ACTIVATE_80M_PIN, GPIO_PIN_RESET);
	}
}

void HAL_Activate2M(bool isActivate)
{
	if (isActivate)
	{
		HAL_GPIO_WritePin(HAL_ACTIVATE_2M_PORT, HAL_ACTIVATE_2M_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HAL_ACTIVATE_2M_PORT, HAL_ACTIVATE_2M_PIN, GPIO_PIN_RESET);
	}
}

void HAL_SwitchUBattCheck(bool isOn)
{
	if (isOn)
	{
		HAL_GPIO_WritePin(HAL_ENABLE_UBATT_CHECK_PORT, HAL_ENABLE_UBATT_CHECK_PIN, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HAL_ENABLE_UBATT_CHECK_PORT, HAL_ENABLE_UBATT_CHECK_PIN, GPIO_PIN_RESET);
	}
}
