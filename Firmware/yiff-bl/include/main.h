/*
 * main.h
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_MAIN_H_
#define INCLUDE_MAIN_H_

#include <stdbool.h>
#include <stm32f1xx_hal.h>
#include <Bootloader.h>
#include <l2hal.h>
#include <GlobalVariables.h>
#include <HAL.h>
#include <Version.h>
#include <HardwareLogic.h>

/**
 * Initialize display and fonts
 */
void Main_InitDisplayAndFonts(void);


#endif /* INCLUDE_MAIN_H_ */
