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
#define YHL_TIME_CLEAR_RIGHT 115
#define YHL_TIME_CLEAR_BOTTOM 11
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
#define YHL_FOX_NAME_CLEAR_TOP 12
#define YHL_FOX_NAME_CLEAR_BOTTOM 25
#define YHL_FOX_NAME_TOP 13

/**
 * 3rd line clear params
 */
#define YHL_3RD_LINE_CLEAR_TOP 25
#define YHL_3RD_LINE_CLEAR_BOTTOM 44

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
 * Cycle
 */
#define YHL_CYCLE_CLEAR_TOP 45
#define YHL_CYCLE_CLEAR_BOTTOM 58
#define YHL_CYCLE_LEFT 0
#define YHL_CYCLE_TOP 46

/**
 * Ending tone
 */
#define YHL_ENDING_TONE_CLEAR_TOP 59
#define YHL_ENDING_TONE_CLEAR_BOTTOM 70
#define YHL_ENDING_TONE_LEFT 0
#define YHL_ENDING_TONE_TOP 58

/**
 * Cycle state
 */
#define YHL_CYCLE_STATE_CLEAR_TOP 71
#define YHL_CYCLE_STATE_CLEAR_BOTTOM 82
#define YHL_CYCLE_STATE_LEFT 0
#define YHL_CYCLE_STATE_TOP 70

/**
 * Global state
 */
#define YHL_GLOBAL_STATE_CLEAR_TOP 83
#define YHL_GLOBAL_STATE_CLEAR_BOTTOM 94
#define YHL_GLOBAL_STATE_LEFT 0
#define YHL_GLOBAL_STATE_TOP 82

/**
 * Fox power
 */
#define YHL_POWER_CLEAR_TOP 95
#define YHL_POWER_CLEAR_BOTTOM 107
#define YHL_POWER_LEFT 0
#define YHL_POWER_TOP 94

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
void DrawFoxCode(FoxCodeEnum code, bool isFast, uint16_t availableWidth);

/**
 * Draw fox cycle.
 */
void DrawFoxCycle(FoxCycleStruct cycle);

/**
 * Draw ending tone length.
 */
void DrawEndingTone(uint8_t endingToneLength, FoxCycleStruct cycle);

/**
 * Draw fox cycle state.
 */
void DrawFoxCycleState(CycleStateStruct cycleState, Time currentTime, FoxCycleStruct cycle, GlobalFoxStateStruct globalState);

/**
 * Display global fox state.
 */
void DrawGlobalState(GlobalFoxStateStruct globalState, Time currentTime);

/**
 * Draw fox power.
 */
void DrawFoxPower(float power, FoxFrequencyStruct frequency);

#endif /* INCLUDE_STATUSDISPLAY_H_ */
