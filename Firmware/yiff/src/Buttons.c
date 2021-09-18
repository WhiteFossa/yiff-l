/*
 * Buttons.c
 *
 *  Created on: Apr 13, 2021
 *      Author: fossa
 */

#include <Buttons.h>

void DrawButtons(void)
{
	DrawButton(&LeftButton, YHL_BUTTONS_LEFT_BUTTON_LEFT, YHL_BUTTONS_TOP_LINE, YHL_BUTTONS_WIDTH, YHL_BUTTONS_HEIGHT);
	DrawButton(&RightButton, YHL_BUTTONS_RIGHT_BUTTON_LEFT, YHL_BUTTONS_TOP_LINE, YHL_BUTTONS_WIDTH, YHL_BUTTONS_HEIGHT);
}

void DrawButton(FoxButtonStruct* button, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	FMGL_API_FontSettings font;
	FMGL_API_ColorStruct fillColor;
	if (button->IsPressed)
	{
		font = invertedCommonFont;
		fillColor = OnColor;
	}
	else
	{
		font = commonFont;
		fillColor = OffColor;
	}

	FMGL_API_DrawRectangleFilled(&fmglContext,
				x,
				y,
				(uint16_t)(x + width),
				(uint16_t)(y + height),
				OnColor,
				fillColor);

	/* Text dry run */
	uint16_t textWidth, textHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &textWidth, &textHeight, true, button->Text);

	int16_t spacingX, spacingY;
	spacingX = ((int16_t)width - (int16_t)textWidth) / 2;
	spacingY = ((int16_t)height - (int16_t)textHeight) / 2;

	if (spacingX < 0) { spacingX = 0; }
	if (spacingY < 0) { spacingY = 0; }

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &font, (uint16_t)(x + spacingX), (uint16_t)(y + spacingY), NULL, NULL, false, button->Text);
}
