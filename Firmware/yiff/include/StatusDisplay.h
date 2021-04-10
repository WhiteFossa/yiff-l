/*
 * StatusDisplay.h
 *
 *  Created on: Apr 7, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_STATUSDISPLAY_H_
#define INCLUDE_STATUSDISPLAY_H_

#include <GraphicsBase.h>
#include <FoxStatus.h>
#include <Resources/tx_on.h>
#include <Resources/tx_off.h>

/**
 * Battery position and sizes
 */
#define YHL_BATTERY_LEFT 0
#define YHL_BATTERY_TOP 4
#define YHL_BATTERY_MAIN_PART_WIDTH 60
#define YHL_BATTERY_CONTACT_WIDTH 1
#define YHL_BATTERY_MAIN_PART_HEIGHT 5
#define YHL_BATTERY_CONTACT_HEIGHT 3
#define YHL_BATTERY_CONTACT_SHIFT_Y 1
#define YHL_BATTERY_INNER_PART_SPACING 2

/**
 * Time position and sizes
 */
#define YHL_TIME_LEFT 66
#define YHL_TIME_TOP 1

/**
 * TX status indicator.
 */
#define YHL_TX_STATUS_LEFT 116
#define YHL_TX_STATUS_TOP 0
#define YHL_TX_STATUS_ICON_SIZE 12

/**
 * Fox name.
 */
#define YHL_FOX_NAME_TOP 13

/**
 * Frequency
 */
#define YHL_FREQUENCY_LEFT 0
#define YHL_FREQUENCY_TOP 24

/**
 * Code
 */
#define YHL_CODE_LINE1_TOP 24
#define YHL_CODE_LINE2_TOP 34

/**
 * Draw complete status display.
 */
void DrawStatusDisplay(FoxStateStruct foxState);

/**
 * Draw battery level.
 */
void DrawBattery(float level);

/**
 * Draw current time.
 */
void DrawCurrentTime(Time time);

/**
 * Draw transmitter status.
 */
void DrawTXStatus(bool isTXOn);

/**
 * Draw fox name.
 */
void DrawFoxName(char* name);

/**
 * Draw fox frequency.
 * Returns width of frequency text in pixels
 */
uint16_t DrawFoxFrequency(FoxFrequencyStruct frequency);

/**
 * Draw fox code (two lines).
 * availableWidth - width, available for code text.
 */
void DrawFoxCode(FoxCodeEnum code, uint16_t availableWidth);

#endif /* INCLUDE_STATUSDISPLAY_H_ */
