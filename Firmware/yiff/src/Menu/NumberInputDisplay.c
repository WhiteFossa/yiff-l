/*
 * NumberInputDisplay.c
 *
 *  Created on: Sep 19, 2021
 *      Author: fossa
 */

#include <Menu/NumberInputDisplay.h>
#include <Menu/NumberInputDisplayPrivate.h>

void NumberInputDisplay_Show(char* title,
		int32_t minValue,
		int32_t maxValue,
		int32_t initialValue,
		int32_t step,
		char* (*formatter)(int32_t valueToFormat),
		void (*onCloseHandler)(int32_t enteredValue),
		FoxDisplayEnum previousDisplay)
{
	if (NULL == formatter)
	{
		L2HAL_Error(Generic);
	}

	if (NULL == onCloseHandler)
	{
		L2HAL_Error(Generic);
	}

	NumberInputDisplay_Formatter = formatter;
	NumberInputDisplay_OnCloseHandler = onCloseHandler;

	NumberInputDisplay_MinValue = minValue;
	NumberInputDisplay_MaxValue = maxValue;
	NumberInputDisplay_InitialValue = initialValue;
	NumberInputDisplay_CurrentValue = initialValue;
	NumberInputDisplay_Step = step;

	NumberInputDisplay_PreviousDisplay = previousDisplay;
	FoxState.CurrentDisplay = NumberInputDisplay;

	NumberInputDisplay_Title = title;

	NumberInputDisplay_Display();
}

void NumberInputDisplay_LeftClickHandler(void)
{
	NumberInputDisplay_Close();
	NumberInputDisplay_OnCloseHandler(NumberInputDisplay_CurrentValue);
}


void NumberInputDisplay_EncoderClickHandler(void)
{
	NumberInputDisplay_LeftClickHandler();
}


void NumberInputDisplay_RightClickHandler(void)
{
	NumberInputDisplay_Close();
	NumberInputDisplay_OnCloseHandler(NumberInputDisplay_InitialValue);
}


void NumberInputDisplay_EncoderRotationHandler(int8_t direction)
{
	if (HAL_ENCODER_ROTATION_CLOCKWISE == direction)
	{
		NumberInputDisplay_CurrentValue += NumberInputDisplay_Step;
	}
	else if (HAL_ENCODER_ROTATION_COUNTERCLOCKWISE == direction)
	{
		NumberInputDisplay_CurrentValue -= NumberInputDisplay_Step;
	}

	if (NumberInputDisplay_CurrentValue < NumberInputDisplay_MinValue)
	{
		NumberInputDisplay_CurrentValue = NumberInputDisplay_MinValue;
	}
	else if (NumberInputDisplay_CurrentValue > NumberInputDisplay_MaxValue)
	{
		NumberInputDisplay_CurrentValue = NumberInputDisplay_MaxValue;
	}

	NumberInputDisplay_Display();
}

void NumberInputDisplay_Display(void)
{
	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	/* Title */
	uint16_t displayWidth = FMGL_API_GetDisplayWidth(&fmglContext);
	uint16_t displayHeight = FMGL_API_GetDisplayHeight(&fmglContext);

	/* Title */
	uint16_t titleWidth;
	uint16_t titleHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &titleWidth, &titleHeight, true, NumberInputDisplay_Title);

	int32_t titleXShift = (displayWidth - titleWidth) / 2;
	if (titleXShift < 0)
	{
		/* Didn't fit, drawing as is */
		titleXShift = 0;
	}
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)titleXShift, YHL_NUMBER_INPUT_DISPLAY_TITLE_TOP, NULL, NULL, false, NumberInputDisplay_Title);

	/* Value */
	int32_t usableYSpace = displayHeight - titleHeight - YHL_BUTTONS_HEIGHT;

	if (usableYSpace < 0)
	{
		usableYSpace = 0;
	}

	char* valueText = NumberInputDisplay_Formatter(NumberInputDisplay_CurrentValue);

	uint16_t valueWidth;
	uint16_t valueHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &valueWidth, &valueHeight, true, valueText);

	int32_t valueXShift = (displayWidth - valueWidth) / 2;
	if (valueXShift < 0)
	{
		valueXShift = 0;
	}

	int32_t valueYShift = ((usableYSpace - valueHeight) / 2) + titleHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)valueXShift, (uint16_t)valueYShift, NULL, NULL, false, valueText);

	free(valueText);

	/* Buttons */
	strncpy(LeftButton.Text, "OK", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);
	strncpy(RightButton.Text, "Cancel", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);
	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

void NumberInputDisplay_Close(void)
{
	FoxState.CurrentDisplay = NumberInputDisplay_PreviousDisplay;

	FMGL_API_ClearScreen(&fmglContext);
}
