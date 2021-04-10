/*
 * StatusDisplay.c
 *
 *  Created on: Apr 7, 2021
 *      Author: fossa
 */

#include <StatusDisplay.h>

void DrawStatusDisplay(FoxStateStruct foxState)
{
	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	/* 1st line */
	DrawBattery(foxState.BatteryLevel);
	DrawCurrentTime(foxState.CurrentTime);
	DrawTXStatus(foxState.IsTXOn);

	/* 2nd line */
	DrawFoxName(foxState.Name);

	/* 3rd line */
	uint16_t frequencyTextWidth = DrawFoxFrequency(foxState.Frequency);
	DrawFoxCode(foxState.Code, FMGL_API_GetDisplayWidth(&fmglContext) - frequencyTextWidth);

	/* 4rd line */
	char buffer[32];
	sprintf(buffer, "Cycle: 01:00/05:00");
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 46, NULL, NULL, false, buffer);

	sprintf(buffer, "01:33:37 till start");
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 58, NULL, NULL, false, buffer);

	FMGL_API_PushFramebuffer(&fmglContext);
}

void DrawBattery(float level)
{
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
	char buffer[16];
	sprintf(buffer, "%02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 66, 1, NULL, NULL, false, buffer);
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
