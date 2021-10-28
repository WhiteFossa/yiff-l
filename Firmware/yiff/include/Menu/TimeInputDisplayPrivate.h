/*
 * TimeInputDisplayPrivate.h
 *
 *  Created on: Oct 24, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENU_TIMEINPUTDISPLAYPRIVATE_H_
#define INCLUDE_MENU_TIMEINPUTDISPLAYPRIVATE_H_

/**
 * Title position
 */
#define YHL_TIME_INPUT_DISPLAY_TITLE_TOP 0U

/**
 * What value is active to enter
 */
typedef enum
{
	/**
	 * Entering hours
	 */
	TimeInputDisplay_Hours,

	/**
	 * Entering minutes
	 */
	TimeInputDisplay_Minutes,

	/**
	 * Entering seconds
	 */
	TimeInputDisplay_Seconds
}
TimeInputDisplay_ActiveValueEnum;

/**
 * Pointer to function, which will be called when display is closed
 */
void (*TimeInputDisplay_OnCloseHandler)(uint32_t enteredTime);

/**
 * Minimal time
 */
uint32_t TimeInputDisplay_MinTime;

/**
 * Maximal time
 */
uint32_t TimeInputDisplay_MaxTime;

/**
 * Initial time
 */
uint32_t TimeInputDisplay_InitialTime;

/**
 * Current time
 */
uint32_t TimeInputDisplay_CurrentTime;

/**
 * Seconds
 */
uint8_t TimeInputDisplay_SecondsValue;

/**
 * Minutes
 */
uint8_t TimeInputDisplay_MinutesValue;

/**
 * Hours
 */
uint8_t TimeInputDisplay_HoursValue;

/**
* Return to this display when time is entered
*/
FoxDisplayEnum TimeInputDisplay_PreviousDisplay;

/**
 * What value are we entering?
 */
TimeInputDisplay_ActiveValueEnum TimeInputDisplay_ActiveValue;

/**
 * Display title
 */
char* TimeInputDisplay_Title;


/**
 * Draw current state.
 */
void TimeInputDisplay_Display(void);

/**
 * Call this on display closing
 */
void TimeInputDisplay_Close(void);

/**
 * Constructs time formatting string based on currently active value.
 *
 * Do not forget to free the result!
 */
char* TimeInputDisplay_CostructTimeFormatString(TimeInputDisplay_ActiveValueEnum activeValue);

#endif /* INCLUDE_MENU_TIMEINPUTDISPLAYPRIVATE_H_ */
