/*
 * MenuDisplayPrivate.h
 *
 *  Created on: Sep 18, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MENU_MENUDISPLAYPRIVATE_H_
#define INCLUDE_MENU_MENUDISPLAYPRIVATE_H_

/**
 * Max length + 1 for frequency ranges names
 */
#define YHL_MENU_FREQUENCY_RANGE_MEMORY_SIZE 17U

/**
 * Max length + 1 for fox codes names
 */
#define YHL_MENU_FOX_CODES_MEMORY_SIZE 17U

/**
 * Max length + 1 for fox speeds names
 */
#define YHL_MENU_FOX_SPEEDS_MEMORY_SIZE 17U

/**
 * Max length + 1 for cycle types names
 */
#define YHL_MENU_CYCLE_TYPE_MEMORY_SIZE 17U

/**
 * Maximal ending tone duration in seconds
 */
#define YHL_MENU_MAX_ENDINGTONE_DURATION 10U

/**
 * Maximal transmission duration
 */
#define YHL_MENU_MAX_TX_DURATION 3600U

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
 * Names of fox codes
 */
char* MenuDisplay_FoxCodesNames;

/**
 * Names of fox speeds
 */
char* MenuDisplay_FoxSpeedsNames;

/**
 * Names of cycle types
 */
char* MenuDisplay_CycleTypesNames;

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

/**
 * Call this to select fox code
 */
void MenuDisplay_SelectCode(void);

/**
 * Call this when fox code is selected
 */
void MenyDisplay_SelectCodeCloseHandler(uint8_t codeIndex);

/**
 * Call this to select fox speed
 */
void MenuDisplay_SelectFoxSpeed(void);

/**
 * Call this when fox speed is selected
 */
void MenyDisplay_SelectFoxSpeedCloseHandler(uint8_t speedIndex);

/**
 * Converts fox speed to selected item index
 */
uint8_t MenuDisplay_FoxSpeedToIndex(bool isFast);

/**
 * Converts selected item index to fox speed.
 */
bool MenuDisplay_IndexToFoxSpeed(uint8_t index);

/**
 * Call this to select cycle type
 */
void MenuDisplay_SelectIsContinuousCycle(void);

/**
 * Call this when cycle type is selected
 */
void MenuDisplay_SelectIsContinuousCycleHandler(uint8_t cycleTypeIndex);

/**
 * Converts fox cycle type into item index
 */
uint8_t MenuDisplay_CycleTypeToIndex(bool isContinuous);

/**
 * Converts item index to fox cycle type
 */
bool MenuDisplay_IndexToCycleType(uint8_t index);

/**
 * Call this to enter ending tone duration
 */
void MenuDisplay_EnterEndingToneDuration(void);

/**
 * Call this when ending tone duration is entered
 */
void MenuDisplay_EnterEndingToneDurationOnEnterHandler(int32_t duration);

/**
 * Used to format ending tone duration in ending tone duration input display.
 * Do not forget to free result!
 */
char* MenuDisplay_FormatEndingToneDuration(int32_t duration);

/**
 * Call this to show "cycle is continuous" warning
 */
void MenuDisplay_ShowCycleIsContinuousWarning(void);

/**
 * Call this to enter Tx duration
 */
void MenuDisplay_EnterTxDuration(void);

/**
 * Call this when Tx duration is entered
 */
void MenuDisplay_EnterTxDurationEnterHandler(uint32_t duration);


#endif /* INCLUDE_MENU_MENUDISPLAYPRIVATE_H_ */
