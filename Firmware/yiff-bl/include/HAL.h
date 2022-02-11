/*
 * HAL.h
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_HAL_H_
#define INCLUDE_HAL_H_

#include <stm32f1xx_hal.h>
#include <l2hal.h>
#include <l2hal_errors.h>
#include <stdbool.h>

extern UART_HandleTypeDef UART_Handle;
extern I2C_HandleTypeDef I2C_Display;

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
 * FSYNC pin for frequency synthesizer
 */
#define HAL_SYNTHESIZER_FSYNC_PORT GPIOA
#define HAL_SYNTHESIZER_FSYNC_PIN GPIO_PIN_8

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
 * Left button
 */
#define HAL_LEFT_BUTTON_PORT GPIOA
#define HAL_LEFT_BUTTON_PIN GPIO_PIN_4

/**
 * Right button
 */
#define HAL_RIGHT_BUTTON_PORT GPIOA
#define HAL_RIGHT_BUTTON_PIN GPIO_PIN_6

/**
 * Encoder button
 */
#define HAL_ENCODER_BUTTON_PORT GPIOB
#define HAL_ENCODER_BUTTON_PIN GPIO_PIN_14

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
 * Bluetooth uses this USART
 */
#define HAL_BLUETOOTH_UART USART1

/**
 * Bluetooth UART baudrate
 */
#define HAL_BLUETOOTH_UART_BAUDRATE 115200


/**
 * Init hardware to a safe state (i.e. disable transmitters, enable display and bluetooth and so on)
 */
void HAL_InitHardware(void);

/**
 * Enables UART
 */
void HAL_EnableUART(void);

/**
 * True, if display bus is initialized
 */
bool HAL_GetDisplayBusInitializationStatus(void);

/**
 * De-initialize display bus
 */
void HAL_DeInitializeDisplayBus(void);

/**
 * Disconnects I2C bus from display pins and pulls those pins to the ground
 */
void HAL_SupressDisplayParasiticPower(void);

/**
 * Turns display power on and off
 */
void HAL_SwitchDisplayPower(bool isOn);

/**
 * Initialize display bus
 */
void HAL_InitializeDisplayBus(void);

#endif /* INCLUDE_HAL_H_ */
