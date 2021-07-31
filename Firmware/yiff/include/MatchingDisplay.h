/*
 * MatchingDisplay.h
 *
 *  Created on: Jul 31, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MATCHINGDISPLAY_H_
#define INCLUDE_MATCHINGDISPLAY_H_

#include <GraphicsBase.h>
#include <FoxStatus.h>
#include <HAL.h>

/**
 * Topmost pixel for antenna matching status string
 */
#define YHL_ANTENNA_MATCHING_PROGRESS_TOP 116

/**
 * Bottommost pixel of antenna matching graph
 */
#define YHL_ANTENNA_MATCHING_GRAPH_BOTTOM 115

/**
 * Draw complete matching display
 */
void DrawMatchingDisplay(FoxStateStruct foxState);

/* Draw status string with matching progress*/
void DrawMatchingStatusString(uint8_t step);

/**
 * Draw antenna matching graph
 */
void DrawMatchingGraph(MatchingDisplayStruct matchingData);

#endif /* INCLUDE_MATCHINGDISPLAY_H_ */
