/*
 * MenuDisplay.h
 *
 *  Created on: Aug 3, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENUDISPLAY_H_
#define INCLUDE_MENUDISPLAY_H_

#include <GraphicsBase.h>
#include <FoxStatus.h>
#include <Buttons.h>
#include <HAL.h>

/**
 * Maximal length of item's text
 */
#define YHL_MENU_MAX_ITEM_TEXT_LENGTH 31
#define YHK_MENU_MAX_ITEM_TEXT_MEMORY_SIZE (YHL_MENU_MAX_ITEM_TEXT_LENGTH + 1)

/**
 * Menu line height
 */
#define YHL_MENU_LINE_HEIGHT 12

/**
 * Vertical spacing between menu lines
 */
#define YHL_MENU_LINE_VERTICAL_SPACING 2

/**
 * How much menu lines we able to display
 */
#define YHL_MENU_NUMBER_OF_LINES 8

/**
 * Menu leaf
 */
typedef struct
{
	/**
	 * Leaf name
	 */
	char Name[YHK_MENU_MAX_ITEM_TEXT_MEMORY_SIZE];
}
MenuLeaf;

/**
 * Menu node (contains nodes and leaves)
 */
typedef struct
{
	/**
	 * Pointer to parent node
	 */
	void* Parent;

	/**
	 * Node name
	 */
	char Name[YHK_MENU_MAX_ITEM_TEXT_MEMORY_SIZE];

	/**
	 * How many sub-nodes this node have
	 */
	uint8_t NodesCount;

	/**
	 * Pointer to nodes array
	 */
	void* Nodes;

	/**
	 * How many leaves this node have
	 */
	uint8_t LeavesCount;

	/**
	 * Pointer to leaves array
	 */
	MenuLeaf* Leaves;

	/**
	 * Right button action
	 */
	void (*RightButtonAction)(void);

	/**
	 * Right button text
	 */
	char RightButtonText[YHL_MAX_BUTTON_TEXT_LENGTH + 1];
}
MenuNode;

extern FoxStateStruct FoxState;
extern FoxButtonStruct RightButton;


/**
 * Start node for the whole menu
 */
MenuNode Menu_RootNode;

/**
 * Current menu node
 */
MenuNode Menu_CurrentNode;

/**
 * Index of active line
 */
uint8_t ActiveLineIndex;

/**
 * How many menu lines current node have
 */
uint8_t CurrentNodeLinesCount;

/**
 * Current node lines, sub-nodes first, leaves then.
 * CurrentNodeLines + lineNumber * YHK_MENU_MAX_ITEM_TEXT_MEMORY_SIZE
 */
void* CurrentNodeLines;

/**
 * How many lines are currently displayed
 */
uint8_t WindowLinesCount;

/**
 * Line with this index (from CurrentNodeLines) will be displayed at the top
 */
uint8_t BaseLine;

/**
 * Initialize meny display
 */
void Menu_InitMenuDisplay(void);

/**
 * Draws current menu display
 */
void Menu_DrawMenuDisplay(void);

/**
 * Draw menu lines with selected line
 */
void Menu_DrawMenuLines(uint8_t linesCount, char* lines, uint8_t activeLineIndex);

/**
 * Call it when right button is pressed
 */
void Menu_RightButtonHandler(void);

/**
 * Call it when encoder is rotated
 */
void Menu_EncoderRotationHandler(int8_t direction);

/**
 * Called when menu need to be scrolled up
 */
void Menu_ScrollUpHandler(void);

/**
 * Called when menu need to be scrolled down
 */
void Menu_ScrollDownHandler(void);

void Menu_RootNode_RightButtonAction(void);
void Menu_Node1_RightButtonAction(void);
void Menu_Node2_RightButtonAction(void);

#endif /* INCLUDE_MENUDISPLAY_H_ */
