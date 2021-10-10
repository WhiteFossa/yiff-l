/*
 * HAL.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <HAL.h>
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
	HAL_GPIO_WritePin(HAL_MANIPULATOR_PORT, HAL_MANIPULATOR_PIN, GPIO_PIN_RESET);

	/* Display power*/
	GPIO_InitStruct.Pin = HAL_ACTIVATE_DISPLAY_POWER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_DISPLAY_POWER_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ACTIVATE_DISPLAY_POWER_PORT, HAL_ACTIVATE_DISPLAY_POWER_PIN, GPIO_PIN_RESET);

	/* Fox activation */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_FOX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_FOX_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ACTIVATE_FOX_PORT, HAL_ACTIVATE_FOX_PIN, GPIO_PIN_RESET);

	/* Bluetooth power */
	GPIO_InitStruct.Pin = HAL_ENABLE_BLUETOOTH_POWER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ENABLE_BLUETOOTH_POWER_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ENABLE_BLUETOOTH_POWER_PORT, HAL_ENABLE_BLUETOOTH_POWER_PIN, GPIO_PIN_RESET);

	/* Activate 3.5MHz */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_80M_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_80M_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ACTIVATE_80M_PORT, HAL_ACTIVATE_80M_PIN, GPIO_PIN_RESET);

	/* Activate 144MHz */
	GPIO_InitStruct.Pin = HAL_ACTIVATE_2M_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ACTIVATE_2M_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ACTIVATE_2M_PORT, HAL_ACTIVATE_2M_PIN, GPIO_PIN_RESET);

	/* Battery voltage check */
	GPIO_InitStruct.Pin = HAL_ENABLE_UBATT_CHECK_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_ENABLE_UBATT_CHECK_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_ENABLE_UBATT_CHECK_PORT, HAL_ENABLE_UBATT_CHECK_PIN, GPIO_PIN_RESET);

	/* Synthesizer fsync */
	GPIO_InitStruct.Pin = HAL_SYNTHESIZER_FSYNC_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; /* Note me! High speed leads to lots of noise */
	HAL_GPIO_Init(HAL_SYNTHESIZER_FSYNC_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_SYNTHESIZER_FSYNC_PORT, HAL_SYNTHESIZER_FSYNC_PIN, GPIO_PIN_RESET);

	/* Antenna matching (except 3th and 4th channels - they are SWDIO/SCLK) */
	GPIO_InitStruct.Pin = HAL_AM_CHAN0_PIN | HAL_AM_CHAN1_PIN | HAL_AM_CHAN2_PIN | HAL_AM_CHAN5_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_AM_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_AM_PORT, HAL_AM_CHAN0_PIN | HAL_AM_CHAN1_PIN | HAL_AM_CHAN2_PIN | HAL_AM_CHAN5_PIN, GPIO_PIN_RESET);

	/* Antenna matching (3th and 4th channels) */
	HAL_GPIO_DeInit(GPIOA, HAL_AM_CHAN3_PIN | HAL_AM_CHAN4_PIN);
	GPIO_InitStruct.Pin = HAL_AM_CHAN3_PIN | HAL_AM_CHAN4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(HAL_AM_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(HAL_AM_PORT, HAL_AM_CHAN3_PIN | HAL_AM_CHAN4_PIN, GPIO_PIN_RESET);

	/* Left button (interrupt) */
	GPIO_InitStruct.Pin = HAL_LEFT_BUTTON_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_LEFT_BUTTON_PORT, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(HAL_LEFT_BUTTON_EXTI_LINE, 0, 0);
	HAL_NVIC_EnableIRQ(HAL_LEFT_BUTTON_EXTI_LINE);

	/* Right button (interrupt) */
	GPIO_InitStruct.Pin = HAL_RIGHT_BUTTON_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_RIGHT_BUTTON_PORT, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(HAL_RIGHT_BUTTON_EXTI_LINE, 0, 0);
	HAL_NVIC_EnableIRQ(HAL_RIGHT_BUTTON_EXTI_LINE);

	/* Encoder button (interrupt) */
	GPIO_InitStruct.Pin = HAL_ENCODER_BUTTON_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_ENCODER_BUTTON_PORT, &GPIO_InitStruct);

	/* Encoder right (interrupt) */
	GPIO_InitStruct.Pin = HAL_ENCODER_RIGHT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_ENCODER_RIGHT_PORT, &GPIO_InitStruct);

	/* Encoder left (no interrupt) */
	GPIO_InitStruct.Pin = HAL_ENCODER_LEFT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HAL_ENCODER_LEFT_PORT, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(HAL_ENCODER_BUTTON_EXTI_LINE, 0, 0);
	HAL_NVIC_EnableIRQ(HAL_ENCODER_BUTTON_EXTI_LINE);

	/**
	 * Variables initial state
	 */
	HAL_BatteryLevelADC = 0;
	HAL_ADCAccumulator = 0;
	HAL_ADCAveragesCounter = 0;
	HAL_U80mNewMeasurementCallback = NULL;

	HAL_LeftButtonCallback = NULL;
	HAL_RightButtonCallback = NULL;
	HAL_EncoderButtonCallback = NULL;
	HAL_EncoderRotationCallback = NULL;
	HAL_EncoderPosition = 0;

	HAL_LeftButtonNoiseSuppressionCounter = 0;
	HAL_EncoderButtonNoiseSuppressionCounter = 0;
	HAL_RightButtonNoiseSuppressionCounter = 0;

	/**
	 * Launching ADC conversions
	 */
	HAL_SetupADCGeneric();
	HAL_SetupADCForUAntMeasurement();

	/**
	 * Setting up 3.5MHz output stage power source
	 */
	HAL_ConnectToU80mRegulator();

	/**
	 * Connecting to frequency synthesizer
	 */
	HAL_ConnectToSynthesizer();
}

void HAL_SwitchManipulator(bool isTxOn)
{
	if (isTxOn)
	{
		if (FoxState.Frequency.Is144MHz)
		{
			/* Turn tone generator on, active is high */
			HAL_Enable2mToneGenerator();
		}
		else
		{
			HAL_GPIO_WritePin(HAL_MANIPULATOR_PORT, HAL_MANIPULATOR_PIN, GPIO_PIN_RESET); /* Active is low */
		}
	}
	else
	{
		if (FoxState.Frequency.Is144MHz)
		{
			/* Turn tone generator off */
			HAL_Disable2mToneGenerator();
			HAL_GPIO_WritePin(HAL_MANIPULATOR_PORT, HAL_MANIPULATOR_PIN, GPIO_PIN_RESET); /* Inactive is low*/
		}
		else
		{
			HAL_GPIO_WritePin(HAL_MANIPULATOR_PORT, HAL_MANIPULATOR_PIN, GPIO_PIN_SET); /* Inactive is high */
		}
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
	HAL_SwitchAntennaMatching(isActivate);

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

	HAL_CurrentADCChannel = YHL_HAL_UAnt;

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

	HAL_CurrentADCChannel = YHL_HAL_UBatt;

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

	HAL_CurrentADCChannel = YHL_HAL_U80m;

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
		case YHL_HAL_UAnt:
			HAL_AntennaLevelADC = averaged;
			HAL_SetupADCForUBattMeasurement();
			break;

		case YHL_HAL_UBatt:
			HAL_BatteryLevelADC = averaged;
			HAL_SetupADCForU80mMeasurement();
			break;

		case YHL_HAL_U80m:
			HAL_80mLevelADC = averaged;

			if (HAL_U80mNewMeasurementCallback != NULL)
			{
				HAL_U80mNewMeasurementCallback();
			}

			HAL_SetupADCForUAntMeasurement();
			break;

		default:
			L2HAL_Error(Generic);
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

	HAL_SetU80mRegulatorCode(HAL_U80M_LOWEST_VOLTAGE_CODE); // Least possible voltage
}

void HAL_SetU80mRegulatorCode(uint8_t code)
{
	HAL_CurrentU80mCode = code;
	L2HAL_AD5245_SetValue(&U80mRegulatorContext, code);
}

uint8_t HAL_GetU80mRegulatorCode(void)
{
	return HAL_CurrentU80mCode;
}

void HAL_SetU80mMeasuredCallback(void (*callback)(void))
{
	HAL_U80mNewMeasurementCallback = callback;
}

float HAL_GetU80mFromPower(float power)
{
	float result = EEPROM_Header.P80mA * power + EEPROM_Header.P80mB;

	if (result > YHL_HL_MAX_U80M)
	{
		result = YHL_HL_MAX_U80M;
	}
	else if (result < YHL_HL_MIN_U80M)
	{
		result = YHL_HL_MIN_U80M;
	}

	return result;
}

void HAL_ConnectToSynthesizer(void)
{
	SynthesizerContext.SPIHandle = &SPIHandle;
	SynthesizerContext.FSYNCPort = GPIOA;
	SynthesizerContext.FSYNCPin = GPIO_PIN_8;
	L2HAL_AD9835_Init(&SynthesizerContext);

	HAL_PutSynthesizerToSleep();
}

void HAL_PutSynthesizerToSleep(void)
{
	L2HAL_AD9835_PowerControl(&SynthesizerContext, true, false, false);
}

void HAL_WakeSynthesizerUp(void)
{
	L2HAL_AD9835_PowerControl(&SynthesizerContext, false, false, false);
}

void HAL_SetupSynthesizer(float frequency)
{
	uint32_t frequencyWord = (uint32_t)floor(HAL_SYNTHESIZER_MAX_PHA * frequency / HAL_SYNTHESIZER_BASE_CLOCK+ 0.5f);

	L2HAL_AD9835_WriteFrequencyWord(&SynthesizerContext, Freg0, frequencyWord);
	HAL_WakeSynthesizerUp();
}

void HAL_Enable2mToneGenerator(void)
{
	if (HAL_TIM_OC_GetState(&ToneTimerHandle) == HAL_TIM_STATE_READY)
	{
		return;
	}

	/* Setting up timer */
	ToneTimerHandle.Instance = HAL_TONE_TIMER;

	ToneTimerHandle.Init.Prescaler = HAL_TONE_TIMER_PRESCALER;
	ToneTimerHandle.Init.Period = HAL_TONE_TIMER_PERIOD;
	ToneTimerHandle.Init.ClockDivision = 0;
	ToneTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	ToneTimerHandle.Init.RepetitionCounter = 0;
	ToneTimerHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_OC_Init(&ToneTimerHandle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	/* Setting up timer output channel */
	TIM_OC_InitTypeDef channel;
	channel.OCMode = TIM_OCMODE_TOGGLE;
	channel.Pulse = 0;
	channel.OCPolarity = TIM_OCPOLARITY_HIGH;
	channel.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	channel.OCFastMode = TIM_OCFAST_DISABLE;
	channel.OCIdleState = TIM_OCIDLESTATE_RESET;
	channel.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	if (HAL_TIM_OC_ConfigChannel(&ToneTimerHandle, &channel, HAL_TONE_TIMER_CHANNEL) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	if (HAL_TIM_OC_Start(&ToneTimerHandle, HAL_TONE_TIMER_CHANNEL) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void HAL_Disable2mToneGenerator(void)
{
	if (HAL_TIM_OC_GetState(&ToneTimerHandle) == HAL_TIM_STATE_RESET)
	{
		return;
	}

	if (HAL_TIM_OC_Stop(&ToneTimerHandle, HAL_TONE_TIMER_CHANNEL) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	HAL_TIM_OC_DeInit(&ToneTimerHandle);
}

void HAL_SwitchAntennaMatchingChannel(YHL_HAL_AMChannelEnum channel, bool isOn)
{
	uint16_t pin;

	switch (channel)
	{
		case YHL_HAL_AM0:
			pin = HAL_AM_CHAN0_PIN;
			break;

		case YHL_HAL_AM1:
			pin = HAL_AM_CHAN1_PIN;
			break;

		case YHL_HAL_AM2:
			pin = HAL_AM_CHAN2_PIN;
			break;

		case YHL_HAL_AM3:
			pin = HAL_AM_CHAN3_PIN;
			break;

		case YHL_HAL_AM4:
			pin = HAL_AM_CHAN4_PIN;
			break;

		case YHL_HAL_AM5:
			pin = HAL_AM_CHAN5_PIN;
			break;

		default:
			L2HAL_Error(Generic);
			break;
	}

	GPIO_PinState pinState;
	if (isOn)
	{
		pinState = GPIO_PIN_SET;
	}
	else
	{
		pinState = GPIO_PIN_RESET;
	}

	HAL_GPIO_WritePin(HAL_AM_PORT, pin, pinState);
}

void HAL_SetAntennaMatchingValue(uint8_t value)
{
	if (value > HAL_AM_MAX_VALUE)
	{
		L2HAL_Error(Generic);
	}

	/* Supressing carrier to decrease switching damage for relays */
	FoxState.ForceCarrierOff = true;
	HL_ProcessManipulatorFoxStateChange();

	HAL_Delay(HAL_AM_PAUSE);

	HAL_SwitchAntennaMatchingChannel(YHL_HAL_AM0, (value & 0b1) != 0);
	HAL_SwitchAntennaMatchingChannel(YHL_HAL_AM1, (value & 0b10) != 0);
	HAL_SwitchAntennaMatchingChannel(YHL_HAL_AM2, (value & 0b100) != 0);
	HAL_SwitchAntennaMatchingChannel(YHL_HAL_AM3, (value & 0b1000) != 0);
	HAL_SwitchAntennaMatchingChannel(YHL_HAL_AM4, (value & 0b10000) != 0);
	HAL_SwitchAntennaMatchingChannel(YHL_HAL_AM5, (value & 0b100000) != 0);

	HAL_Delay(HAL_AM_PAUSE);

	/* Restoring carrier */
	FoxState.ForceCarrierOff = false;
	HL_ProcessManipulatorFoxStateChange();
}

void HAL_SwitchAntennaMatching(bool isOn)
{
	if (isOn)
	{
		/* Disabling SWD, because we use SWD pins for antenna matching */
		//__HAL_AFIO_REMAP_SWJ_DISABLE();
	}
	else
	{
		/* Re-enabling SWD */
		//__HAL_AFIO_REMAP_SWJ_NOJTAG();
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (HAL_LEFT_BUTTON_PIN == GPIO_Pin)
	{
		HAL_OnLeftButtonPressed();
	}
	else if (HAL_RIGHT_BUTTON_PIN == GPIO_Pin)
	{
		HAL_OnRightButtonPressed();
	}
	else if (HAL_ENCODER_BUTTON_PIN == GPIO_Pin)
	{
		HAL_OnEncoderButtonPressed();
	}
	else if (HAL_ENCODER_RIGHT_PIN == GPIO_Pin)
	{
		HAL_OnEncoderTurned();
	}
}

void HAL_OnLeftButtonPressed(void)
{
	if (HAL_LeftButtonNoiseSuppressionCounter > 0)
	{
		return;
	}

	HAL_LeftButtonNoiseSuppressionCounter = HAL_BUTTONS_NOISE_SUPPRESSION_INTERVAL;

	if (HAL_LeftButtonCallback != NULL)
	{
		HAL_LeftButtonCallback();
	}
}

void HAL_OnRightButtonPressed(void)
{
	if (HAL_RightButtonNoiseSuppressionCounter > 0)
	{
		return;
	}

	HAL_RightButtonNoiseSuppressionCounter = HAL_BUTTONS_NOISE_SUPPRESSION_INTERVAL;

	if (HAL_RightButtonCallback != NULL)
	{
		HAL_RightButtonCallback();
	}
}

void HAL_OnEncoderButtonPressed(void)
{
	if (HAL_EncoderButtonNoiseSuppressionCounter > 0)
	{
		return;
	}

	HAL_EncoderButtonNoiseSuppressionCounter = HAL_BUTTONS_NOISE_SUPPRESSION_INTERVAL;

	if (HAL_EncoderButtonCallback != NULL)
	{
		HAL_EncoderButtonCallback();
	}
}

void HAL_OnEncoderTurned(void)
{
	uint8_t sum = 0;
	for (uint8_t sample = 0; sample < HAL_ENCODER_SAMPLES_COUNT; sample ++)
	{
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(HAL_ENCODER_LEFT_PORT, HAL_ENCODER_LEFT_PIN))
		{
			sum++;
		}
	}

	if (sum > (HAL_ENCODER_SAMPLES_COUNT / 2 - 1))
	{
		HAL_EncoderPosition --;
	}
	else
	{
		HAL_EncoderPosition ++;
	}

	if (HAL_EncoderPosition >= HAL_ENCODER_DIVISION)
	{
		HAL_EncoderPosition = 0;

		if (HAL_EncoderRotationCallback != NULL)
		{
			HAL_EncoderRotationCallback(HAL_ENCODER_ROTATION_CLOCKWISE);
		}
	}
	else if (HAL_EncoderPosition <= -1 * HAL_ENCODER_DIVISION)
	{
		HAL_EncoderPosition = 0;

		if (HAL_EncoderRotationCallback != NULL)
		{
			HAL_EncoderRotationCallback(HAL_ENCODER_ROTATION_COUNTERCLOCKWISE);
		}
	}
}

void HAL_RegisterLeftButtonHandler(void (*handler)(void))
{
	HAL_LeftButtonCallback = handler;
}

void HAL_RegisterRightButtonHandler(void (*handler)(void))
{
	HAL_RightButtonCallback = handler;
}

void HAL_RegisterEncoderButtonHandler(void (*handler)(void))
{
	HAL_EncoderButtonCallback = handler;
}

void HAL_RegisterEncoderRotationHandler(void (*handler)(int8_t))
{
	HAL_EncoderRotationCallback = handler;
}

void HAL_OnTick(void)
{
	if (HAL_LeftButtonNoiseSuppressionCounter > 0)
	{
		HAL_LeftButtonNoiseSuppressionCounter --;
	}

	if (HAL_EncoderButtonNoiseSuppressionCounter > 0)
	{
		HAL_EncoderButtonNoiseSuppressionCounter --;
	}

	if (HAL_RightButtonNoiseSuppressionCounter > 0)
	{
		HAL_RightButtonNoiseSuppressionCounter --;
	}
}
