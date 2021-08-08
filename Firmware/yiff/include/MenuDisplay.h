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
 * Menu line height
 */
#define YHL_MENU_LINE_HEIGHT 12

/**
 * Vertical spacing between menu lines
 */
#define YHL_MENU_LINE_VERTICAL_SPACING 2

/**
 * Menu leaf
 */
typedef struct
{
	/**
	 * Leaf name
	 */
	char Name[32];
}
MenuLeaf;

/**
 * Menu node (contains nodes and leafs)
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
	char Name[32];

	/**
	 * How many sub-nodes this node have
	 */
	uint8_t NodesCount;

	/**
	 * Pointer to nodes array
	 */
	void* Nodes;

	/**
	 * How many leafs this node have
	 */
	uint8_t LeafsCount;

	/**
	 * Pointer to leafs array
	 */
	MenuLeaf* Leafs;

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
void Menu_DrawMenuLines(uint8_t linesCount, char** lines, uint8_t activeLineIndex);

/**
 * Call it when right button is pressed
 */
void Menu_RightButtonHandler(void);

/**
 * Call it when encoder is rotated
 */
void Menu_EncoderRotationHandler(int8_t direction);

void Menu_RootNode_RightButtonAction(void);
void Menu_Node1_RightButtonAction(void);
void Menu_Node2_RightButtonAction(void);

#endif /* INCLUDE_MENUDISPLAY_H_ */
