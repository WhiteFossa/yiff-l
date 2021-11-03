/*
 * MatchingDisplay.c
 *
 *  Created on: Jul 31, 2021
 *      Author: fossa
 */

#include <MatchingDisplay.h>

void DrawMatchingDisplay(FoxStateStruct foxState)
{
	if (foxState.CurrentDisplay != AntennaMatchingDisplay)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_DrawMatchingDisplayWhenNoMatchingDisplay);
	}

	if (!foxState.GlobalState.IsMatchingInProgress)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_DrawMatchingDisplayWhenNoMatchingInProgress);
	}

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	/* Matching graph */
	DrawMatchingGraph(foxState.MatchingDisplayData);

	/* Bottomline - matching step*/
	DrawMatchingStatusString(foxState.MatchingDisplayData.MatchingStep);

	FMGL_API_PushFramebuffer(&fmglContext);
}

void DrawMatchingStatusString(uint8_t step)
{
	uint8_t bufferSize = 32;
	char buffer[bufferSize];
	snprintf(buffer, bufferSize, "Matching: %d/%d", step, HAL_AM_MAX_VALUE);

	uint16_t stringWidth;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &stringWidth, NULL, true, buffer);

	int32_t spacing = (FMGL_API_GetDisplayWidth(&fmglContext) - stringWidth) / 2;
	if (spacing < 0)
	{
		spacing = 0;
	}

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)spacing, YHL_ANTENNA_MATCHING_PROGRESS_TOP, NULL, NULL, false, buffer);
}

void DrawMatchingGraph(MatchingDisplayStruct matchingData)
{
	float maxValue = 0;
	for (uint8_t step = 0; step < HAL_AM_MAX_VALUE; step++)
	{
		if (matchingData.MatchingLevels[step] > maxValue)
		{
			maxValue = matchingData.MatchingLevels[step];
		}
	}

	/* Protection from division by zero */
	if (0 == maxValue)
	{
		maxValue = 0.1f;
	}

	float scalingFactor = YHL_ANTENNA_MATCHING_GRAPH_BOTTOM / maxValue;

	for (uint8_t step = 0; step < HAL_AM_MAX_VALUE; step++)
	{
		uint16_t x = (uint16_t)step * 2U;
		int32_t y = YHL_ANTENNA_MATCHING_GRAPH_BOTTOM - floor(matchingData.MatchingLevels[step] * scalingFactor + 0.5f);
		if (y < 0)
		{
			y = 0;
		}

		FMGL_API_DrawRectangleFilled(&fmglContext, x, YHL_ANTENNA_MATCHING_GRAPH_BOTTOM, x + 1, (uint16_t)y, OnColor, OnColor);
	}
}
