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
 * Enable/disable battery level check. Note, that when it enabled, it leads to some current, pulled from battery.
 * So do not turn it on for long when device in sleep mode.
 */
void HAL_SwitchUBattCheck(bool isOn);

#endif /* INCLUDE_HAL_H_ */
