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

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	uint16_t displayWidth = FMGL_API_GetDisplayWidth(&fmglContext);
	uint16_t displayHeight = FMGL_API_GetDisplayHeight(&fmglContext);

	/* Title */
	uint16_t titleWidth;
	uint16_t titleHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &titleWidth, &titleHeight, true, title);

	int16_t titleXShift = ((int16_t)displayWidth - (int16_t)titleWidth) / 2;
	if (titleXShift < 0)
	{
		/* Didn't fit, drawing as is */
		titleXShift = 0;
	}
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)titleXShift, YHL_INFORMAION_POPUP_TITLE_TOP, NULL, NULL, false, title);

	/* Message */
	uint16_t usableYSpace = displayHeight - titleHeight - YHL_BUTTONS_HEIGHT;
	uint16_t messageWidth;
	uint16_t messageHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &messageWidth, &messageHeight, true, message);

	int messageXShift = (displayWidth - messageWidth) / 2;
	if (messageXShift < 0)
	{
		messageXShift = 0;
	}

	int messageYShift = ((usableYSpace - messageHeight) / 2) + titleHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)messageXShift, (uint16_t)messageYShift, NULL, NULL, false, message);

	/* Buttons */
	strcpy(LeftButton.Text, "Back");
	strcpy(RightButton.Text, "Back");
	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

void InformationPopup_Close(void)
{
	FoxState.CurrentDisplay = InformationPopup_PreviousDisplay;

	FMGL_API_ClearScreen(&fmglContext);
}
