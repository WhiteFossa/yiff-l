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
		L2HAL_Error(Generic);
	}

	if (!foxState.MatchingDisplayData.IsMatchingInProgress)
	{
		L2HAL_Error(Generic);
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
	char buffer[32];
	sprintf(buffer, "Matching: %d/%d", step, HAL_AM_MAX_VALUE);

	uint16_t stringWidth;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &stringWidth, NULL, true, buffer);

	int16_t spacing = (FMGL_API_GetDisplayWidth(&fmglContext) - stringWidth) / 2;
	if (spacing < 0)
	{
		spacing = 0;
	}

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, spacing, YHL_ANTENNA_MATCHING_PROGRESS_TOP, NULL, NULL, false, buffer);
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

	if (0 == maxValue)
	{
		maxValue = 0.1f;
	}

	float scalingFactor = YHL_ANTENNA_MATCHING_GRAPH_BOTTOM / maxValue;

	for (uint8_t step = 0; step < HAL_AM_MAX_VALUE; step++)
	{
		uint16_t x = (uint16_t)step * 2U;
		uint16_t y = YHL_ANTENNA_MATCHING_GRAPH_BOTTOM - floor(matchingData.MatchingLevels[step] * scalingFactor + 0.5f);
		if (y < 0)
		{
			y = 0;
		}

		FMGL_API_DrawRectangleFilled(&fmglContext, x, YHL_ANTENNA_MATCHING_GRAPH_BOTTOM, x + 1, y, OnColor, OnColor);
	}
}
