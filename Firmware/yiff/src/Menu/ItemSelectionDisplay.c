/*
 * ItemSelectionDisplay.c
 *
 *  Created on: Sep 12, 2021
 *      Author: fossa
 */

#include <Menu/ItemSelectionDisplay.h>
#include <Menu/ItemSelectionDisplayPrivate.h>

void ItemSelectionDisplay_Show(char* title,
		char* itemsPtr,
		uint8_t itemSize,
		uint8_t itemsCount,
		uint8_t selectedItemIndex,
		void (*onCloseHandler)(uint8_t newSelectedItemIndex),
		FoxDisplayEnum previousDisplay)
{
	if (NULL == onCloseHandler)
	{
		L2HAL_Error(Generic);
	}

	ItemSelectionDisplay_PreviousDisplay = previousDisplay;
	FoxState.CurrentDisplay = ItemSelectionDisplay;

	ItemSelectionDisplay_OnCloseHandler = onCloseHandler;
	ItemSelectionDisplay_ItemsCount = itemsCount;
	ItemSelectionDisplay_InitiallySelectedIndex = selectedItemIndex;
	ItemSelectionDisplay_SelectedItemIndex = selectedItemIndex;
	ItemSelectionDisplay_ActiveLineIndex = 0;
	ItemSelectionDisplay_BaseLine = 0;
	ItemSelectionDisplay_ItemSize = itemSize;
	ItemSelectionDisplay_ItemsPtr = itemsPtr;

	if (ItemSelectionDisplay_ItemsCount > YHL_ITEM_SELECTION_DISPLAY_MAX_LINES)
	{
		ItemSelectionDisplay_WindowLinesCount = YHL_ITEM_SELECTION_DISPLAY_MAX_LINES;
	}
	else
	{
		ItemSelectionDisplay_WindowLinesCount = ItemSelectionDisplay_ItemsCount;
	}

	ItemSelectionDisplay_Title = title;

	ItemSelectionDisplay_Display();
}

void ItemSelectionDisplay_Display(void)
{
	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	uint16_t displayWidth = FMGL_API_GetDisplayWidth(&fmglContext);
	uint16_t displayHeight = FMGL_API_GetDisplayHeight(&fmglContext);

	/* Title */
	uint16_t titleWidth;
	uint16_t titleHeight;
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, &titleWidth, &titleHeight, true, ItemSelectionDisplay_Title);

	int16_t titleXShift = ((int16_t)displayWidth - (int16_t)titleWidth) / 2;
	if (titleXShift < 0)
	{
		/* Didn't fit, drawing as is */
		titleXShift = 0;
	}
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, (uint16_t)titleXShift, YHL_ITEM_SELECTION_DISPLAY_TITLE_TOP, NULL, NULL, false, ItemSelectionDisplay_Title);

	/* Lines */
	char window[YHL_ITEM_SELECTION_DISPLAY_MAX_LINES][YHL_ITEM_SELECTION_DISPLAY_TEXT_MEMORY_SIZE];

	for (uint8_t line = 0; line < ItemSelectionDisplay_WindowLinesCount; line ++)
	{
		char* src = (char*)(ItemSelectionDisplay_ItemsPtr + ItemSelectionDisplay_ItemSize * (ItemSelectionDisplay_BaseLine + line));
		char* dst = window[line];
		strcpy(dst, src);
	}

	ItemSelectionDisplay_DrawMenuLines(ItemSelectionDisplay_WindowLinesCount,
			window,
			ItemSelectionDisplay_ActiveLineIndex,
			ItemSelectionDisplay_SelectedItemIndex - ItemSelectionDisplay_BaseLine);

	/* Buttons */
	strcpy(LeftButton.Text, "OK");
	strcpy(RightButton.Text, "Cancel");
	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

void ItemSelectionDisplay_DrawMenuLines(uint8_t linesCount, char* lines, uint8_t activeLineIndex, int16_t selectedLineIndex)
{
	uint16_t rightmostPixel = FMGL_API_GetDisplayWidth(&fmglContext) - 1;
	uint16_t lineTop;

	FMGL_API_FontSettings font;
	for (uint8_t line = 0; line < linesCount; line ++)
	{
		char* lineText = lines + YHL_ITEM_SELECTION_DISPLAY_TEXT_MEMORY_SIZE * line;

		char buffer[YHL_ITEM_SELECTION_DISPLAY_TEXT_MEMORY_SIZE + 3];
		buffer[0] = '[';

		if (line == selectedLineIndex)
		{
			buffer[1] = 0x95;
		}
		else
		{
			buffer[1] = ' ';
		}


		buffer[2] = ']';
		strcpy(&buffer[3], lineText);

		lineTop = line * (YHL_ITEM_SELECTION_DISPLAY_LINE_HEIGHT + YHL_ITEM_SELECTION_DISPLAY_LINE_VERTICAL_SPACING) + YHL_ITEM_SELECTION_DISPLAY_LINES_TOP;

		if (line != activeLineIndex)
		{
			font = commonFont;
		}
		else
		{
			font = invertedCommonFont;
			FMGL_API_DrawRectangleFilled(&fmglContext, 0, lineTop, rightmostPixel, lineTop + YHL_ITEM_SELECTION_DISPLAY_LINE_HEIGHT, OnColor, OnColor);
		}

		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &font, 0, lineTop, NULL, NULL, false, buffer);
	}
}

void ItemSelectionDisplay_LeftClickHandler(void)
{
	/* Left click is OK*/
	ItemSelectionDisplay_Close();
	ItemSelectionDisplay_OnCloseHandler(ItemSelectionDisplay_SelectedItemIndex);
}


void ItemSelectionDisplay_EncoderClickHandler(void)
{
	ItemSelectionDisplay_SelectedItemIndex = ItemSelectionDisplay_ActiveLineIndex + ItemSelectionDisplay_BaseLine;
	ItemSelectionDisplay_Display();
}


void ItemSelectionDisplay_RightClickHandler(void)
{
	/* Right click is Cancel */
	ItemSelectionDisplay_Close();
	ItemSelectionDisplay_OnCloseHandler(ItemSelectionDisplay_InitiallySelectedIndex);
}

void ItemSelectionDisplay_EncoderRotationHandler(int8_t direction)
{
	int8_t activeLineIndex = ItemSelectionDisplay_ActiveLineIndex;

	if (HAL_ENCODER_ROTATION_CLOCKWISE == direction)
	{
		activeLineIndex ++;
	}
	else if (HAL_ENCODER_ROTATION_COUNTERCLOCKWISE == direction)
	{
		activeLineIndex --;
	}

	if (activeLineIndex < 0)
	{
		/* Scroll up */
		activeLineIndex = 0;

		ItemSelectionDisplay_ScrollUpHandler();
	}
	else if (activeLineIndex > ItemSelectionDisplay_WindowLinesCount - 1)
	{
		/* Scroll down */
		activeLineIndex = ItemSelectionDisplay_WindowLinesCount - 1;

		ItemSelectionDisplay_ScrollDownHandler();
	}

	ItemSelectionDisplay_ActiveLineIndex = activeLineIndex;

	ItemSelectionDisplay_Display();
}


void ItemSelectionDisplay_ScrollUpHandler(void)
{
	int16_t baseLineTmp = ItemSelectionDisplay_BaseLine;

	baseLineTmp --;

	if (baseLineTmp < 0)
	{
		baseLineTmp = 0;
	}

	ItemSelectionDisplay_BaseLine = baseLineTmp;
}


void ItemSelectionDisplay_ScrollDownHandler(void)
{
	if (ItemSelectionDisplay_ItemsCount <= YHL_ITEM_SELECTION_DISPLAY_MAX_LINES)
	{
		/* Scrolling disabled because of too few items */
		ItemSelectionDisplay_BaseLine = 0;
		return;
	}

	ItemSelectionDisplay_BaseLine ++;

	uint8_t offscreenItemsCount = ItemSelectionDisplay_ItemsCount - YHL_ITEM_SELECTION_DISPLAY_MAX_LINES;

	if (ItemSelectionDisplay_BaseLine > offscreenItemsCount)
	{
		ItemSelectionDisplay_BaseLine = offscreenItemsCount;
	}
}

void ItemSelectionDisplay_Close(void)
{
	FoxState.CurrentDisplay = ItemSelectionDisplay_PreviousDisplay;

	FMGL_API_ClearScreen(&fmglContext);
}
