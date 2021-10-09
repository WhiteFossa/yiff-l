/*
 * ItemSelectionDisplayPrivate.h
 *
 *  Created on: Sep 12, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENU_ITEMSELECTIONDISPLAYPRIVATE_H_
#define INCLUDE_MENU_ITEMSELECTIONDISPLAYPRIVATE_H_

#include <stdint.h>

/**
 * Pointer to function, which will be called when display is closed
 */
void (*ItemSelectionDisplay_OnCloseHandler)(uint8_t newSelectedItemIndex);

/**
* Return to this display when item is selected
*/
FoxDisplayEnum ItemSelectionDisplay_PreviousDisplay;

/**
 * Initially selected index (for cancels)
 */
uint8_t ItemSelectionDisplay_InitiallySelectedIndex;

/**
 * Selected item (not in a window, but globally) index
 */
uint8_t ItemSelectionDisplay_SelectedItemIndex;

/**
 * How many items we have to display
 */
uint8_t ItemSelectionDisplay_ItemsCount;

/**
 * Pointer to array with items
 */
char* ItemSelectionDisplay_ItemsPtr;

/**
 * Size of one item in ItemSelectionDisplay_ItemsPtr
 */
uint8_t ItemSelectionDisplay_ItemSize;

/**
 * Selected item (in a window) index
 */
uint8_t ItemSelectionDisplay_ActiveLineIndex;

/**
 * Title is stored here
 */
char* ItemSelectionDisplay_Title;

/**
 * How much lines we have on display currently
 */
uint8_t ItemSelectionDisplay_WindowLinesCount;

/**
 * Item with this index is displayed at the top of window
 */
uint8_t ItemSelectionDisplay_BaseLine;

/**
 * Draw current item selection display state
 */
void ItemSelectionDisplay_Display(void);

/**
 * Closes item selection display.
 */
void ItemSelectionDisplay_Close(void);

/**
 * Draw menu lines with selected line. Negative selectedLineIndex means that no visible lines selected
 */
void ItemSelectionDisplay_DrawMenuLines(uint8_t linesCount, char* lines, uint8_t activeLineIndex, int16_t selectedLineIndex);

/**
 * Called when items need to be scrolled up
 */
void ItemSelectionDisplay_ScrollUpHandler(void);

/**
 * Called when menu need to be scrolled down
 */
void ItemSelectionDisplay_ScrollDownHandler(void);

#endif /* INCLUDE_MENU_ITEMSELECTIONDISPLAYPRIVATE_H_ */
