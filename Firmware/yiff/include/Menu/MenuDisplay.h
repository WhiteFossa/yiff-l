/*
 * MenuDisplay.h
 *
 *  Created on: Aug 3, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENUDISPLAY_H_
#define INCLUDE_MENUDISPLAY_H_

#include <GraphicsBase.h>
#include <Buttons.h>
#include <Menu/ItemSelectionDisplay.h>
#include <Menu/InformationPopupDisplay.h>
#include <Menu/NumberInputDisplay.h>
#include <Menu/TimeInputDisplay.h>
#include <HAL.h>
#include <EEPROM.h>
#include <printf.h>
#include <GlobalStateMachine.h>
#include <main.h>
#include <math.h>
#include <Time.h>
#include <SelfDiagnostics.h>

/**
 * Maximal length of item's text
 */
#define YHL_MENU_MAX_ITEM_TEXT_LENGTH 32
#define YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE (YHL_MENU_MAX_ITEM_TEXT_LENGTH + 1)

/**
 * Maximal length of leaf button action text
 */
#define YHL_MENU_MAX_LEFT_BUTTON_TEXT_LENGTH 8
#define YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE (YHL_MENU_MAX_LEFT_BUTTON_TEXT_LENGTH + 1)

/**
 * Max nodes count in menu node
 */
#define YHL_MENU_MAX_NODES_COUNT 16

/**
 * Max leaves in menu node
 */
#define YHL_MENU_MAX_LEAVES_COUNT 16


/**
 * Menu line height
 */
#define YHL_MENU_LINE_HEIGHT 12

/**
 * Vertical spacing between menu lines
 */
#define YHL_MENU_LINE_VERTICAL_SPACING 2

/**
 * How much menu lines we are able to display
 */
#define YHL_MENU_NUMBER_OF_LINES 8

/**
 * Result is not a node
 */
#define YHL_MENU_NOT_A_NODE -1

/**
 * Result is not a leaf
 */
#define YHL_MENU_NOT_A_LEAF - 1

/**
 * Text for left button for menu nodes
 */
#define YHL_MENU_LEFT_BUTTON_TEXT_FOR_NODES "Enter"

/**
 * Menu leaf
 */
typedef struct
{
	/**
	 * Leaf name
	 */
	const char Name[YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE];

	/**
	 * Left button text for this node
	 */
	const char LeftButtonText[YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE];

	/**
	 * This function is called (if not null) when left button is pressed on the menu leaf
	 */
	void (*LeftButtonAction)(void);
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
	const char* NamePtr;

	/**
	 * How many sub-nodes this node have
	 */
	uint8_t NodesCount;

	/**
	 * Pointers to sub-nodes
	 */
	uintptr_t Nodes[YHL_MENU_MAX_NODES_COUNT];

	/**
	 * How many leaves this node have
	 */
	uint8_t LeavesCount;

	/**
	 * Array of pointers to leaves
	 */
	const MenuLeaf* LeavesPtrs[YHL_MENU_MAX_LEAVES_COUNT];
}
MenuNode;

extern FoxStateStruct FoxState;
extern FoxButtonStruct LeftButton;
extern FoxButtonStruct RightButton;


/**
 * Initialize menu display
 */
void MenuDisplay_InitMenuDisplay(void);

/**
 * Draws current menu display
 */
void MenuDisplay_DrawMenuDisplay(void);

/**
 * Call it when left button is pressed
 */
void MenuDisplay_LeftButtonHandler(void);

/**
 * Call it when right button is pressed
 */
void MenuDisplay_RightButtonHandler(void);

/**
 * Call it when encoder is pressed
 */
void MenuDisplay_EncoderClickHandler(void);

/**
 * Call it when encoder is rotated
 */
void MenuDisplay_EncoderRotationHandler(int8_t direction);

/**
 * Call this to exit menu display
 */
void MenuDisplay_ExitMenuDisplay(void);



#endif /* INCLUDE_MENUDISPLAY_H_ */
