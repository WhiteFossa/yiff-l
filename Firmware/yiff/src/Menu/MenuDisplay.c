/*
 * MenuDisplay.c
 *
 *  Created on: Aug 3, 2021
 *      Author: fossa
 */

#include <Menu/MenuDisplay.h>
#include <Menu/MenuDisplayPrivate.h>

void MenuDisplay_InitMenuDisplay(void)
{
	MenuDisplay_RootNode.Parent = NULL;
	strncpy(MenuDisplay_RootNode.Name, "Fox settings", YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);

	/* Root menu nodes */
	MenuDisplay_RootNode.NodesCount = 2;
	MenuDisplay_RootNode.Nodes = malloc(sizeof(MenuNode) * MenuDisplay_RootNode.NodesCount);

	/* Profile settings */
	MenuNode* profileSettingsNode = &((MenuNode*)MenuDisplay_RootNode.Nodes)[0];
	profileSettingsNode->Parent = &MenuDisplay_RootNode;
	strncpy(profileSettingsNode->Name, "Profile settings", YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	profileSettingsNode->LeavesCount = 2;
	profileSettingsNode->Leaves = malloc(sizeof(MenuLeaf) * profileSettingsNode->LeavesCount);

	/* Profile settings -> Show current profile */
	strncpy(profileSettingsNode->Leaves[0].Name, "Show current profile", YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	strncpy(profileSettingsNode->Leaves[0].LeftButtonText, "Show", YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE);
	profileSettingsNode->Leaves[0].LeftButtonAction = &MenuDisplay_ShowCurrentProfileInformationPopup;

	/* Profile settings -> Select profile*/
	strncpy(profileSettingsNode->Leaves[1].Name, "Select profile", YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	strncpy(profileSettingsNode->Leaves[1].LeftButtonText, "Select", YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE);
	profileSettingsNode->Leaves[1].LeftButtonAction = &MenuDisplay_SelectCurrentProfile;

	profileSettingsNode->NodesCount = 0;
	profileSettingsNode->Nodes = NULL;


	/* Edit current profile */
	MenuNode* editCurrentProfileNode = &((MenuNode*)MenuDisplay_RootNode.Nodes)[1];
	editCurrentProfileNode->Parent = &MenuDisplay_RootNode;
	strncpy(editCurrentProfileNode->Name, "Edit current profile", YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	editCurrentProfileNode->LeavesCount = 0;
	editCurrentProfileNode->Leaves = malloc(sizeof(MenuLeaf) * profileSettingsNode->LeavesCount);

	/* Edit current profile -> Frequency */
	editCurrentProfileNode->NodesCount = 1;
	editCurrentProfileNode->Nodes = malloc(sizeof(MenuNode) * editCurrentProfileNode->NodesCount);

	MenuNode* frequencySettingsNode = &((MenuNode*)editCurrentProfileNode->Nodes)[0];
	frequencySettingsNode->Parent = editCurrentProfileNode;
	strncpy(frequencySettingsNode->Name, "Frequency", YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	frequencySettingsNode->LeavesCount = 2;
	frequencySettingsNode->Leaves = malloc(sizeof(MenuLeaf) * frequencySettingsNode->LeavesCount);

	/* Edit current profile -> Frequency -> Range */
	strncpy(frequencySettingsNode->Leaves[0].Name, "Range", YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	strncpy(frequencySettingsNode->Leaves[0].LeftButtonText, "Set", YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE);
	frequencySettingsNode->Leaves[0].LeftButtonAction = &MenuDisplay_SelectFrequencyRange;

	/* Edit current profile -> Frequency -> Value */
	strncpy(frequencySettingsNode->Leaves[1].Name, "Value", YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	strncpy(frequencySettingsNode->Leaves[1].LeftButtonText, "Set", YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE);
	frequencySettingsNode->Leaves[1].LeftButtonAction = &MenuDisplay_EnterFrequency;

	frequencySettingsNode->NodesCount = 0;
	frequencySettingsNode->Nodes = NULL;



	/* Root menu leaves */
	MenuDisplay_RootNode.LeavesCount = 0;
	MenuDisplay_RootNode.Leaves = malloc(sizeof(MenuLeaf) * MenuDisplay_RootNode.LeavesCount);

	MenuDisplay_ActiveLineIndex = 0;
	MenuDisplay_CurrentNodeLines = NULL;
	MenuDisplay_ProfilesNames = NULL;

	MenuDisplay_SwitchNode(&MenuDisplay_RootNode);
}


void MenuDisplay_SwitchNode(MenuNode* nodePtr)
{
	MenuDisplay_CurrentNode = *nodePtr;

	MenuDisplay_CurrentNodeLinesCount = MenuDisplay_CurrentNode.NodesCount + MenuDisplay_CurrentNode.LeavesCount;
	if (MenuDisplay_CurrentNodeLines != NULL)
	{
		free(MenuDisplay_CurrentNodeLines);
	}

	MenuDisplay_CurrentNodeLines = malloc(MenuDisplay_CurrentNodeLinesCount * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);

	/* Nodes first */
	for (uint8_t nodesCounter = 0; nodesCounter < MenuDisplay_CurrentNode.NodesCount; nodesCounter ++)
	{
		char* dst = (char*)(MenuDisplay_CurrentNodeLines + nodesCounter * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
		MenuNode node = ((MenuNode*)MenuDisplay_CurrentNode.Nodes)[nodesCounter];
		char* src = node.Name;
		strncpy(dst, src, YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	}

	/* Leaves then */
	for (uint8_t leavesCounter = 0; leavesCounter < MenuDisplay_CurrentNode.LeavesCount; leavesCounter ++)
	{
		uint8_t baseCount = MenuDisplay_CurrentNode.NodesCount;
		char* dst = (char*)(MenuDisplay_CurrentNodeLines + (baseCount + leavesCounter) * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
		char* src = MenuDisplay_CurrentNode.Leaves[leavesCounter].Name;
		strncpy(dst, src, YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	}

	/* How many lines could we display */
	if (MenuDisplay_CurrentNodeLinesCount > YHL_MENU_NUMBER_OF_LINES)
	{
		MenuDisplay_WindowLinesCount = YHL_MENU_NUMBER_OF_LINES;
	}
	else
	{
		MenuDisplay_WindowLinesCount = MenuDisplay_CurrentNodeLinesCount;
	}

	MenuDisplay_BaseLine = 0;
	MenuDisplay_ActiveLineIndex = 0;
}

void MenuDisplay_GoToParentNode(void)
{
	MenuNode* parent = MenuDisplay_CurrentNode.Parent;
	if (NULL == parent)
	{
		/* Exiting menu */
		FoxState.CurrentDisplay = StatusDisplay;
		strncpy(LeftButton.Text, "Menu", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);
		strncpy(RightButton.Text, "Bt. off", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);

		FMGL_API_ClearScreen(&fmglContext);
		return;
	}

	MenuDisplay_SwitchNode(parent);
	MenuDisplay_DrawMenuDisplay();
}

void MenuDisplay_DrawMenuDisplay(void)
{
	if (FoxState.CurrentDisplay != MenuDisplay)
	{
		L2HAL_Error(Generic);
	}

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	/* Display window - this lines will be displayed*/
	char window[YHL_MENU_NUMBER_OF_LINES][YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE];

	for (uint8_t linesCounter = 0; linesCounter < MenuDisplay_WindowLinesCount; linesCounter ++)
	{
		char* src = (char*)(MenuDisplay_CurrentNodeLines + (MenuDisplay_BaseLine + linesCounter) * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
		char* dst = window[linesCounter];
		strncpy(dst, src, YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	}

	MenuDisplay_DrawMenuLines(MenuDisplay_WindowLinesCount, (char*)window, MenuDisplay_ActiveLineIndex);

	/* Buttons texts */
	int16_t leafIndex = MenuDisplay_GetCurrentLeafIndex(MenuDisplay_GetCurrentNodeActiveLineIndex());
	if (YHL_MENU_NOT_A_LEAF == leafIndex)
	{
		/* It's a node, left button text is the same for all nodes */
		strncpy(LeftButton.Text, YHL_MENU_LEFT_BUTTON_TEXT_FOR_NODES, YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE);
	}
	else
	{
		strncpy(LeftButton.Text, MenuDisplay_CurrentNode.Leaves[leafIndex].LeftButtonText, YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE);
	}

	strncpy(RightButton.Text, "Back", YHL_MAX_BUTTON_TEXT_MEMORY_SIZE);

	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

uint8_t MenuDisplay_GetCurrentNodeActiveLineIndex(void)
{
	return MenuDisplay_BaseLine + MenuDisplay_ActiveLineIndex;
}

int16_t MenuDisplay_GetCurrentSubNodeIndex(uint8_t currentNodeActiveLineIndex)
{
	if (currentNodeActiveLineIndex > MenuDisplay_CurrentNode.NodesCount - 1)
	{
		return YHL_MENU_NOT_A_NODE;
	}

	return currentNodeActiveLineIndex;
}

int16_t MenuDisplay_GetCurrentLeafIndex(uint8_t currentNodeActiveLineIndex)
{
	int subNodeIndex = MenuDisplay_GetCurrentSubNodeIndex(currentNodeActiveLineIndex);

	if (subNodeIndex != YHL_MENU_NOT_A_NODE)
	{
		/* It's a node, not a leaf */
		return YHL_MENU_NOT_A_LEAF;
	}

	return currentNodeActiveLineIndex - MenuDisplay_CurrentNode.NodesCount;
}

void MenuDisplay_LeftButtonHandler(void)
{
	MenuDisplay_OnAction(MenuLeftButtonClick);
}

void MenuDisplay_OnAction(MenuActionEnum action)
{
	uint8_t currentNodeActiveLineIndex = MenuDisplay_GetCurrentNodeActiveLineIndex();

	int16_t subNodeIndex = MenuDisplay_GetCurrentSubNodeIndex(currentNodeActiveLineIndex);

	if (subNodeIndex != YHL_MENU_NOT_A_NODE)
	{
		MenuNode* subNode = &((MenuNode*)MenuDisplay_CurrentNode.Nodes)[subNodeIndex];
		MenuDisplay_ActionOnNodeHandler(subNode, action);
		return;
	}

	int16_t leafIndex = MenuDisplay_GetCurrentLeafIndex(currentNodeActiveLineIndex);
	if (YHL_MENU_NOT_A_LEAF == leafIndex)
	{
		/* Not a node, not a leaf, definitely a bug */
		L2HAL_Error(Generic);
	}

	MenuLeaf* leafPtr = &MenuDisplay_CurrentNode.Leaves[leafIndex];
	MenuDisplay_ActionOnLeafHandler(leafPtr, action);
}

void MenuDisplay_ActionOnNodeHandler(MenuNode* node, MenuActionEnum action)
{
	if (MenuLeftButtonClick == action || MenuEncoderClick == action)
	{
		/* Entering menu node */
		MenuDisplay_SwitchNode(node);
		MenuDisplay_DrawMenuDisplay();
		return;
	}

	if (MenuRightButtonClick == action)
	{
		/* Going back */
		MenuDisplay_GoToParentNode();
	}
}

void MenuDisplay_ActionOnLeafHandler(MenuLeaf* leaf, MenuActionEnum action)
{
	if (MenuLeftButtonClick == action || MenuEncoderClick == action)
	{
		if (leaf->LeftButtonAction != NULL)
		{
			leaf->LeftButtonAction();
		}
		return;
	}

	if (MenuRightButtonClick == action)
	{
		/* Going back */
		MenuDisplay_GoToParentNode();
	}
}

void MenuDisplay_RightButtonHandler(void)
{
	MenuDisplay_OnAction(MenuRightButtonClick);
}

void MenuDisplay_EncoderClickHandler(void)
{
	MenuDisplay_OnAction(MenuEncoderClick);
}

void MenuDisplay_EncoderRotationHandler(int8_t direction)
{
	int16_t activeLineIndex = MenuDisplay_ActiveLineIndex;

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

		MenuDisplay_ScrollUpHandler();
	}
	else if (activeLineIndex > MenuDisplay_WindowLinesCount - 1)
	{
		/* Scroll down */
		activeLineIndex = MenuDisplay_WindowLinesCount - 1;

		MenuDisplay_ScrollDownHandler();
	}

	MenuDisplay_ActiveLineIndex = (uint8_t)activeLineIndex;

	MenuDisplay_DrawMenuDisplay();
}

void MenuDisplay_DrawMenuLines(uint8_t linesCount, char* lines, uint8_t activeLineIndex)
{
	uint16_t rightmostPixel = FMGL_API_GetDisplayWidth(&fmglContext) - 1;
	uint16_t lineTop;

	FMGL_API_FontSettings font;
	for (uint8_t line = 0; line < linesCount; line ++)
	{
		lineTop = line * (YHL_MENU_LINE_HEIGHT + YHL_MENU_LINE_VERTICAL_SPACING);

		if (line != activeLineIndex)
		{
			font = commonFont;
		}
		else
		{
			font = invertedCommonFont;
			FMGL_API_DrawRectangleFilled(&fmglContext, 0, lineTop, rightmostPixel, lineTop + YHL_MENU_LINE_HEIGHT, OnColor, OnColor);
		}

		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &font, 0, lineTop, NULL, NULL, false, lines + YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE * line);
	}
}

void MenuDisplay_ScrollUpHandler(void)
{
	int16_t baseLineTmp = MenuDisplay_BaseLine;

	baseLineTmp --;

	if (baseLineTmp < 0)
	{
		baseLineTmp = 0;
	}

	MenuDisplay_BaseLine = (uint8_t)baseLineTmp;
}


void MenuDisplay_ScrollDownHandler(void)
{
	if (MenuDisplay_CurrentNodeLinesCount <= YHL_MENU_NUMBER_OF_LINES)
	{
		/* Scrolling disabled because of too few items */
		MenuDisplay_BaseLine = 0;
		return;
	}

	MenuDisplay_BaseLine ++;

	uint8_t offscreenItemsCount = MenuDisplay_CurrentNodeLinesCount - YHL_MENU_NUMBER_OF_LINES;

	if (MenuDisplay_BaseLine > offscreenItemsCount)
	{
		MenuDisplay_BaseLine = offscreenItemsCount;
	}
}

void MenuDisplay_ShowCurrentProfileInformationPopup(void)
{
	InformationPopup_Show("Current profile:", EEPROM_GetProfile(EEPROM_Header.ProfileInUse).Name, MenuDisplay);
}

void MenuDisplay_SelectCurrentProfile(void)
{
	MenuDisplay_ProfilesNames = malloc(YHL_PROFILE_NAME_MEMORY_SIZE * EEPROM_Header.NumberOfProfiles);
	for (uint8_t profile = 0; profile < EEPROM_Header.NumberOfProfiles; profile ++)
	{
		char* src = EEPROM_GetProfile(profile).Name;
		char* dst = MenuDisplay_ProfilesNames + YHL_PROFILE_NAME_MEMORY_SIZE * profile;
		strncpy(dst, src, YHL_PROFILE_NAME_MEMORY_SIZE);
	}

	ItemSelectionDisplay_Show("Select profile",
			MenuDisplay_ProfilesNames,
			YHL_PROFILE_NAME_MEMORY_SIZE,
			EEPROM_Header.NumberOfProfiles,
			EEPROM_Header.ProfileInUse,
			&MenuDisplay_SelectCurrentProfileCloseHandler,
			MenuDisplay);
}

void MenuDisplay_SelectCurrentProfileCloseHandler(uint8_t profileIndex)
{
	free(MenuDisplay_ProfilesNames);
	MenuDisplay_ProfilesNames = NULL;

	if (profileIndex != EEPROM_Header.ProfileInUse)
	{
		EEPROM_SwitchProfile(profileIndex);
	}

	MenuDisplay_DrawMenuDisplay();
}

void MenuDisplay_SelectFrequencyRange(void)
{
	uint8_t rangesCount = 2;
	MenuDisplay_FrequencyRangesNames = malloc(rangesCount * YHL_PROFILE_NAME_MEMORY_SIZE);
	strncpy(MenuDisplay_FrequencyRangesNames, "3.5MHz", YHL_PROFILE_NAME_MEMORY_SIZE);
	strncpy(MenuDisplay_FrequencyRangesNames + YHL_PROFILE_NAME_MEMORY_SIZE, "144MHz", YHL_PROFILE_NAME_MEMORY_SIZE);

	uint8_t activeRangeIndex = MenuDisplay_GetFrequencyRangeIndex(EEPROM_CurrentProfile.Frequency.Is144MHz);

	ItemSelectionDisplay_Show("Select range",
			MenuDisplay_FrequencyRangesNames,
			YHL_PROFILE_NAME_MEMORY_SIZE,
			rangesCount,
			activeRangeIndex,
			&MenuDisplay_SelectFrequencyRangeCloseHandler,
			MenuDisplay);
}

void MenuDisplay_SelectFrequencyRangeCloseHandler(uint8_t rangeIndex)
{
	free(MenuDisplay_FrequencyRangesNames);
	MenuDisplay_FrequencyRangesNames = NULL;

	uint8_t currentRangeIndex = MenuDisplay_GetFrequencyRangeIndex(FoxState.Frequency.Is144MHz);
	if (currentRangeIndex == rangeIndex)
	{
		/* Frequency range didn't change */
		MenuDisplay_DrawMenuDisplay();
		return;
	}

	bool is144MHz = MenuDisplay_GetFrequencyRangeByIndex(rangeIndex);

	uint newFrequency;
	if (is144MHz)
	{
		newFrequency = YHL_DEFAULT_144M_FREQUENCY;
	}
	else
	{
		newFrequency = YHL_DEFAULT_80M_FREQUENCY;
	}

	FoxState_SetFrequency(is144MHz, newFrequency);
	PendingCommandsFlags.NeedToSetFrequency = true;

	uint8_t popupBufferSize = 16;
	char popupBuffer[popupBufferSize];

	float freqMHz = (float)newFrequency / 1000000.0f;

	snprintf(popupBuffer, popupBufferSize, "%.3f MHz", freqMHz);

	InformationPopup_Show("Frequency updated:", popupBuffer, MenuDisplay);
}

uint8_t MenuDisplay_GetFrequencyRangeIndex(bool is144MHz)
{
	if (is144MHz)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool MenuDisplay_GetFrequencyRangeByIndex(uint8_t rangeIndex)
{
	switch(rangeIndex)
	{
		case 0:
			return false;
		case 1:
			return true;
		default:
			L2HAL_Error(Generic);
			return false; /* Never will be reached, to suppress warning */
	}
}

void MenuDisplay_EnterFrequency(void)
{
	if (FoxState.Frequency.Is144MHz)
	{
		NumberInputDisplay_Show("Select frequency",
				YHL_MIN_2M_FREQUENCY,
				YHL_MAX_2M_FREQUENCY,
				(int32_t)FoxState.Frequency.FrequencyHz,
				50000,
				&MenuDisplay_FormatFrequency,
				&MenuDisplay_EnterFrequencyOnEnterHandler,
				MenuDisplay);
	}
	else
	{
		NumberInputDisplay_Show("Select frequency",
				YHL_MIN_80M_FREQUENCY,
				YHL_MAX_80M_FREQUENCY,
				(int32_t)FoxState.Frequency.FrequencyHz,
				5000,
				&MenuDisplay_FormatFrequency,
				&MenuDisplay_EnterFrequencyOnEnterHandler,
				MenuDisplay);
	}
}

char* MenuDisplay_FormatFrequency(int32_t frequencyHz)
{
	char* buffer = malloc(32);
	snprintf(buffer, 32, "%.3f MHz", frequencyHz / 1000000.0f);

	return buffer;
}

void MenuDisplay_EnterFrequencyOnEnterHandler(int32_t frequency)
{
	FoxState_SetFrequency(FoxState.Frequency.Is144MHz, frequency);
	PendingCommandsFlags.NeedToSetFrequency = true;

	MenuDisplay_DrawMenuDisplay();
}
