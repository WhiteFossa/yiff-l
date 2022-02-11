/*
 * HardwareLogic.h
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_HARDWARELOGIC_H_
#define INCLUDE_HARDWARELOGIC_H_

#include <l2hal_errors.h>
#include <stdbool.h>
#include <l2hal_ssd1327.h>

extern L2HAL_SSD1327_ContextStruct L2HAL_SSD1327_Context;
extern I2C_HandleTypeDef I2C_Display;

/**
 * Try to initialize display this times before reporting a failure
 */
#define YBL_HL_INITIALIZE_DISPLAY_ATTEMPS_COUNT 5

/**
 * Time to wait after powering display down before attempt to turn it on
 */
#define YBL_HL_DISPLAY_POWERDOWN_TIME 1000

/**
 * Time to spin display power regulator up
 */
#define YBL_HL_DISPLAY_REGULATOR_SPINUP_TIME 1000

/**
 * Turns display on and initializes it
 */
void HL_TurnDisplayOn(void);



#endif /* INCLUDE_HARDWARELOGIC_H_ */
