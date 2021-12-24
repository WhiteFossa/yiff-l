/*
 * NumberInputDisplay.h
 *
 *  Created on: Sep 19, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENU_NUMBERINPUTDISPLAY_H_
#define INCLUDE_MENU_NUMBERINPUTDISPLAY_H_

#include <GraphicsBase.h>
#include <Buttons.h>
#include <HAL.h>
#include <FoxStatus.h>
#include <printf.h>
#include <stdlib.h>
#include <SelfDiagnostics.h>

extern FoxStateStruct FoxState;

/**
 * Position of title top
 */
#define YHL_NUMBER_INPUT_DISPLAY_TITLE_TOP 0

#define YHL_NUMBER_INPUT_DISPLAY_FORMATTER_BUFFER_SIZE 32


/**
 * Call it to show number input display
 */
void NumberInputDisplay_Show(char* title,
		int32_t minValue,
		int32_t maxValue,
		int32_t initialValue,
		int32_t step,
		void (*formatter)(int32_t valueToFormat, char buffer[YHL_NUMBER_INPUT_DISPLAY_FORMATTER_BUFFER_SIZE]),
		void (*onCloseHandler)(int32_t enteredValue),
		FoxDisplayEnum previousDisplay);

/**
 * Call it on left button click
 */
void NumberInputDisplay_LeftClickHandler(void);

/**
 * Call it on encoder click
 */
void NumberInputDisplay_EncoderClickHandler(void);

/**
 * Call it on right button click
 */
void NumberInputDisplay_RightClickHandler(void);

/**
 * Call it on encoder rotation
 */
void NumberInputDisplay_EncoderRotationHandler(int8_t direction);


#endif /* INCLUDE_MENU_NUMBERINPUTDISPLAY_H_ */
