/*
 * Buttons.h
 *
 *  Created on: Apr 13, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_BUTTONS_H_
#define INCLUDE_BUTTONS_H_

#include <GraphicsBase.h>

/**
 * Text on buttons must be no longer then this
 */
#define YHL_MAX_BUTTON_TEXT_LENGTH 16

/**
 * Button descriptor.
 */
typedef struct
{
	/**
	 * True, if button is pressed (such button is displayed inverted).
	 */
	bool IsPressed;

	/**
	 * Button text.
	 */
	char Text[YHL_MAX_BUTTON_TEXT_LENGTH + 1];
}
FoxButtonStruct;

extern FoxButtonStruct LeftButton;
extern FoxButtonStruct RightButton;

/**
 * Draw buttons (does not flush framebuffer, flush it yourself)
 */
void DrawButtons(void);

void DrawButton(FoxButtonStruct button, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#endif /* INCLUDE_BUTTONS_H_ */
