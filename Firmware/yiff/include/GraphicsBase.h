/*
 * GraphicsBase.h
 *
 *  Created on: Apr 7, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_GRAPHICSBASE_H_
#define INCLUDE_GRAPHICSBASE_H_

#include <fmgl.h>
#include <terminusRegular12.h>

/**
 * FMGL context.
 */
FMGL_API_DriverContext fmglContext;

/**
 * Common font.
 */
FMGL_API_FontSettings commonFont;

/**
 * Inverted common font.
 */
FMGL_API_FontSettings invertedCommonFont;

/**
 * Big font for displaying frequency.
 */
FMGL_API_FontSettings frequencyFont;

/**
 * Off color.
 */
FMGL_API_ColorStruct OffColor;

/**
 * On color.
 */
FMGL_API_ColorStruct OnColor;



#endif /* INCLUDE_GRAPHICSBASE_H_ */
