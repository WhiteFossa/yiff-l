/*
 * StatusDisplay.c
 *
 *  Created on: Apr 7, 2021
 *      Author: fossa
 */

#include <StatusDisplay.h>

void DrawStatusDisplay(FoxStateStruct foxState)
{
	/* Clear display */
//	FMGL_API_ClearScreen(&fmglContext);

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	uint16_t rightmostPixel = FMGL_API_GetDisplayWidth(&fmglContext) - 1;

	/* 1st line */
	DrawBattery(foxState.BatteryLevel);
	DrawCurrentTime(foxState.CurrentTime);
	DrawTXStatus(foxState.IsTXOn);

	/* 2nd line */
	DrawFoxName(foxState.Name);

	/* 3rd line */
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_3RD_LINE_CLEAR_TOP, rightmostPixel, YHL_3RD_LINE_CLEAR_BOTTOM, OffColor, OffColor);
	uint16_t frequencyTextWidth = DrawFoxFrequency(foxState.Frequency);
	DrawFoxCode(foxState.Code, FMGL_API_GetDisplayWidth(&fmglContext) - frequencyTextWidth);

	/* 4rd line */
	DrawFoxCycle(foxState.Cycle);

	/* 5th line */
	DrawEndingTone(foxState.EndingToneLength, foxState.Cycle);

	/* 6th line */
	DrawFoxCycleState(foxState.CycleState, foxState.CurrentTime, foxState.Cycle, foxState.GlobalState);

	/* 7th line */
	DrawGlobalState(foxState.GlobalState, foxState.CurrentTime);

	/* 8th line */
	DrawFoxPower(foxState.Power, foxState.Frequency);

	/* Buttons */
	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

void DrawBattery(float level)
{
	FMGL_API_DrawRectangleFilled(&fmglContext,
			YHL_BATTERY_LEFT,
			YHL_BATTERY_TOP,
			YHL_BATTERY_LEFT + YHL_BATTERY_CONTACT_WIDTH + YHL_BATTERY_MAIN_PART_WIDTH,
			YHL_BATTERY_TOP + YHL_BATTERY_MAIN_PART_HEIGHT,
			OffColor,
			OffColor);

	/* Outer rectangle*/
	FMGL_API_DrawRectangle(&fmglContext,
			YHL_BATTERY_LEFT + YHL_BATTERY_CONTACT_WIDTH,
			YHL_BATTERY_TOP,
			YHL_BATTERY_LEFT + YHL_BATTERY_CONTACT_WIDTH + YHL_BATTERY_MAIN_PART_WIDTH,
			YHL_BATTERY_TOP + YHL_BATTERY_MAIN_PART_HEIGHT);

	/* Contact */
	FMGL_API_DrawRectangleFilled(&fmglContext,
			YHL_BATTERY_LEFT,
			YHL_BATTERY_TOP + YHL_BATTERY_CONTACT_SHIFT_Y,
			YHL_BATTERY_LEFT + YHL_BATTERY_CONTACT_WIDTH,
			YHL_BATTERY_TOP + YHL_BATTERY_CONTACT_SHIFT_Y + YHL_BATTERY_CONTACT_HEIGHT,
			OnColor,
			OnColor);

	/* Inner part */
	float innerPartWidthF = (YHL_BATTERY_MAIN_PART_WIDTH - 2 * YHL_BATTERY_INNER_PART_SPACING) * level;
	if (innerPartWidthF > 0)
	{
		FMGL_API_DrawRectangleFilled(&fmglContext,
			YHL_BATTERY_LEFT + YHL_BATTERY_CONTACT_WIDTH + YHL_BATTERY_MAIN_PART_WIDTH - YHL_BATTERY_INNER_PART_SPACING - floor(innerPartWidthF + 0.5),
			YHL_BATTERY_TOP + YHL_BATTERY_INNER_PART_SPACING,
			YHL_BATTERY_LEFT + YHL_BATTERY_CONTACT_WIDTH + YHL_BATTERY_MAIN_PART_WIDTH - YHL_BATTERY_INNER_PART_SPACING,
			YHL_BATTERY_TOP + YHL_BATTERY_MAIN_PART_HEIGHT - YHL_BATTERY_INNER_PART_SPACING,
			OnColor,
			OnColor);
	}
}

void DrawCurrentTime(Time time)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, YHL_TIME_LEFT, YHL_TIME_TOP, YHL_TIME_CLEAR_RIGHT, YHL_TIME_CLEAR_BOTTOM, OffColor, OffColor);
	char buffer[16];
	TimeToHMS(time, buffer);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_TIME_LEFT, YHL_TIME_TOP, NULL, NULL, false, buffer);
}

void DrawTXStatus(bool isTXOn)
{
	FMGL_API_XBMImage TXIcon;
	TXIcon.Width = YHL_TX_STATUS_ICON_SIZE;
	TXIcon.Height = YHL_TX_STATUS_ICON_SIZE;

	if (isTXOn)
	{
		TXIcon.Raster = tx_on_bits;
	}
	else
	{
		TXIcon.Raster = tx_off_bits;
	}

	FMGL_API_RenderXBM(&fmglContext, &TXIcon, YHL_TX_STATUS_LEFT, YHL_TX_STATUS_TOP, 1, 1, OnColor, OffColor, FMGL_XBMTransparencyModeNormal);
}

void DrawFoxName(char* name)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_FOX_NAME_CLEAR_TOP, FMGL_API_GetDisplayWidth(&fmglContext) - 1, YHL_FOX_NAME_CLEAR_BOTTOM, OffColor, OffColor);

	uint16_t width;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &width, NULL, true, name);

	int16_t spacing = (FMGL_API_GetDisplayWidth(&fmglContext) - width) / 2;
	if (spacing < 0)
	{
		spacing = 0;
	}

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, spacing, YHL_FOX_NAME_TOP, NULL, NULL, false, name);
}

uint16_t DrawFoxFrequency(FoxFrequencyStruct frequency)
{
	float freqMHz = frequency.FrequencyHz / 1000000.0f;

	char buffer[16];
	sprintf(buffer, "%.3f", freqMHz);

	uint16_t width;

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &frequencyFont, YHL_FREQUENCY_LEFT, YHL_FREQUENCY_TOP, &width, NULL, false, buffer);

	return width;
}


void DrawFoxCode(FoxCodeEnum code, uint16_t availableWidth)
{
	char number[16];
	char codeTxt[16];

	switch(code)
	{
		case Finish:
			sprintf(number, "Finish");
			sprintf(codeTxt, "MO");
		break;
		case F1:
			sprintf(number, "#1");
			sprintf(codeTxt, "MOE");
		break;
		case F2:
			sprintf(number, "#2");
			sprintf(codeTxt, "MOI");
		break;
		case F3:
			sprintf(number, "#3");
			sprintf(codeTxt, "MOS");
		break;
		case F4:
			sprintf(number, "#4");
			sprintf(codeTxt, "MOH");
		break;
		case F5:
			sprintf(number, "#5");
			sprintf(codeTxt, "MO5");
		break;
		case Beacon:
			sprintf(number, "Beacon");
			sprintf(codeTxt, "S");
		break;
		default:
			L2HAL_Error(Generic);
		break;
	}

	/* Dry runs*/
	uint16_t numberWidth;
	uint16_t codeWidth;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &numberWidth, NULL, true, number);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &codeWidth, NULL, true, codeTxt);

	uint16_t numberSpacing = (availableWidth - numberWidth) / 2;
	uint16_t codeSpacing = (availableWidth - codeWidth) / 2;

	uint16_t lastPixel = FMGL_API_GetDisplayWidth(&fmglContext) - 1;

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, lastPixel - (numberWidth + numberSpacing), YHL_CODE_LINE1_TOP, NULL, NULL, false, number);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, lastPixel - (codeWidth + codeSpacing), YHL_CODE_LINE2_TOP, NULL, NULL, false, codeTxt);
}

void DrawFoxCycle(FoxCycleStruct cycle)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_CYCLE_CLEAR_TOP, FMGL_API_GetDisplayWidth(&fmglContext) - 1, YHL_CYCLE_CLEAR_BOTTOM, OffColor, OffColor);

	char buffer[32];
	char cycleTimeTxt[16];

	if (cycle.IsContinuous)
	{
		sprintf(cycleTimeTxt, "continuous");
	}
	else
	{
		char TxTimeTxt[8];
		char PauseTimeTxt[8];
		TimeToMS(cycle.TxTime, TxTimeTxt);
		TimeToMS(cycle.PauseTime, PauseTimeTxt);

		sprintf(cycleTimeTxt, "%s/%s", TxTimeTxt, PauseTimeTxt);
	}

	sprintf(buffer, "Cycle: %s", cycleTimeTxt);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_CYCLE_LEFT, YHL_CYCLE_TOP, NULL, NULL, false, buffer);
}

void DrawEndingTone(uint8_t endingToneLength, FoxCycleStruct cycle)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_ENDING_TONE_CLEAR_TOP, FMGL_API_GetDisplayWidth(&fmglContext) - 1, YHL_ENDING_TONE_CLEAR_BOTTOM, OffColor, OffColor);
	char buffer[32];

	if (cycle.IsContinuous)
	{
		sprintf(buffer, "Ending tone: N/A");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_ENDING_TONE_LEFT, YHL_ENDING_TONE_TOP, NULL, NULL, false, buffer);
		return;
	}

	sprintf(buffer, "Ending tone: %ds", endingToneLength);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_ENDING_TONE_LEFT, YHL_ENDING_TONE_TOP, NULL, NULL, false, buffer);
}

void DrawFoxCycleState(CycleStateStruct cycleState, Time currentTime, FoxCycleStruct cycle, GlobalFoxStateStruct globalState)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_CYCLE_STATE_CLEAR_TOP, FMGL_API_GetDisplayWidth(&fmglContext) - 1, YHL_CYCLE_STATE_CLEAR_BOTTOM, OffColor, OffColor);

	if (BeforeFinish != globalState.CurrentState)
	{
		char buffer[32];
		sprintf(buffer, "Waiting for start");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_CYCLE_STATE_LEFT, YHL_CYCLE_STATE_TOP, NULL, NULL, false, buffer);
		return;
	}

	if (true == cycle.IsContinuous)
	{
		char buffer[32];
		sprintf(buffer, "N/A: Continuous cycle");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_CYCLE_STATE_LEFT, YHL_CYCLE_STATE_TOP, NULL, NULL, false, buffer);
		return;
	}

	uint32_t secondsTillAction = SecondsSinceDayBegin(cycleState.StateChangeTime) - SecondsSinceDayBegin(currentTime);
	Time timeTillAction = TimeSinceDayBegin(secondsTillAction);

	char timeBuffer[16];
	TimeToHMS(timeTillAction, timeBuffer);

	char actionBuffer[16];
	switch(cycleState.CycleState)
	{
		case Tx:
			sprintf(actionBuffer, "pause");
		break;
		case Pause:
			sprintf(actionBuffer, "TX");
		break;
		default:
			L2HAL_Error(Generic);
		break;
	}

	char buffer[32];
	sprintf(buffer, "%s till %s", timeBuffer, actionBuffer);

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_CYCLE_STATE_LEFT, YHL_CYCLE_STATE_TOP, NULL, NULL, false, buffer);
}

void DrawGlobalState(GlobalFoxStateStruct globalState, Time currentTime)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_GLOBAL_STATE_CLEAR_TOP, FMGL_API_GetDisplayWidth(&fmglContext) - 1, YHL_GLOBAL_STATE_CLEAR_BOTTOM, OffColor, OffColor);

	if (Standby == globalState.CurrentState)
	{
		char buffer[32];
		sprintf(buffer, "Standing by");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_GLOBAL_STATE_LEFT, YHL_GLOBAL_STATE_TOP, NULL, NULL, false, buffer);
		return;
	}

	uint32_t secondsTillAction = SecondsSinceDayBegin(globalState.StateChangeTime) - SecondsSinceDayBegin(currentTime);
	Time timeTillAction = TimeSinceDayBegin(secondsTillAction);

	char timeBuffer[16];
	TimeToHMS(timeTillAction, timeBuffer);

	char actionBuffer[16];
	switch(globalState.CurrentState)
	{
		case BeforeStart:
			sprintf(actionBuffer, "start");
		break;
		case BeforeFinish:
			sprintf(actionBuffer, "finish");
		break;
		default:
			L2HAL_Error(Generic);
		break;
	}

	char buffer[32];
	sprintf(buffer, "%s till %s", timeBuffer, actionBuffer);

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_GLOBAL_STATE_LEFT, YHL_GLOBAL_STATE_TOP, NULL, NULL, false, buffer);
}

void DrawFoxPower(float power, FoxFrequencyStruct frequency)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_POWER_CLEAR_TOP, FMGL_API_GetDisplayWidth(&fmglContext) - 1, YHL_POWER_CLEAR_BOTTOM, OffColor, OffColor);

	if (frequency.Is144MHz)
	{
		char buffer[32];
		sprintf(buffer, "Power: N/A");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_POWER_LEFT, YHL_POWER_TOP, NULL, NULL, false, buffer);
		return;
	}

	char buffer[32];
	sprintf(buffer, "Power: %.1fW", power);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_POWER_LEFT, YHL_POWER_TOP, NULL, NULL, false, buffer);
}
