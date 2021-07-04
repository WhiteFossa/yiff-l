/*
 * HAL.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>
#include <HALPrivate.h>

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

	/**
	 * Variables initial state
	 */

	HAL_BatteryLevelADC = 0;
	HAL_ADCAccumulator = 0;
	HAL_ADCAveragesCounter = 0;

	/**
	 * Launching ADC conversions
	 */
	HAL_SetupADCGeneric();
	HAL_SetupADCForUAntMeasurement();

	/**
	 * Setting up 3.5MHz output stage power source
	 */
	HAL_ConnectToU80mRegulator();
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

void HAL_SetupADCGeneric(void)
{
	ADC_Handle.Instance = ADC1;
	ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_Handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
	ADC_Handle.Init.ContinuousConvMode = ENABLE;
	ADC_Handle.Init.NbrOfConversion = 1;
	ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
	ADC_Handle.Init.NbrOfDiscConversion = 1;
	ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;

	if (HAL_ADC_Init(&ADC_Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void HAL_SetupADCForUAntMeasurement(void)
{
	HAL_ADC_Stop_IT(&ADC_Handle);

	HAL_CurrentADCChannel = UAnt;

	ADC_ChannelConfTypeDef adcChannelConfig;
	adcChannelConfig.Channel = HAL_ADC_UANT_CHANNEL;
	adcChannelConfig.Rank = ADC_REGULAR_RANK_1;
	adcChannelConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

	if (HAL_ADC_ConfigChannel(&ADC_Handle, &adcChannelConfig) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	HAL_ADC_Start_IT(&ADC_Handle);
}

void HAL_SetupADCForUBattMeasurement(void)
{
	HAL_ADC_Stop_IT(&ADC_Handle);

	HAL_CurrentADCChannel = UBatt;

	ADC_ChannelConfTypeDef adcChannelConfig;
	adcChannelConfig.Channel = HAL_ADC_UBATT_CHANNEL;
	adcChannelConfig.Rank = ADC_REGULAR_RANK_1;
	adcChannelConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

	if (HAL_ADC_ConfigChannel(&ADC_Handle, &adcChannelConfig) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	HAL_ADC_Start_IT(&ADC_Handle);
}

void HAL_SetupADCForU80mMeasurement(void)
{
	HAL_ADC_Stop_IT(&ADC_Handle);

	HAL_CurrentADCChannel = U80m;

	ADC_ChannelConfTypeDef adcChannelConfig;
	adcChannelConfig.Channel = HAL_ADC_U80M_CHANNEL;
	adcChannelConfig.Rank = ADC_REGULAR_RANK_1;
	adcChannelConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;

	if (HAL_ADC_ConfigChannel(&ADC_Handle, &adcChannelConfig) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	HAL_ADC_Start_IT(&ADC_Handle);
}

/**
 * ADC callback
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	if (AdcHandle != &ADC_Handle)
	{
		return;
	}

	HAL_AddNewADCMeasurement((uint16_t)HAL_ADC_GetValue(&ADC_Handle));
}

void HAL_AddNewADCMeasurement(uint16_t measurement)
{
	if (HAL_ADCAveragesCounter < HAL_ADC_AVERAGING)
	{
		HAL_ADCAccumulator += measurement;
		HAL_ADCAveragesCounter ++;
		return;
	}

	float averaged = HAL_ADCAccumulator / (float)HAL_ADC_AVERAGING;
	HAL_ADCAccumulator = 0;
	HAL_ADCAveragesCounter = 0;

	/* Writing result and switching to next channel */
	switch (HAL_CurrentADCChannel)
	{
		case UAnt:
			HAL_AntennaLevelADC = averaged;
			HAL_SetupADCForUBattMeasurement();
			break;

		case UBatt:
			HAL_BatteryLevelADC = averaged;
			HAL_SetupADCForU80mMeasurement();
			break;

		case U80m:
			HAL_80mLevelADC = averaged;
			HAL_SetupADCForUAntMeasurement();
			break;
	}
}

float HAL_GetUAntADC(void)
{
	return HAL_AntennaLevelADC;
}

float HAL_GetUBattADC(void)
{
	return HAL_BatteryLevelADC;
}

float HAL_GetU80mADC(void)
{
	return HAL_80mLevelADC;
}

float HAL_GetUbattVolts(void)
{
	float adc = HAL_GetUBattADC();

	return EEPROM_Header.UBattADCA * adc + EEPROM_Header.UBattADCB;
}

float HAL_GetU80mVolts(void)
{
	float adc = HAL_GetU80mADC();

	return EEPROM_Header.U80mADCA * adc + EEPROM_Header.U80mADCB;
}

float HAL_GetBatteryLevel(void)
{
	float voltage = HAL_GetUbattVolts();

	float result = HAL_BATTERY_LEVEL_A * voltage + HAL_BATTERY_LEVEL_B;

	if (result < 0)
	{
		result = 0;
	}
	else if (result > 1)
	{
		result = 1;
	}

	return result;
}

void HAL_ConnectToU80mRegulator(void)
{
	U80mRegulatorContext = L2HAL_AD5245_DetectDACAtAddress(&I2C_Other, HAL_U80M_REGULATOR_BUS_ADDRESS);

	if (!U80mRegulatorContext.IsFound)
	{
		L2HAL_Error(Generic);
	}

	L2HAL_AD5245_SetValue(&U80mRegulatorContext, 0x00);
}
