/*
 * NumberInputDisplayPrivate.h
 *
 *  Created on: Sep 19, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENU_NUMBERINPUTDISPLAYPRIVATE_H_
#define INCLUDE_MENU_NUMBERINPUTDISPLAYPRIVATE_H_

#include <stdint.h>

/**
 * Pointer to function, which will be used to format value to display.
 */
char* (*NumberInputDisplay_Formatter)(int32_t valueToFormat);

/**
 * Pointer to function, which will be called when display is closed
 */
void (*NumberInputDisplay_OnCloseHandler)(int32_t enteredValue);

/**
 * Minimal value
 */
int32_t NumberInputDisplay_MinValue;

/**
 * Maximal value
 */
int32_t NumberInputDisplay_MaxValue;

/**
 * Initial value
 */
int32_t NumberInputDisplay_InitialValue;

/**
 * Current value
 */
int32_t NumberInputDisplay_CurrentValue;

/**
 * Decrement/increment step
 */
int32_t NumberInputDisplay_Step;

/**
* Return to this display when number is entered
*/
FoxDisplayEnum NumberInputDisplay_PreviousDisplay;

/**
 * Display title
 */
char* NumberInputDisplay_Title;

/**
 * Draw current state.
 */
void NumberInputDisplay_Display(void);

/**
 * Close current display.
 */
void NumberInputDisplay_Close(void);

#endif /* INCLUDE_MENU_NUMBERINPUTDISPLAYPRIVATE_H_ */
