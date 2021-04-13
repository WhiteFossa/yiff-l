/*
 * Buttons.c
 *
 *  Created on: Apr 13, 2021
 *      Author: fossa
 */

#include <Buttons.h>

void DrawButtons()
{
	DrawButton(LeftButton, 0, 110, 59, 17);
	DrawButton(RightButton, 68, 110, 59, 17);

	FMGL_API_PushFramebuffer(&fmglContext);
}

void DrawButton(FoxButtonStruct button, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	FMGL_API_FontSettings font;
	if (button.IsPressed)
	{
		font = invertedCommonFont;
		FMGL_API_DrawRectangleFilled(&fmglContext,
			x,
			y,
			x + width,
			y + height,
			OnColor,
			OnColor);
	}
	else
	{
		font = commonFont;
		FMGL_API_SetActiveColor(&fmglContext, OnColor);
		FMGL_API_DrawRectangle(&fmglContext,
			x,
			y,
			x + width,
			y + height);
	}

	FMGL_API_PushFramebuffer(&fmglContext);

	/* Text dry run */
	uint16_t textWidth, textHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &textWidth, &textHeight, true, button.Text);

	int16_t spacingX, spacingY;
	spacingX = (width - textWidth) / 2;
	spacingY = (height - textHeight) / 2;

	if (spacingX < 0) { spacingX = 0; }
	if (spacingY < 0) { spacingY = 0; }

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &font, x + spacingX, y + spacingY, NULL, NULL, false, button.Text);
}
