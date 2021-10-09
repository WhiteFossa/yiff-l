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
 * Pointer to function, which will be called when display is closed
 */
void (*NumberInputDisplay_OnCloseHandler)(float enteredValue);

/**
 * Minimal value
 */
float NumberInputDisplay_MinValue;

/**
 * Maximal value
 */
float NumberInputDisplay_MaxValue;

/**
 * Initial value
 */
float NumberInputDisplay_InitialValue;

/**
 * Current value
 */
float NumberInputDisplay_CurrentValue;

/**
 * Decrement/increment step
 */
float NumberInputDisplay_Step;

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
