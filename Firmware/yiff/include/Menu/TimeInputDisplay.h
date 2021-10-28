/*
 * TimeInputDisplay.h
 *
 *  Created on: Oct 24, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENU_TIMEINPUTDISPLAY_H_
#define INCLUDE_MENU_TIMEINPUTDISPLAY_H_

#include <printf.h>
#include <stdlib.h>
#include <GraphicsBase.h>
#include <Buttons.h>
#include <FoxStatus.h>
#include <HAL.h>

/**
 * Call it to show time input display (times are in seconds)
 */
void TimeInputDisplay_Show(char* title,
		uint32_t minTime,
		uint32_t maxTime,
		uint32_t initialTime,
		void (*onCloseHandler)(uint32_t enteredTime),
		FoxDisplayEnum previousDisplay);


/**
 * Call it on left button click
 */
void TimeInputDisplay_LeftClickHandler(void);

/**
 * Call it on encoder click
 */
void TimeInputDisplay_EncoderClickHandler(void);

/**
 * Call it on right button click
 */
void TimeInputDisplay_RightClickHandler(void);

/**
 * Call it on encoder rotation
 */
void TimeInputDisplay_EncoderRotationHandler(int8_t direction);


#endif /* INCLUDE_MENU_TIMEINPUTDISPLAY_H_ */
