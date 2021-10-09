/*
 * NumberInputDisplay.c
 *
 *  Created on: Sep 19, 2021
 *      Author: fossa
 */

#include <Menu/NumberInputDisplay.h>
#include <Menu/NumberInputDisplayPrivate.h>

void NumberInputDisplay_Show(char* title,
		float minValue,
		float maxValue,
		float initialValue,
		float step,
		void (*onCloseHandler)(float enteredValue),
		FoxDisplayEnum previousDisplay)
{
	if (NULL == onCloseHandler)
	{
		L2HAL_Error(Generic);
	}

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

}


void NumberInputDisplay_RightClickHandler(void)
{
	NumberInputDisplay_Close();
	NumberInputDisplay_OnCloseHandler(NumberInputDisplay_InitialValue);
}


void NumberInputDisplay_EncoderRotationHandler(int8_t direction)
{

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

	int16_t titleXShift = ((int16_t)displayWidth - (int16_t)titleWidth) / 2;
	if (titleXShift < 0)
	{
		/* Didn't fit, drawing as is */
		titleXShift = 0;
	}
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)titleXShift, YHL_NUMBER_INPUT_DISPLAY_TITLE_TOP, NULL, NULL, false, NumberInputDisplay_Title);


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
