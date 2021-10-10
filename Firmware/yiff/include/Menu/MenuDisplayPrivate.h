/*
 * MenuDisplayPrivate.h
 *
 *  Created on: Sep 18, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENU_MENUDISPLAYPRIVATE_H_
#define INCLUDE_MENU_MENUDISPLAYPRIVATE_H_

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
 * Start node for the whole menu
 */
MenuNode MenuDisplay_RootNode;

/**
 * Current menu node
 */
MenuNode MenuDisplay_CurrentNode;

/**
 * Index of active line
 */
uint8_t MenuDisplay_ActiveLineIndex;

/**
 * How many menu lines current node have
 */
uint8_t MenuDisplay_CurrentNodeLinesCount;

/**
 * Current node lines, sub-nodes first, leaves then.
 * MenuDisplay_CurrentNodeLines + lineNumber * YHK_MENU_MAX_ITEM_TEXT_MEMORY_SIZE
 */
void* MenuDisplay_CurrentNodeLines;

/**
 * How many lines are currently displayed
 */
uint8_t MenuDisplay_WindowLinesCount;

/**
 * Line with this index (from MenuDisplay_CurrentNodeLines) will be displayed at the top
 */
uint8_t MenuDisplay_BaseLine;

/**
 * Names of fox profiles
 */
char* MenuDisplay_ProfilesNames;

/**
 * Names of frequency ranges
 */
char* MenuDisplay_FrequencyRangesNames;

/**
 * Draw menu lines with selected line
 */
void MenuDisplay_DrawMenuLines(uint8_t linesCount, char* lines, uint8_t activeLineIndex);

/**
 * Called when menu need to be scrolled up
 */
void MenuDisplay_ScrollUpHandler(void);

/**
 * Called when menu need to be scrolled down
 */
void MenuDisplay_ScrollDownHandler(void);

/**
 * Gets active line index (globally active, not in display window)
 */
uint8_t MenuDisplay_GetCurrentNodeActiveLineIndex(void);

/**
 * Gets current node active sub-node index or returns YHL_MENU_NOT_A_NODE if active line points to a leaf
 */
int16_t MenuDisplay_GetCurrentSubNodeIndex(uint8_t currentNodeActiveLineIndex);

/**
 * Gets current node active list index or returns YHL_MENU_NOT_A_LEAF if active line points to a sub-node
 */
int16_t MenuDisplay_GetCurrentLeafIndex(uint8_t currentNodeActiveLineIndex);

/**
 * Call it on action (e.g. button press)
 */
void MenuDisplay_OnAction(MenuActionEnum action);

/**
 * Called when something is happened on menu node
 */
void MenuDisplay_ActionOnNodeHandler(MenuNode* node, MenuActionEnum action);

/**
 * Called when something is happened on menu leaf
 */
void MenuDisplay_ActionOnLeafHandler(MenuLeaf* leaf, MenuActionEnum action);

/**
 * Make given node current
 */
void MenuDisplay_SwitchNode(MenuNode* nodePtr);

/**
 * Go to parent of current node
 */
void MenuDisplay_GoToParentNode(void);

/**
 * Show information popup about current profile
 */
void MenuDisplay_ShowCurrentProfileInformationPopup(void);

/**
 * Call this to select current profile
 */
void MenuDisplay_SelectCurrentProfile(void);

/**
 * Call it when current profile is selected
 */
void MenuDisplay_SelectCurrentProfileCloseHandler(uint8_t profileIndex);

/**
 * Call this to select frequency range
 */
void MenuDisplay_SelectFrequencyRange(void);

/**
 * Call this when frequency range is selected
 */
void MenuDisplay_SelectFrequencyRangeCloseHandler(uint8_t rangeIndex);

/**
 * Returns frequency range index depending on range type
 */
uint8_t MenuDisplay_GetFrequencyRangeIndex(bool is144MHz);

/**
 * Returns "Is 144 MHz" flag by range index
 */
bool MenuDisplay_GetFrequencyRangeByIndex(uint8_t rangeIndex);

/**
 * Call this to enter frequency
 */
void MenuDisplay_EnterFrequency(void);

/**
 * Call this when frequency is entered
 */
void MenuDisplay_EnterFrequencyOnEnterHandler(int32_t frequency);

/**
 * Used to format frequency in frequency input display.
 * Do not forget to free result!
 */
char* MenuDisplay_FormatFrequency(int32_t frequencyHz);


#endif /* INCLUDE_MENU_MENUDISPLAYPRIVATE_H_ */
