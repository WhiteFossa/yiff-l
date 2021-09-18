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
//#include <Menu/InformationPopupDisplay.h>
#include <Menu/ItemSelectionDisplay.h>
#include <HAL.h>
#include <EEPROM.h>

/**
 * Maximal length of item's text
 */
#define YHL_MENU_MAX_ITEM_TEXT_LENGTH 31
#define YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE (YHL_MENU_MAX_ITEM_TEXT_LENGTH + 1)

/**
 * Maximal length of leaf button action text
 */
#define YHL_MENU_MAX_LEFT_BUTTON_TEXT_LENGTH 8
#define YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE (YHL_MENU_MAX_LEFT_BUTTON_TEXT_LENGTH + 1)

/**
 * What happened on menu item
 */
typedef enum
{
	MenuLeftButtonClick,

	MenuEncoderClick,

	MenuRightButtonClick
}
MenuActionEnum;

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
	char Name[YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE];

	/**
	 * Left button text for this node
	 */
	char LeftButtonText[YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE];

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
	char Name[YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE];

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
}
MenuNode;

extern FoxStateStruct FoxState;
extern FoxButtonStruct LeftButton;
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
 * Names of fox profiles
 */
char* MenuDisplay_ProfilesNames;

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
 * Call it when left button is pressed
 */
void Menu_LeftButtonHandler(void);

/**
 * Call it when right button is pressed
 */
void Menu_RightButtonHandler(void);

/**
 * Call it when encoder is pressed
 */
void Menu_EncoderClickHandler(void);

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

/**
 * Gets active line index (globally active, not in display window)
 */
uint8_t Menu_GetCurrentNodeActiveLineIndex(void);

/**
 * Gets current node active sub-node index or returns YHL_MENU_NOT_A_NODE if active line points to a leaf
 */
int8_t Menu_GetCurrentSubNodeIndex(uint8_t currentNodeActiveLineIndex);

/**
 * Gets current node active list index or returns YHL_MENU_NOT_A_LEAF if active line points to a sub-node
 */
int8_t Menu_GetCurrentLeafIndex(uint8_t currentNodeActiveLineIndex);

/**
 * Call it on action (e.g. button press)
 */
void Menu_OnAction(MenuActionEnum action);

/**
 * Called when something is happened on menu node
 */
void Menu_ActionOnNodeHandler(MenuNode* node, MenuActionEnum action);

/**
 * Called when something is happened on menu leaf
 */
void Menu_ActionOnLeafHandler(MenuLeaf* leaf, MenuActionEnum action);

/**
 * Make given node current
 */
void Menu_SwitchNode(MenuNode* nodePtr);

/**
 * Go to parent of current node
 */
void Menu_GoToParentNode(void);

/**
 * Show information popup about current profile
 */
void Menu_ShowCurrentProfileInformationPopup(void);

/**
 * Call this to select current profile
 */
void Menu_SelectCurrentProfile(void);

/**
 * Call it when current profile is selected
 */
void Menu_SelectCurrentProfileCloseHandler(uint8_t profileIndex);

#endif /* INCLUDE_MENUDISPLAY_H_ */
