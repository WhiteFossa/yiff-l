/*
 * InformationPopupDisplay.c
 *
 *  Created on: Sep 11, 2021
 *      Author: fossa
 */

#include <Menu/InformationPopupDisplay.h>
#include <Menu/InformationPopupDisplayPrivate.h>

void InformationPopup_Show(char* title, char* message, FoxDisplayEnum previousDisplay)
{
	InformationPopup_PreviousDisplay = previousDisplay;
	FoxState.CurrentDisplay = InformationPopupDisplay;

	if (FoxState.SupressDrawing)
	{
		return;
	}

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	uint16_t displayWidth = FMGL_API_GetDisplayWidth(&fmglContext);
	uint16_t displayHeight = FMGL_API_GetDisplayHeight(&fmglContext);

	/* Title */
	uint16_t titleWidth;
	uint16_t titleHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &titleWidth, &titleHeight, true, title);

	int32_t titleXShift = (displayWidth - titleWidth) / 2;
	if (titleXShift < 0)
	{
		/* Didn't fit, drawing as is */
		titleXShift = 0;
	}
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)titleXShift, YHL_INFORMAION_POPUP_TITLE_TOP, NULL, NULL, false, title);

	/* Message */
	int32_t usableYSpace = displayHeight - titleHeight - YHL_BUTTONS_HEIGHT;

	if (usableYSpace < 0)
	{
		usableYSpace = 0;
	}

	uint16_t messageWidth;
	uint16_t messageHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &messageWidth, &messageHeight, true, message);

	int32_t messageXShift = (displayWidth - messageWidth) / 2;
	if (messageXShift < 0)
	{
		messageXShift = 0;
	}

	int32_t messageYShift = ((usableYSpace - messageHeight) / 2) + titleHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)messageXShift, (uint16_t)messageYShift, NULL, NULL, false, message);

	/* Buttons */
	strncpy(LeftButton.Text, "Back", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);
	strncpy(RightButton.Text, "Back", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);
	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

void InformationPopup_Close(void)
{
	FoxState.CurrentDisplay = InformationPopup_PreviousDisplay;

	if (FoxState.SupressDrawing)
	{
		return;
	}

	FMGL_API_ClearScreen(&fmglContext);
}
