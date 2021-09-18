/*
 * StatusDisplay.c
 *
 *  Created on: Apr 7, 2021
 *      Author: fossa
 */

#include <main.h>

void DrawStatusDisplay(FoxStateStruct foxState)
{
	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	uint16_t rightmostPixel = (uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - 1U);

	/* 1st line */
	DrawBattery(foxState.BatteryLevel);
	DrawCurrentTime(foxState.CurrentTime);
	DrawTXStatus(foxState.IsTXOn);

	/* 2nd line */
	DrawFoxName(foxState.Name);

	/* 3rd line */
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_3RD_LINE_CLEAR_TOP, rightmostPixel, YHL_3RD_LINE_CLEAR_BOTTOM, OffColor, OffColor);
	uint16_t frequencyTextWidth = DrawFoxFrequency(foxState.Frequency);
	DrawFoxCode(foxState.Code, foxState.IsFast, (uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - frequencyTextWidth));

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
	float innerPartWidthF = (YHL_BATTERY_MAIN_PART_WIDTH - 2U * YHL_BATTERY_INNER_PART_SPACING) * level;
	if (innerPartWidthF > 0)
	{
		FMGL_API_DrawRectangleFilled(&fmglContext,
			(uint16_t)(YHL_BATTERY_LEFT + YHL_BATTERY_CONTACT_WIDTH + YHL_BATTERY_MAIN_PART_WIDTH - YHL_BATTERY_INNER_PART_SPACING - (uint16_t)floor(innerPartWidthF + 0.5f)),
			YHL_BATTERY_TOP + YHL_BATTERY_INNER_PART_SPACING,
			YHL_BATTERY_LEFT + YHL_BATTERY_CONTACT_WIDTH + YHL_BATTERY_MAIN_PART_WIDTH - YHL_BATTERY_INNER_PART_SPACING,
			YHL_BATTERY_TOP + YHL_BATTERY_MAIN_PART_HEIGHT - YHL_BATTERY_INNER_PART_SPACING,
			OnColor,
			OnColor);
	}
}

void DrawCurrentTime(uint32_t time)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, YHL_TIME_LEFT, YHL_TIME_TOP, YHL_TIME_CLEAR_RIGHT, YHL_TIME_CLEAR_BOTTOM, OffColor, OffColor);

	uint8_t bufferSize = 16;
	char buffer[bufferSize];
	TimestampToHMSString(time, buffer, bufferSize);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_TIME_LEFT, YHL_TIME_TOP, NULL, NULL, false, buffer);
}

void DrawTXStatus(bool isTXOn)
{
	FMGL_API_XBMImage TXIcon;
	TXIcon.Width = YHL_TX_STATUS_ICON_SIZE;
	TXIcon.Height = YHL_TX_STATUS_ICON_SIZE;

	if (isTXOn)
	{
		TXIcon.Raster = (uint8_t*)tx_on_bits;
	}
	else
	{
		TXIcon.Raster = (uint8_t*)tx_off_bits;
	}

	FMGL_API_RenderXBM(&fmglContext, &TXIcon, YHL_TX_STATUS_LEFT, YHL_TX_STATUS_TOP, 1, 1, OnColor, OffColor, FMGL_XBMTransparencyModeNormal);
}

void DrawFoxName(char* name)
{
	FMGL_API_DrawRectangleFilled(&fmglContext,
			0,
			YHL_FOX_NAME_CLEAR_TOP,
			(uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - 1U),
			YHL_FOX_NAME_CLEAR_BOTTOM, OffColor, OffColor);

	uint16_t width;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &width, NULL, true, name);

	int16_t spacing = (int16_t)((FMGL_API_GetDisplayWidth(&fmglContext) - width) / 2U);
	if (spacing < 0)
	{
		spacing = 0;
	}

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)spacing, YHL_FOX_NAME_TOP, NULL, NULL, false, name);
}

uint16_t DrawFoxFrequency(FoxFrequencyStruct frequency)
{
	float freqMHz = (float)frequency.FrequencyHz / 1000000.0f;

	char buffer[16];
	snprintf(buffer, 16, "%.3f", freqMHz);

	uint16_t width;

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &frequencyFont, YHL_FREQUENCY_LEFT, YHL_FREQUENCY_TOP, &width, NULL, false, buffer);

	return width;
}


void DrawFoxCode(FoxCodeEnum code, bool isFast, uint16_t availableWidth)
{
	uint8_t numberBufferSize = 16;
	char number[numberBufferSize];

	uint8_t codeTxtBufferSize = 8;
	char codeTxt[codeTxtBufferSize];

	switch(code)
	{
		case Finish:
			snprintf(number, numberBufferSize, "Finish");
			snprintf(codeTxt, codeTxtBufferSize, "MO");
		break;
		case F1:
			snprintf(number, numberBufferSize, "#1");
			snprintf(codeTxt, codeTxtBufferSize, "MOE");
		break;
		case F2:
			snprintf(number, numberBufferSize, "#2");
			snprintf(codeTxt, codeTxtBufferSize, "MOI");
		break;
		case F3:
			snprintf(number, numberBufferSize, "#3");
			snprintf(codeTxt, codeTxtBufferSize, "MOS");
		break;
		case F4:
			snprintf(number, numberBufferSize, "#4");
			snprintf(codeTxt, codeTxtBufferSize, "MOH");
		break;
		case F5:
			snprintf(number, numberBufferSize, "#5");
			snprintf(codeTxt, codeTxtBufferSize, "MO5");
		break;
		case Beacon:
			snprintf(number, numberBufferSize, "Beacon");
			snprintf(codeTxt, codeTxtBufferSize, "S");
		break;
		default:
			L2HAL_Error(Generic);
		break;
	}

	/* Adding fox speed */
	uint8_t speedBufferSize = 4;
	char speed[speedBufferSize];
	if (isFast)
	{
		snprintf(speed, speedBufferSize, "F");
	}
	else
	{
		snprintf(speed, speedBufferSize, "S");
	}

	uint8_t codeTxtWithSpeedBufferSize = 16;
	char codeTxtWithSpeed[codeTxtWithSpeedBufferSize];
	snprintf(codeTxtWithSpeed, codeTxtWithSpeedBufferSize, "%s:%s", codeTxt, speed);

	/* Dry runs*/
	uint16_t numberWidth;
	uint16_t codeWidth;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &numberWidth, NULL, true, number);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &codeWidth, NULL, true, codeTxtWithSpeed);

	uint16_t numberSpacing = (uint16_t)(availableWidth - numberWidth) / 2U;
	uint16_t codeSpacing = (uint16_t)(availableWidth - codeWidth) / 2U;

	uint16_t lastPixel = (uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - 1U);

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)(lastPixel - (numberWidth + numberSpacing)), YHL_CODE_LINE1_TOP, NULL, NULL, false, number);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)(lastPixel - (codeWidth + codeSpacing)), YHL_CODE_LINE2_TOP, NULL, NULL, false, codeTxtWithSpeed);
}

void DrawFoxCycle(FoxCycleStruct cycle)
{
	FMGL_API_DrawRectangleFilled(&fmglContext, 0, YHL_CYCLE_CLEAR_TOP, (uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - 1U), YHL_CYCLE_CLEAR_BOTTOM, OffColor, OffColor);

	uint8_t bufferSize = 32;
	char buffer[bufferSize];

	uint8_t cycleTimeTxtSize = 16;
	char cycleTimeTxt[cycleTimeTxtSize];

	if (cycle.IsContinuous)
	{
		snprintf(cycleTimeTxt, cycleTimeTxtSize, "continuous");
	}
	else
	{
		uint8_t txTimeTxtSize = 8;
		char txTimeTxt[txTimeTxtSize];

		uint8_t pauseTimeTxtSize = 8;
		char pauseTimeTxt[pauseTimeTxtSize];

		TimespanToMSString(cycle.TxTime, txTimeTxt, txTimeTxtSize);
		TimespanToMSString(cycle.PauseTime, pauseTimeTxt, pauseTimeTxtSize);

		snprintf(cycleTimeTxt, cycleTimeTxtSize, "%s/%s", txTimeTxt, pauseTimeTxt);
	}

	snprintf(buffer, bufferSize, "Cycle: %s", cycleTimeTxt);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_CYCLE_LEFT, YHL_CYCLE_TOP, NULL, NULL, false, buffer);
}

void DrawEndingTone(uint8_t endingToneLength, FoxCycleStruct cycle)
{
	FMGL_API_DrawRectangleFilled(&fmglContext,
			0,
			YHL_ENDING_TONE_CLEAR_TOP,
			(uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - 1U),
			YHL_ENDING_TONE_CLEAR_BOTTOM,
			OffColor,
			OffColor);

	uint8_t bufferSize = 32;
	char buffer[bufferSize];

	if (cycle.IsContinuous)
	{
		snprintf(buffer, bufferSize, "Ending tone: N/A");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_ENDING_TONE_LEFT, YHL_ENDING_TONE_TOP, NULL, NULL, false, buffer);
		return;
	}

	snprintf(buffer, bufferSize, "Ending tone: %ds", endingToneLength);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_ENDING_TONE_LEFT, YHL_ENDING_TONE_TOP, NULL, NULL, false, buffer);
}

void DrawFoxCycleState(CycleStateStruct cycleState, uint32_t currentTime, FoxCycleStruct cycle, GlobalFoxStateStruct globalState)
{
	FMGL_API_DrawRectangleFilled(&fmglContext,
			0,
			YHL_CYCLE_STATE_CLEAR_TOP,
			(uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - 1U),
			YHL_CYCLE_STATE_CLEAR_BOTTOM,
			OffColor,
			OffColor);

	if (GfsBeforeFinish != globalState.CurrentState)
	{
		uint8_t bufferSize = 32;
		char buffer[bufferSize];
		snprintf(buffer, bufferSize, "Waiting for start");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_CYCLE_STATE_LEFT, YHL_CYCLE_STATE_TOP, NULL, NULL, false, buffer);
		return;
	}

	if (true == cycle.IsContinuous)
	{
		uint8_t bufferSize = 32;
		char buffer[bufferSize];
		snprintf(buffer, bufferSize, "N/A: Continuous cycle");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_CYCLE_STATE_LEFT, YHL_CYCLE_STATE_TOP, NULL, NULL, false, buffer);
		return;
	}

	uint32_t timeTillAction = cycleState.StateChangeTime - currentTime;

	uint8_t timeBufferSize = 16;
	char timeBuffer[timeBufferSize];
	TimespanToHMSString(timeTillAction, timeBuffer, timeBufferSize);

	uint8_t actionBufferSize = 16;
	char actionBuffer[actionBufferSize];
	switch(cycleState.CycleState)
	{
		case CsTx:
		case CsEndingTone:
			snprintf(actionBuffer, actionBufferSize, "pause");
		break;
		case CsPause:
		case CsPreparation:
		case CsReady:
			snprintf(actionBuffer, actionBufferSize, "TX");
		break;
		default:
			L2HAL_Error(Generic);
		break;
	}

	uint8_t bufferSize = 32;
	char buffer[bufferSize];
	snprintf(buffer, bufferSize, "%s till %s", timeBuffer, actionBuffer);

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_CYCLE_STATE_LEFT, YHL_CYCLE_STATE_TOP, NULL, NULL, false, buffer);
}

void DrawGlobalState(GlobalFoxStateStruct globalState, uint32_t currentTime)
{
	FMGL_API_DrawRectangleFilled(&fmglContext,
			0,
			YHL_GLOBAL_STATE_CLEAR_TOP,
			(uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - 1U),
			YHL_GLOBAL_STATE_CLEAR_BOTTOM,
			OffColor,
			OffColor);

	if (GfsStandby == globalState.CurrentState)
	{
		uint8_t bufferSize = 32;
		char buffer[bufferSize];
		snprintf(buffer, bufferSize, "Standing by");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_GLOBAL_STATE_LEFT, YHL_GLOBAL_STATE_TOP, NULL, NULL, false, buffer);
		return;
	}

	uint32_t secondsTillAction = globalState.StateChangeTime - currentTime;

	uint8_t timeBufferSize = 16;
	char timeBuffer[timeBufferSize];
	TimespanToHMSString(secondsTillAction, timeBuffer, timeBufferSize);

	uint8_t actionBufferSize = 16;
	char actionBuffer[actionBufferSize];
	switch(globalState.CurrentState)
	{
		case GfsPreparation:
		case GfsReady:
		case GfsBeforeStart:
			snprintf(actionBuffer, actionBufferSize, "start");
		break;
		case GfsBeforeFinish:
			snprintf(actionBuffer, actionBufferSize, "finish");
		break;
		default:
			L2HAL_Error(Generic);
		break;
	}

	uint8_t bufferSize = 32;
	char buffer[bufferSize];
	snprintf(buffer, bufferSize, "%s till %s", timeBuffer, actionBuffer);

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_GLOBAL_STATE_LEFT, YHL_GLOBAL_STATE_TOP, NULL, NULL, false, buffer);
}

void DrawFoxPower(float power, FoxFrequencyStruct frequency)
{
	FMGL_API_DrawRectangleFilled(&fmglContext,
			0,
			YHL_POWER_CLEAR_TOP,
			(uint16_t)(FMGL_API_GetDisplayWidth(&fmglContext) - 1U),
			YHL_POWER_CLEAR_BOTTOM,
			OffColor,
			OffColor);

	uint8_t bufferSize = 32;
	char buffer[bufferSize];
	if (frequency.Is144MHz)
	{
		snprintf(buffer, bufferSize, "Power: N/A");
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_POWER_LEFT, YHL_POWER_TOP, NULL, NULL, false, buffer);
		return;
	}

	snprintf(buffer, bufferSize, "Power: %.1fW", power);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, YHL_POWER_LEFT, YHL_POWER_TOP, NULL, NULL, false, buffer);
}
