/*
 * HAL.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_HAL_H_
#define INCLUDE_HAL_H_

/***************
 * PORT B pins *
 ***************/

/**
 * Manipulator pin
 */
#define HAL_MANIPULATOR_PIN GPIO_PIN_0
#define HAL_MANIPULATOR_PORT GPIOB

/**
 * Display power pin
 */
#define HAL_ACTIVATE_DISPLAY_POWER_PIN GPIO_PIN_5
#define HAL_ACTIVATE_DISPLAY_POWER_PORT GPIOB

/**
 * Activate fox (turns 5V source on)
 */
#define HAL_ACTIVATE_FOX_PIN GPIO_PIN_3
#define HAL_ACTIVATE_FOX_PORT GPIOB

/**
 * Enable Bluetooth power
 */
#define HAL_ENABLE_BLUETOOTH_POWER_PIN GPIO_PIN_9
#define HAL_ENABLE_BLUETOOTH_POWER_PORT GPIOB

/**
 * Activate 3.5MHz tract
 */
#define HAL_ACTIVATE_80M_PIN GPIO_PIN_1
#define HAL_ACTIVATE_80M_PORT GPIOB

/**
 * Activate 144MHz tract
 */
#define HAL_ACTIVATE_2M_PIN GPIO_PIN_4
#define HAL_ACTIVATE_2M_PORT GPIOB

/**
 * Enable battery voltage check switch
 */
#define HAL_ENABLE_UBATT_CHECK_PIN GPIO_PIN_8
#define HAL_ENABLE_UBATT_CHECK_PORT GPIOB

/**
 * ADC channel for antenna voltage
 */
#define HAL_ADC_UANT_CHANNEL ADC_CHANNEL_0

/**
 * ADC channel for battery voltage
 */
#define HAL_ADC_UBATT_CHANNEL ADC_CHANNEL_1

/**
 * ADC channel for 3.5MHz output stage voltage
 */
#define HAL_ADC_U80M_CHANNEL ADC_CHANNEL_2

/**
 * ADC measurements count for averaging
 */
#define HAL_ADC_AVERAGING 500U

/**
 * Battery voltages
 */
#define HAL_FULL_BATTERY_VOLTS 8.0f
#define HAL_EMPTY_BATTERY_VOLTS 6.0f

/**
 * Battery voltage to charge level factors
 */
#define HAL_BATTERY_LEVEL_A 0.5f
#define HAL_BATTERY_LEVEL_B -3.0f

/**
 * 3.5MHz output stage power regulator address
 */
#define HAL_U80M_REGULATOR_BUS_ADDRESS 0x58

/**
 * Code for U80m lowest voltage
 */
#define HAL_U80M_LOWEST_VOLTAGE_CODE 0xFF

/**
 * Wait this time for regulators spin-up
 */
#define HAL_REGULATORS_SPIN_UP_TIME 500

/**
 * Base clock of frequency synthesizer
 */
#define HAL_SYNTHESIZER_BASE_CLOCK 50000000.0f

/**
 * Maximal value for synthesizer phase accumulator
 */
#define HAL_SYNTHESIZER_MAX_PHA 4294967296.0f


/**
 * FSYNC pin for frequency synthesizer
 */
#define HAL_SYNTHESIZER_FSYNC_PORT GPIOA
#define HAL_SYNTHESIZER_FSYNC_PIN GPIO_PIN_8

extern ADC_HandleTypeDef ADC_Handle;
extern L2HAL_AD5245_ContextStruct U80mRegulatorContext;
extern I2C_HandleTypeDef I2C_Other;
extern L2HAL_AD9835_ContextStruct SynthesizerContext;
extern SPI_HandleTypeDef SPIHandle;

/**
 * ADC channel in use
 */
typedef enum
{
	/* 3.5MHz antenna */
	UAnt = 0,

	/* Battery */
	UBatt = 1,

	/* 3.5MHz output stage voltage */
	U80m = 2
}
HAL_ADCChannelEnum;


/**
 * Init fox-specific hardware here
 */
void HAL_IntiHardware(void);

/**
 * Switch TX manipulator on of off
 */
void HAL_SwitchManipulator(bool isTxOn);

/**
 * Turns display power on and off
 */
void HAL_SwitchDisplayPower(bool isOn);

/**
 * Turns fox on or off
 */
void HAL_ActivateFox(bool isActivate);

/**
 * Turns on or off bluetooth module power
 */
void HAL_SwitchBluetoothPower(bool isOn);

/**
 * Turns 3.5MHz tract on or off
 */
void HAL_Activate80M(bool isActivate);

/**
 * Turns 144MHz tract on or off
 */
void HAL_Activate2M(bool isActivate);

/**
 * Enable/disable battery level check. Note, that when it enabled, it leads to some current to be pulled from battery.
 * So do not turn it on for long when device in sleep mode.
 */
void HAL_SwitchUBattCheck(bool isOn);

/**
 * Generic part of ADC setup
 */
void HAL_SetupADCGeneric(void);

/**
 * Sets ADC up for antenna signal level measurement
 */
void HAL_SetupADCForUAntMeasurement(void);

/**
 * Sets ADC up for battery level measurement
 */
void HAL_SetupADCForUBattMeasurement(void);

/**
 * Sets ADC up for antenna signal level measurement (3.5MHz)
 */
void HAL_SetupADCForU80mMeasurement(void);

/**
 * Get antenna voltage (in ADC measurements)
 */
float HAL_GetUAntADC(void);

/**
 * Get battery voltage (in ADC measurements)
 */
float HAL_GetUBattADC(void);

/**
 * Returns UBatt in volts
 */
float HAL_GetUbattVolts(void);

/**
 * Get 3.5MHz output stage voltage
 */
float HAL_GetU80mADC(void);

/**
 * Returns U80m in volts
 */
float HAL_GetU80mVolts(void);

/**
 * Returns battery charge level [0-1]
 */
float HAL_GetBatteryLevel(void);

/**
 * Sets up context for 3.5MHz output stage voltage regulator, and setting least possible voltage
 */
void HAL_ConnectToU80mRegulator(void);

/**
 * Sets 3.5MHz output stage voltage regulator code. 0x00 - maximal voltage, 0xFF - minimal
 */
void HAL_SetU80mRegulatorCode(uint8_t code);

/**
 * Get U80m regulator code
 */
uint8_t HAL_GetU80mRegulatorCode(void);

/**
 * Call this function to set callback, which will be called when new measurement of 3.5MHz output stage voltage arrives
 */
void HAL_SetU80mMeasuredCallback(void (*callback)(void));

/**
 * Determines output stage power voltage from requested fox power
 */
float HAL_GetU80mFromPower(float power);

/**
 * Inits synthesizer and puts it to sleep.
 */
void HAL_ConnectToSynthesizer(void);

/**
 * Puts synthesizer to sleep
 */
void HAL_PutSynthesizerToSleep(void);

/**
 * Wakes synthesizer up
 */
void HAL_WakeSynthesizerUp(void);

/**
 * Activates frequncy synthesizer and sets it up for required frequency
 */
void HAL_SetupSynthesizer(float frequency);


#endif /* INCLUDE_HAL_H_ */
