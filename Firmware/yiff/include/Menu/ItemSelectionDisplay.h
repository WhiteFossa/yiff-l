/*
 * ItemSelectionDisplay.h
 *
 *  Created on: Sep 12, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENU_ITEMSELECTIONDISPLAY_H_
#define INCLUDE_MENU_ITEMSELECTIONDISPLAY_H_

#include <GraphicsBase.h>
#include <Buttons.h>
#include <HAL.h>
#include <FoxStatus.h>

extern FoxStateStruct FoxState;

/**
 * Position of title top
 */
#define YHL_ITEM_SELECTION_DISPLAY_TITLE_TOP 0U

/**
 * Starting from this position we display items
 */
#define YHL_ITEM_SELECTION_DISPLAY_LINES_TOP 16U

/**
 * Line height
 */
#define YHL_ITEM_SELECTION_DISPLAY_LINE_HEIGHT 12U

/**
 * Vertical spacing between lines
 */
#define YHL_ITEM_SELECTION_DISPLAY_LINE_VERTICAL_SPACING 2U

/**
 * Maximum amount of lines
 */
#define YHL_ITEM_SELECTION_DISPLAY_MAX_LINES 6

#define YHL_ITEM_SELECTION_DISPLAY_TEXT_LENGTH 31
#define YHL_ITEM_SELECTION_DISPLAY_TEXT_MEMORY_SIZE (YHL_ITEM_SELECTION_DISPLAY_TEXT_LENGTH + 1)


/**
 * Display for selecting a single item
 */

/**
 * Call this to display item selection menu
 */
void ItemSelectionDisplay_Show(char* title,
		char* itemsPtr,
		uint8_t itemSize,
		uint8_t itemsCount,
		uint8_t selectedItemIndex,
		void (*onCloseHandler)(uint8_t newSelectedItemIndex),
		FoxDisplayEnum previousDisplay);


/**
 * Call it on left button click
 */
void ItemSelectionDisplay_LeftClickHandler(void);

/**
 * Call it on encoder click
 */
void ItemSelectionDisplay_EncoderClickHandler(void);

/**
 * Call it on right button click
 */
void ItemSelectionDisplay_RightClickHandler(void);

/**
 * Call it on encoder rotation
 */
void ItemSelectionDisplay_EncoderRotationHandler(int8_t direction);


#endif /* INCLUDE_MENU_ITEMSELECTIONDISPLAY_H_ */
