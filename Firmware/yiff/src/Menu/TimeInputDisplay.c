/*
 * TimeInputDisplay.c
 *
 *  Created on: Oct 24, 2021
 *      Author: fossa
 */

#include <Menu/TimeInputDisplay.h>
#include <Menu/TimeInputDisplayPrivate.h>

void TimeInputDisplay_Show(char* title,
		uint32_t minTime,
		uint32_t maxTime,
		uint32_t initialTime,
		void (*onCloseHandler)(uint32_t enteredTime),
		FoxDisplayEnum previousDisplay)
{
	if (NULL == onCloseHandler)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_NoCloseHandlerInTimeInputDisplay);
	}

	TimeInputDisplay_OnCloseHandler = onCloseHandler;

	TimeInputDisplay_MinTime = minTime;
	TimeInputDisplay_MaxTime = maxTime;
	TimeInputDisplay_InitialTime = initialTime;
	TimeInputDisplay_CurrentTime = initialTime;
	TimeInputDisplay_Title = title;
	TimeInputDisplay_PreviousDisplay = previousDisplay;

	FoxState.CurrentDisplay = TimeInputDisplay;

	Time initialTimeAsTimeStruct = TimespanToTime(TimeInputDisplay_InitialTime);
	TimeInputDisplay_HoursValue = initialTimeAsTimeStruct.Hours;
	TimeInputDisplay_MinutesValue = initialTimeAsTimeStruct.Minutes;
	TimeInputDisplay_SecondsValue = initialTimeAsTimeStruct.Seconds;

	/* Initially we are entering seconds */
	TimeInputDisplay_ActiveValue = TimeInputDisplay_Seconds;

	TimeInputDisplay_Display();
}

void TimeInputDisplay_Display()
{
	if (FoxState.SupressDrawing)
	{
		return;
	}

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	/* Title */
	uint16_t displayWidth = FMGL_API_GetDisplayWidth(&fmglContext);
	uint16_t displayHeight = FMGL_API_GetDisplayHeight(&fmglContext);

	/* Title */
	uint16_t titleWidth;
	uint16_t titleHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &titleWidth, &titleHeight, true, TimeInputDisplay_Title);

	int32_t titleXShift = (displayWidth - titleWidth) / 2;
	if (titleXShift < 0)
	{
		/* Didn't fit, drawing as is */
		titleXShift = 0;
	}
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)titleXShift, YHL_TIME_INPUT_DISPLAY_TITLE_TOP, NULL, NULL, false, TimeInputDisplay_Title);

	/* Value */
	int32_t usableYSpace = displayHeight - titleHeight - YHL_BUTTONS_HEIGHT;

	if (usableYSpace < 0)
	{
		usableYSpace = 0;
	}

	char* formatString = TimeInputDisplay_CostructTimeFormatString(TimeInputDisplay_ActiveValue);
	char valueText[32];
	snprintf(valueText, 32, formatString, TimeInputDisplay_HoursValue, TimeInputDisplay_MinutesValue, TimeInputDisplay_SecondsValue);
	free(formatString);

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


	/* Buttons */
	strncpy(LeftButton.Text, "OK", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);
	strncpy(RightButton.Text, "Cancel", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);
	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

char* TimeInputDisplay_CostructTimeFormatString(TimeInputDisplay_ActiveValueEnum activeValue)
{
	char* buffer = malloc(32);

	switch (activeValue)
	{
		case TimeInputDisplay_Hours:
			strncpy(buffer, "[%02d] : %02d : %02d", 32);
			break;

		case TimeInputDisplay_Minutes:
			strncpy(buffer, "%02d : [%02d] : %02d", 32);
			break;

		case TimeInputDisplay_Seconds:
			strncpy(buffer, "%02d : %02d : [%02d]", 32);
			break;

		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongActiveValueInTimeInputDisplayCostructTimeFormatString);
	}

	return buffer;
}

void TimeInputDisplay_LeftClickHandler()
{
	TimeInputDisplay_Close();
	TimeInputDisplay_OnCloseHandler(TimeInputDisplay_CurrentTime);
}


void TimeInputDisplay_EncoderClickHandler()
{
	switch(TimeInputDisplay_ActiveValue)
	{
	case TimeInputDisplay_Hours:
		TimeInputDisplay_ActiveValue = TimeInputDisplay_Seconds;
		break;

	case TimeInputDisplay_Minutes:
		TimeInputDisplay_ActiveValue = TimeInputDisplay_Hours;
		break;

	case TimeInputDisplay_Seconds:
		TimeInputDisplay_ActiveValue = TimeInputDisplay_Minutes;
		break;

	default:
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongActiveValueInTimeInputDisplayEncoderClickHandler);
	}

	TimeInputDisplay_Display();
}


void TimeInputDisplay_RightClickHandler()
{
	TimeInputDisplay_Close();
	TimeInputDisplay_OnCloseHandler(TimeInputDisplay_InitialTime);
}


void TimeInputDisplay_EncoderRotationHandler(int8_t direction)
{
	uint8_t preRotationHours = TimeInputDisplay_HoursValue;
	uint8_t preRotationMinutes = TimeInputDisplay_MinutesValue;
	uint8_t preRotationSeconds = TimeInputDisplay_SecondsValue;

	int8_t delta;
	if (HAL_ENCODER_ROTATION_CLOCKWISE == direction)
	{
		delta = 1;
	}
	else
	{
		delta = -1;
	}

	uint8_t* currentValuePtr;

	switch(TimeInputDisplay_ActiveValue)
	{
	case TimeInputDisplay_Hours:
		currentValuePtr = &TimeInputDisplay_HoursValue;
		break;

	case TimeInputDisplay_Minutes:
		currentValuePtr = &TimeInputDisplay_MinutesValue;
		break;

	case TimeInputDisplay_Seconds:
		currentValuePtr = &TimeInputDisplay_SecondsValue;
		break;

	default:
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongActiveValueInTimeInputDisplayEncoderRotationHandler);
	}

	int16_t currentValue = *currentValuePtr;

	currentValue += delta;

	int8_t maxValue;
	if (TimeInputDisplay_Hours == TimeInputDisplay_ActiveValue)
	{
		maxValue = 23;
	}
	else
	{
		maxValue = 59;
	}

	if (currentValue < 0)
	{
		currentValue = maxValue;
	}
	else if (currentValue > maxValue)
	{
		currentValue = 0;
	}

	*currentValuePtr = (uint8_t)currentValue;

	Time currentValueAsTime;
	currentValueAsTime.Days = 0;
	currentValueAsTime.Hours = TimeInputDisplay_HoursValue;
	currentValueAsTime.Minutes = TimeInputDisplay_MinutesValue;
	currentValueAsTime.Seconds = TimeInputDisplay_SecondsValue;

	uint32_t currentValueAsTimespan = TimeToTimespan(currentValueAsTime);

	if (currentValueAsTimespan < TimeInputDisplay_MinTime || currentValueAsTimespan > TimeInputDisplay_MaxTime)
	{
		TimeInputDisplay_HoursValue = preRotationHours;
		TimeInputDisplay_MinutesValue = preRotationMinutes;
		TimeInputDisplay_SecondsValue = preRotationSeconds;
	}
	else
	{
		TimeInputDisplay_CurrentTime = currentValueAsTimespan;
	}

	TimeInputDisplay_Display();
}

void TimeInputDisplay_Close()
{
	FoxState.CurrentDisplay = TimeInputDisplay_PreviousDisplay;

	if (FoxState.SupressDrawing)
	{
		return;
	}

	FMGL_API_ClearScreen(&fmglContext);
}
