/*
 * HAL.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_HAL_H_
#define INCLUDE_HAL_H_

#include <l2hal.h>
#include <FoxStatus.h>
#include <EEPROM.h>
#include <HardwareLogic.h>
#include <math.h>
#include <SelfDiagnostics.h>
#include <Random.h>

extern EEPROMHeaderStruct EEPROM_Header;
extern FoxStateStruct FoxState;

/*******************
 * Debugging stuff *
 *******************/

/**
 * If defined, then antenna matching will be disabled, and SWD will be kept enabled.
 * Otherwise SWD connection will be dropped when antenna matching starts
 */
/* #define HAL_DISABLE_ANTENNA_MATCHING */

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


/**
 * Timer clock is 12MHz
 */
#define HAL_TONE_TIMER_PRESCALER (uint32_t)((SystemCoreClock / 12000000) - 1);

/**
 * Period is 6KHz, so compares happens at 2KHz frequency, which leads to 1KHz tone (because of TIM_OCMODE_TOGGLE)
 */
#define HAL_TONE_TIMER_PERIOD 6000 - 1;

/**
 * Timer itself
 */
#define HAL_TONE_TIMER TIM3

/**
 * Tone timer channel
 */
#define HAL_TONE_TIMER_CHANNEL TIM_CHANNEL_3

/**
 * 3.5 MHz antenna matching stuff
 */
#define HAL_AM_PORT GPIOA
#define HAL_AM_CHAN0_PIN GPIO_PIN_3
#define HAL_AM_CHAN1_PIN GPIO_PIN_11
#define HAL_AM_CHAN2_PIN GPIO_PIN_12
#define HAL_AM_CHAN3_PIN GPIO_PIN_13
#define HAL_AM_CHAN4_PIN GPIO_PIN_14
#define HAL_AM_CHAN5_PIN GPIO_PIN_15

/**
 * Maximal value for 3.5MHz antenna matching
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * !! UPDATE YHL_MATCHING_LEVELS_COUNT AFTER CHANGING THIS !!
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 */
#define HAL_AM_MAX_VALUE 63

/**
 * Pause in milliseconds for transient processes during antenna matching.
 * Logic: carrier is suppressed -> pause -> matching switch -> pause -> carrier restored
 */
#define HAL_AM_PAUSE 100

/**
 * Left button
 */
#define HAL_LEFT_BUTTON_PORT GPIOA
#define HAL_LEFT_BUTTON_PIN GPIO_PIN_4
#define HAL_LEFT_BUTTON_EXTI_LINE EXTI4_IRQn

/**
 * Right button
 */
#define HAL_RIGHT_BUTTON_PORT GPIOA
#define HAL_RIGHT_BUTTON_PIN GPIO_PIN_6
#define HAL_RIGHT_BUTTON_EXTI_LINE EXTI9_5_IRQn

/**
 * Encoder button
 */
#define HAL_ENCODER_BUTTON_PORT GPIOB
#define HAL_ENCODER_BUTTON_PIN GPIO_PIN_14
#define HAL_ENCODER_BUTTON_EXTI_LINE EXTI15_10_IRQn

/**
 * Encoder right
 */
#define HAL_ENCODER_RIGHT_PORT GPIOB
#define HAL_ENCODER_RIGHT_PIN GPIO_PIN_13

/**
 * Encoder left
 */
#define HAL_ENCODER_LEFT_PORT GPIOB
#define HAL_ENCODER_LEFT_PIN GPIO_PIN_12


/**
 * Encoder
 */
#define HAL_ENCODER_ROTATION_NONE 0
#define HAL_ENCODER_ROTATION_CLOCKWISE 1
#define HAL_ENCODER_ROTATION_COUNTERCLOCKWISE -1

/**
 * This number of hardware clicks will lead to one software click
 */
#define HAL_ENCODER_DIVISION 2

/**
 * How much time to sample encoder data in ISR to suppress transient effects
 */
#define HAL_ENCODER_SAMPLES_COUNT 128

/**
 * AHB divider for economy mode
 */
#define HAL_AHB_DIVIDER_FOR_ECONOMY_MODE RCC_SYSCLK_DIV16

/**
 * Bluetooth uses this USART
 */
#define HAL_BLUETOOTH_UART USART1

/**
 * Bluetooth UART baudrate
 */
#define HAL_BLUETOOTH_UART_BAUDRATE 115200


extern ADC_HandleTypeDef ADC_Handle;
extern L2HAL_AD5245_ContextStruct U80mRegulatorContext;
extern I2C_HandleTypeDef I2C_Other;
extern L2HAL_AD9835_ContextStruct SynthesizerContext;
extern SPI_HandleTypeDef SPIHandle;
extern TIM_HandleTypeDef ToneTimerHandle;

/**
 * ADC channel in use
 */
typedef enum
{
	/* 3.5MHz antenna */
	YHL_HAL_UAnt = 0,

	/* Battery */
	YHL_HAL_UBatt = 1,

	/* 3.5MHz output stage voltage */
	YHL_HAL_U80m = 2
}
YHL_HAL_ADCChannelEnum;

/**
 * 3.5MHz antenna matching channels
 */
typedef enum
{
	YHL_HAL_AM0,
	YHL_HAL_AM1,
	YHL_HAL_AM2,
	YHL_HAL_AM3,
	YHL_HAL_AM4,
	YHL_HAL_AM5
}
YHL_HAL_AMChannelEnum;


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
 * Returns if battery level check circuit enabled
 */
bool HAL_GetUBattCheck(void);

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

/**
 * Enables timer, which generate 144MHz modulation tone
 */
void HAL_Enable2mToneGenerator(void);

/**
 * Disables tone generation timer (see HAL_Enable2mTonGenerator()) and restores normal state
 * of manipulator pin.
 */
void HAL_Disable2mToneGenerator(void);

/**
 * Switches given 3.5 MHz antenna matching channel or on off
 */
void HAL_SwitchAntennaMatchingChannel(YHL_HAL_AMChannelEnum channel, bool isOn);

/**
 * Sets 3.5MHz antenna matching to given value
 */
void HAL_SetAntennaMatchingValue(uint8_t value);

/**
 * Switches 3.5MHz antenna matching on or off (when enabled SWDIO and SWCLK work as GPIO)
 */
void HAL_SwitchAntennaMatching(bool isOn);

/**
 * Registers left button press handler
 */
void HAL_RegisterLeftButtonHandler(void (*handler)(void));

/**
 * Registers right button press handler
 */
void HAL_RegisterRightButtonHandler(void (*handler)(void));

/**
 * Registers encoder button press handler
 */
void HAL_RegisterEncoderButtonHandler(void (*handler)(void));

/**
 * Registers encoder rotation handler
 */
void HAL_RegisterEncoderRotationHandler(void (*handler)(int8_t));

/**
 * Call it on every SysTick
 */
void HAL_OnTick(void);

/**
 * Shutdown everything except MCU in case of failure.
 */
void HAL_EmergencyShutdown(void);

/**
 * De-initialize display bus
 */
void HAL_DeInitializeDisplayBus(void);

/**
 * True, if display bus is initialized
 */
bool HAL_GetDisplayBusInitializationStatus(void);

/**
 * Disconnects I2C bus from display pins and pulls those pins to the ground
 */
void HAL_SupressDisplayParasiticPower(void);

/**
 * Initialize display bus
 */
void HAL_InitializeDisplayBus(void);

/**
 * Downclocks MCU (core and buses)
 */
void HAL_EnterEconomyMode(void);

/**
 * Restores normal functioning after economy mode
 */
void HAL_ExitEconomyMode(void);

/**
 * Disables ADC and it's clock
 */
void HAL_DisableADC(void);

/**
 * Enables UART
 */
void HAL_EnableUART(void);

/**
 * Disables UART
 */
void HAL_DisableUART(void);

#endif /* INCLUDE_HAL_H_ */
