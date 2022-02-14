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
	MenuDisplay_RootNode.NamePtr = MenuDisplay_RootNodeName;

	/* Root menu nodes */
	MenuDisplay_RootNode.NodesCount = 4;

	/* Clock */
	MenuDisplay_RootNode.Nodes[0] = &MenuDisplay_ClockNode;
	MenuDisplay_ClockNode.Parent = &MenuDisplay_RootNode;
	MenuDisplay_ClockNode.NamePtr = MenuDisplay_ClockNodeName;
	MenuDisplay_ClockNode.LeavesCount = 1;
	MenuDisplay_ClockNode.LeavesPtrs[0] = &MenuDisplay_SetCurrentTimeLeaf; /* Clock -> Set current time */
	MenuDisplay_ClockNode.NodesCount = 0;

	/* Profile settings */
	MenuDisplay_RootNode.Nodes[1] = &MenuDisplay_ProfileSettingsNode;
	MenuDisplay_ProfileSettingsNode.Parent = &MenuDisplay_RootNode;
	MenuDisplay_ProfileSettingsNode.NamePtr = MenuDisplay_ProfileSettingsNodeName;
	MenuDisplay_ProfileSettingsNode.LeavesCount = 2;
	MenuDisplay_ProfileSettingsNode.LeavesPtrs[0] = &MenuDisplay_ShowCurrentProfileLeaf; /* Profile settings -> Show current profile */
	MenuDisplay_ProfileSettingsNode.LeavesPtrs[1] = &MenuDisplay_SelectProfileLeaf; /* Profile settings -> Select profile*/
	MenuDisplay_ProfileSettingsNode.NodesCount = 0;

	/* Edit current profile */
	MenuDisplay_RootNode.Nodes[2] = &MenuDisplay_EditCurrentProfileNode;
	MenuDisplay_EditCurrentProfileNode.Parent = &MenuDisplay_RootNode;
	MenuDisplay_EditCurrentProfileNode.NamePtr = MenuDisplay_EditCurrentProfileNodeName;
	MenuDisplay_EditCurrentProfileNode.LeavesCount = 0;
	MenuDisplay_EditCurrentProfileNode.NodesCount = 5;

	/* Edit current profile -> Frequency */
	MenuDisplay_EditCurrentProfileNode.Nodes[0] = &MenuDisplay_FrequencySettingsNode;
	MenuDisplay_FrequencySettingsNode.Parent = &MenuDisplay_EditCurrentProfileNode;
	MenuDisplay_FrequencySettingsNode.NamePtr = MenuDisplay_FrequencyNodeName;
	MenuDisplay_FrequencySettingsNode.LeavesCount = 2;
	MenuDisplay_FrequencySettingsNode.LeavesPtrs[0] = &MenuDisplay_FrequencyRangeLeaf; /* Edit current profile -> Frequency -> Range */
	MenuDisplay_FrequencySettingsNode.LeavesPtrs[1] = &MenuDisplay_FrequencyValueLeaf; /* Edit current profile -> Frequency -> Value */
	MenuDisplay_FrequencySettingsNode.NodesCount = 0;

	/* Edit current profile -> Code and speed */
	MenuDisplay_EditCurrentProfileNode.Nodes[1] = &MenuDisplay_CodeAndSpeedSettingsNode;
	MenuDisplay_CodeAndSpeedSettingsNode.Parent = &MenuDisplay_EditCurrentProfileNode;
	MenuDisplay_CodeAndSpeedSettingsNode.NamePtr = MenuDisplay_CodeAndSpeedNodeName;
	MenuDisplay_CodeAndSpeedSettingsNode.LeavesCount = 2;
	MenuDisplay_CodeAndSpeedSettingsNode.LeavesPtrs[0] = &MenuDisplay_CodeLeaf; /* Edit current profile -> Code and speed -> Code */
	MenuDisplay_CodeAndSpeedSettingsNode.LeavesPtrs[1] = &MenuDisplay_SpeedLeaf; /* Edit current profile -> Code and speed -> Speed */
	MenuDisplay_CodeAndSpeedSettingsNode.NodesCount = 0;

	/* Edit current profile -> Cycle */
	MenuDisplay_EditCurrentProfileNode.Nodes[2] = &MenuDisplay_CycleSettingsNode;
	MenuDisplay_CycleSettingsNode.Parent = &MenuDisplay_EditCurrentProfileNode;
	MenuDisplay_CycleSettingsNode.NamePtr = MenuDisplay_CycleNodeName;
	MenuDisplay_CycleSettingsNode.LeavesCount = 4;
	MenuDisplay_CycleSettingsNode.LeavesPtrs[0] = &MenuDisplay_IsContinuousLeaf; /* Edit current profile -> Cycle -> Is continuous */
	MenuDisplay_CycleSettingsNode.LeavesPtrs[1] = &MenuDisplay_TxTimeLeaf; /* Edit current profile -> Cycle -> TX time */
	MenuDisplay_CycleSettingsNode.LeavesPtrs[2] = &MenuDisplay_PauseTimeLeaf; /* Edit current profile -> Cycle -> Pause time */
	MenuDisplay_CycleSettingsNode.LeavesPtrs[3] = &MenuDisplay_EndingToneLeaf; /* Edit current profile -> Cycle -> Ending tone duration */
	MenuDisplay_CycleSettingsNode.NodesCount = 0;

	/* Edit current profile -> Run times */
	MenuDisplay_EditCurrentProfileNode.Nodes[3] = &MenuDisplay_RunTimesSettingsNode;
	MenuDisplay_RunTimesSettingsNode.Parent = &MenuDisplay_EditCurrentProfileNode;
	MenuDisplay_RunTimesSettingsNode.NamePtr = MenuDisplay_RunTimesNodeName;
	MenuDisplay_RunTimesSettingsNode.LeavesCount = 2;
	MenuDisplay_RunTimesSettingsNode.LeavesPtrs[0] = &MenuDisplay_StartTimeLeaf; /* Edit current profile -> Run times -> Start time */
	MenuDisplay_RunTimesSettingsNode.LeavesPtrs[1] = &MenuDisplay_FinishTimeLeaf; /* Edit current profile -> Run times -> Finish time */
	MenuDisplay_RunTimesSettingsNode.NodesCount = 0;

	/* Edit current profile -> TX power */
	MenuDisplay_EditCurrentProfileNode.Nodes[4] = &MenuDisplay_TxPowerSettingsNode;
	MenuDisplay_TxPowerSettingsNode.Parent = &MenuDisplay_EditCurrentProfileNode;
	MenuDisplay_TxPowerSettingsNode.NamePtr = MenuDisplay_TxPowerNodeName;
	MenuDisplay_TxPowerSettingsNode.LeavesCount = 1;
	MenuDisplay_TxPowerSettingsNode.LeavesPtrs[0] = &MenuDisplay_TxPowerLeaf; /* Edit current profile -> TX power -> TX power */
	MenuDisplay_TxPowerSettingsNode.NodesCount = 0;

	/* Arming */
	MenuDisplay_RootNode.Nodes[3] = &MenuDisplay_ArmingSettingsNode;
	MenuDisplay_ArmingSettingsNode.Parent = &MenuDisplay_RootNode;
	MenuDisplay_ArmingSettingsNode.NamePtr = MenuDisplay_ArmingNodeName;
	MenuDisplay_ArmingSettingsNode.LeavesCount = 2;
	MenuDisplay_ArmingSettingsNode.LeavesPtrs[0] = &MenuDisplay_ArmLeaf; /* Arming -> Arm */
	MenuDisplay_ArmingSettingsNode.LeavesPtrs[1] = &MenuDisplay_DisarmLeaf; /* Arming -> Disarm */
	MenuDisplay_ArmingSettingsNode.NodesCount = 0;

	/* Root menu leaves */
	MenuDisplay_RootNode.LeavesCount = 0;

	MenuDisplay_ActiveLineIndex = 0;

	MenuDisplay_SwitchNode(&MenuDisplay_RootNode);
}


void MenuDisplay_SwitchNode(MenuNode* nodePtr)
{
	MenuDisplay_CurrentNodePtr = nodePtr;
	MenuDisplay_CurrentNodeLinesCount = MenuDisplay_CurrentNodePtr->NodesCount + MenuDisplay_CurrentNodePtr->LeavesCount;

	/* Nodes first */
	for (uint8_t nodesCounter = 0; nodesCounter < MenuDisplay_CurrentNodePtr->NodesCount; nodesCounter ++)
	{
		char* dst = MenuDisplay_CurrentNodeLines[nodesCounter];
		MenuNode* nodePtr = (MenuNode*)MenuDisplay_CurrentNodePtr->Nodes[nodesCounter];
		const char* src = nodePtr->NamePtr;
		strncpy(dst, src, YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
	}

	/* Leaves then */
	for (uint8_t leavesCounter = 0; leavesCounter < MenuDisplay_CurrentNodePtr->LeavesCount; leavesCounter ++)
	{
		uint8_t baseCount = MenuDisplay_CurrentNodePtr->NodesCount;
		char* dst = MenuDisplay_CurrentNodeLines[baseCount + leavesCounter];
		const char* src = MenuDisplay_CurrentNodePtr->LeavesPtrs[leavesCounter]->Name;
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
	MenuNode* parent = MenuDisplay_CurrentNodePtr->Parent;
	if (NULL == parent)
	{
		/* Exiting menu */
		MenuDisplay_ExitMenuDisplay();
	}

	MenuDisplay_SwitchNode(parent);
	MenuDisplay_DrawMenuDisplay();
}

void MenuDisplay_ExitMenuDisplay(void)
{
	FoxState.CurrentDisplay = StatusDisplay;
	Main_SetDefaultButtonsActions();

	if (FoxState.SupressDrawing)
	{
		return;
	}

	FMGL_API_ClearScreen(&fmglContext);
}

void MenuDisplay_DrawMenuDisplay(void)
{
	if (FoxState.CurrentDisplay != MenuDisplay)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_CurrentDisplayIsNotMenuDisplay);
	}

	if (FoxState.SupressDrawing)
	{
		return;
	}

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	/* Display window - this lines will be displayed*/
	char window[YHL_MENU_NUMBER_OF_LINES][YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE];

	for (uint8_t linesCounter = 0; linesCounter < MenuDisplay_WindowLinesCount; linesCounter ++)
	{
		char* src = MenuDisplay_CurrentNodeLines[MenuDisplay_BaseLine + linesCounter];
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
		strncpy(LeftButton.Text, MenuDisplay_CurrentNodePtr->LeavesPtrs[leafIndex]->LeftButtonText, YHL_MENU_MAX_LEFT_BUTTON_TEXT_MEMORY_SIZE);
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
	if (currentNodeActiveLineIndex > MenuDisplay_CurrentNodePtr->NodesCount - 1)
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

	return currentNodeActiveLineIndex - MenuDisplay_CurrentNodePtr->NodesCount;
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
		MenuNode* subNode = (MenuNode*)MenuDisplay_CurrentNodePtr->Nodes[subNodeIndex];
		MenuDisplay_ActionOnNodeHandler(subNode, action);
		return;
	}

	int16_t leafIndex = MenuDisplay_GetCurrentLeafIndex(currentNodeActiveLineIndex);
	if (YHL_MENU_NOT_A_LEAF == leafIndex)
	{
		/* Not a node, not a leaf, definitely a bug */
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_NotANodeNotALeaf);
	}

	const MenuLeaf* leafPtr = MenuDisplay_CurrentNodePtr->LeavesPtrs[leafIndex];
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

void MenuDisplay_ActionOnLeafHandler(const MenuLeaf* leaf, MenuActionEnum action)
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

		uint16_t tmpWidth;
		uint16_t tmpHeight;
		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &font, 0, lineTop, &tmpWidth, &tmpHeight, false, lines + YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE * line);
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
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	for (uint8_t profile = 0; profile < EEPROM_Header.NumberOfProfiles; profile ++)
	{
		char* src = EEPROM_GetProfile(profile).Name;
		char* dst = MenuDisplay_ProfilesNames[profile];
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
	if (profileIndex != EEPROM_Header.ProfileInUse)
	{
		EEPROM_SwitchProfile(profileIndex);
	}

	MenuDisplay_DrawMenuDisplay();
}

void MenuDisplay_SelectFrequencyRange(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	strncpy(MenuDisplay_FrequencyRangesNames[0], "3.5MHz", YHL_MENU_FREQUENCY_RANGE_MEMORY_SIZE);
	strncpy(MenuDisplay_FrequencyRangesNames[1], "144MHz", YHL_MENU_FREQUENCY_RANGE_MEMORY_SIZE);

	uint8_t activeRangeIndex = MenuDisplay_GetFrequencyRangeIndex(EEPROM_CurrentProfile.Frequency.Is144MHz);

	ItemSelectionDisplay_Show("Select range",
			MenuDisplay_FrequencyRangesNames,
			YHL_MENU_FREQUENCY_RANGE_MEMORY_SIZE,
			YHL_MENU_FREQUENCY_RANGES_COUNT,
			activeRangeIndex,
			&MenuDisplay_SelectFrequencyRangeCloseHandler,
			MenuDisplay);
}

void MenuDisplay_SelectFrequencyRangeCloseHandler(uint8_t rangeIndex)
{
	uint8_t currentRangeIndex = MenuDisplay_GetFrequencyRangeIndex(FoxState.Frequency.Is144MHz);
	if (currentRangeIndex == rangeIndex)
	{
		/* Frequency range didn't change */
		MenuDisplay_DrawMenuDisplay();
		return;
	}

	bool is144MHz = MenuDisplay_GetFrequencyRangeByIndex(rangeIndex);

	uint32_t newFrequency;
	if (is144MHz)
	{
		newFrequency = YHL_DEFAULT_144M_FREQUENCY;
	}
	else
	{
		newFrequency = YHL_DEFAULT_80M_FREQUENCY;
	}

	FoxState_SetFrequency(is144MHz, newFrequency);
	PendingCommandsFlags.NeedToSetFrequencyByHandpaws = true;

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
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongIndexInMenuDisplayGetFrequencyRangeByIndex);
			return false; /* Never will be reached, to suppress warning */
	}
}

void MenuDisplay_EnterFrequency(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

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

void MenuDisplay_FormatFrequency(int32_t frequencyHz, char buffer[YHL_NUMBER_INPUT_DISPLAY_FORMATTER_BUFFER_SIZE])
{
	snprintf(buffer, YHL_NUMBER_INPUT_DISPLAY_FORMATTER_BUFFER_SIZE, "%.3f MHz", frequencyHz / 1000000.0f);
}

void MenuDisplay_EnterFrequencyOnEnterHandler(int32_t frequency)
{
	FoxState_SetFrequency(FoxState.Frequency.Is144MHz, frequency);
	PendingCommandsFlags.NeedToSetFrequencyByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}

void MenuDisplay_SelectCode(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	strncpy(MenuDisplay_FoxCodesNames[0], "Finish (MO)", YHL_MENU_FOX_CODES_MEMORY_SIZE);
	strncpy(MenuDisplay_FoxCodesNames[1], "1 (MOE)", YHL_MENU_FOX_CODES_MEMORY_SIZE);
	strncpy(MenuDisplay_FoxCodesNames[2], "2 (MOI)", YHL_MENU_FOX_CODES_MEMORY_SIZE);
	strncpy(MenuDisplay_FoxCodesNames[3], "3 (MOS)", YHL_MENU_FOX_CODES_MEMORY_SIZE);
	strncpy(MenuDisplay_FoxCodesNames[4], "4 (MOH)", YHL_MENU_FOX_CODES_MEMORY_SIZE);
	strncpy(MenuDisplay_FoxCodesNames[5], "5 (MO5)", YHL_MENU_FOX_CODES_MEMORY_SIZE);
	strncpy(MenuDisplay_FoxCodesNames[6], "Beacon (S)", YHL_MENU_FOX_CODES_MEMORY_SIZE);

	ItemSelectionDisplay_Show("Select code",
			MenuDisplay_FoxCodesNames,
			YHL_MENU_FOX_CODES_MEMORY_SIZE,
			YHL_MENU_CODES_COUNT,
			(uint8_t)FoxState.Code,
			&MenyDisplay_SelectCodeCloseHandler,
			MenuDisplay);
}


void MenyDisplay_SelectCodeCloseHandler(uint8_t codeIndex)
{
	FoxState.Code = (FoxCodeEnum)codeIndex;
	PendingCommandsFlags.NeedToSetCodeByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


void MenuDisplay_SelectFoxSpeed(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	strncpy(MenuDisplay_FoxSpeedsNames[0], "Slow", YHL_MENU_FOX_SPEEDS_MEMORY_SIZE);
	strncpy(MenuDisplay_FoxSpeedsNames[1], "Fast", YHL_MENU_FOX_SPEEDS_MEMORY_SIZE);

	ItemSelectionDisplay_Show("Select speed",
			MenuDisplay_FoxSpeedsNames,
			YHL_MENU_FOX_SPEEDS_MEMORY_SIZE,
			YHL_MENU_SPEEDS_COUNT,
			MenuDisplay_FoxSpeedToIndex(FoxState.IsFast),
			&MenyDisplay_SelectFoxSpeedCloseHandler,
			MenuDisplay);
}


void MenyDisplay_SelectFoxSpeedCloseHandler(uint8_t speedIndex)
{
	FoxState.IsFast = MenuDisplay_IndexToFoxSpeed(speedIndex);
	PendingCommandsFlags.NeedToSetSpeedByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


uint8_t MenuDisplay_FoxSpeedToIndex(bool isFast)
{
	if (isFast)
	{
		return 1;
	}

	return 0;
}


bool MenuDisplay_IndexToFoxSpeed(uint8_t index)
{
	switch (index)
	{
		case 0:
			return false;
		case 1:
			return true;
		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongIndexInMenuDisplayIndexToFoxSpeed);
			return false; /* Never will be reached */
	}
}


void MenuDisplay_SelectIsContinuousCycle(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	strncpy(MenuDisplay_CycleTypesNames[0], "Yes", YHL_MENU_CYCLE_TYPE_MEMORY_SIZE);
	strncpy(MenuDisplay_CycleTypesNames[1], "No", YHL_MENU_CYCLE_TYPE_MEMORY_SIZE);

	ItemSelectionDisplay_Show("Is continuous?",
			MenuDisplay_CycleTypesNames,
			YHL_MENU_CYCLE_TYPE_MEMORY_SIZE,
			YHL_MENU_CYCLES_TYPES_COUNT,
			MenuDisplay_CycleTypeToIndex(FoxState.Cycle.IsContinuous),
			&MenuDisplay_SelectIsContinuousCycleHandler,
			MenuDisplay);
}


void MenuDisplay_SelectIsContinuousCycleHandler(uint8_t cycleTypeIndex)
{
	FoxState.Cycle.IsContinuous = MenuDisplay_IndexToCycleType(cycleTypeIndex);
	PendingCommandsFlags.NeedToSetCycleByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


uint8_t MenuDisplay_CycleTypeToIndex(bool isContinuous)
{
	if (isContinuous)
	{
		return 0;
	}

	return 1;
}

bool MenuDisplay_IndexToCycleType(uint8_t index)
{
	switch (index)
	{
		case 0:
			return true;
		case 1:
			return false;
		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongIndexInMenuDisplayIndexToCycleType);
			return false; /* Never will be reached */
	}
}


void MenuDisplay_EnterEndingToneDuration(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	if (FoxState.Cycle.IsContinuous)
	{
		MenuDisplay_ShowCycleIsContinuousWarning();
		return;
	}

	NumberInputDisplay_Show("Ending tone duration",
		0,
		YHL_MENU_MAX_ENDINGTONE_DURATION,
		(int32_t)FoxState.EndingToneLength,
		1,
		&MenuDisplay_FormatEndingToneDuration,
		&MenuDisplay_EnterEndingToneDurationOnEnterHandler,
		MenuDisplay);
}


void MenuDisplay_ShowCycleIsContinuousWarning(void)
{
	InformationPopup_Show("Not available!", "Cycle is continuous.", MenuDisplay);
}


void MenuDisplay_EnterEndingToneDurationOnEnterHandler(int32_t duration)
{
	FoxState_SetEndingtoneDuration((uint8_t)duration);
	PendingCommandsFlags.NeedToSetEndingToneDurationByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


void MenuDisplay_FormatEndingToneDuration(int32_t duration, char buffer[YHL_NUMBER_INPUT_DISPLAY_FORMATTER_BUFFER_SIZE])
{
	snprintf(buffer, YHL_NUMBER_INPUT_DISPLAY_FORMATTER_BUFFER_SIZE, "%ds", duration);
}


void MenuDisplay_EnterTxDuration(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	if (FoxState.Cycle.IsContinuous)
	{
		MenuDisplay_ShowCycleIsContinuousWarning();
		return;
	}

	TimeInputDisplay_Show("Enter TX duration",
		YHL_MIN_TX_DURATION,
		YHL_MENU_MAX_TX_DURATION,
		FoxState.Cycle.TxTime,
		&MenuDisplay_EnterTxDurationEnterHandler,
		MenuDisplay);
}


void MenuDisplay_EnterTxDurationEnterHandler(uint32_t duration)
{
	FoxState_SetCycleDurations(duration, FoxState.Cycle.PauseTime);
	PendingCommandsFlags.NeedToSetCycleByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


void MenuDisplay_EnterPauseDuration(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	if (FoxState.Cycle.IsContinuous)
	{
		MenuDisplay_ShowCycleIsContinuousWarning();
		return;
	}

	TimeInputDisplay_Show("Enter pause duration",
		YHL_MIN_PAUSE_DURATION,
		YHL_MENU_MAX_PAUSE_DURATION,
		FoxState.Cycle.PauseTime,
		&MenuDisplay_EnterPauseDurationEnterHandler,
		MenuDisplay);
}


void MenuDisplay_EnterPauseDurationEnterHandler(uint32_t duration)
{
	FoxState_SetCycleDurations(FoxState.Cycle.TxTime, duration);
	PendingCommandsFlags.NeedToSetCycleByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


void MenuDisplay_EnterStartTime(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	TimeInputDisplay_Show("Enter start time",
		0,
		YHL_TIME_DAY_IN_SECONDS - 1,
		FoxState.GlobalState.StartTimespan,
		&MenuDisplay_EnterStartTimeEnterHandler,
		MenuDisplay);
}


void MenuDisplay_EnterStartTimeEnterHandler(uint32_t secondsSinceMidnight)
{
	FoxState_SetBeginAndEndTimespans(secondsSinceMidnight, FoxState.GlobalState.EndTimespan);
	PendingCommandsFlags.NeedToSetBeginAndEndTimesByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


void MenuDisplay_EnterFinishTime(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	TimeInputDisplay_Show("Enter finish time",
		0,
		YHL_TIME_DAY_IN_SECONDS - 1,
		FoxState.GlobalState.EndTimespan,
		&MenuDisplay_EnterFinishTimeEnterHandler,
		MenuDisplay);
}


void MenuDisplay_EnterFinishTimeEnterHandler(uint32_t secondsSinceMidnight)
{
	FoxState_SetBeginAndEndTimespans(FoxState.GlobalState.StartTimespan, secondsSinceMidnight);
	PendingCommandsFlags.NeedToSetBeginAndEndTimesByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


void MenuDisplay_Arm(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		InformationPopup_Show("Already armed!", "Fox is already armed!", MenuDisplay);
		return;
	}

	if (FoxState.ServiceSettings.IsForceTx)
	{
		InformationPopup_Show("Service mode!", "Fox in service TX!", MenuDisplay);
		return;
	}

	MenuDisplay_ExitMenuDisplay();

	PendingCommandsFlags.NeedToArmFoxByHandpaws = true;
}

void MenuDisplay_Disarm(void)
{
	if (!FoxState.GlobalState.IsArmed)
	{
		InformationPopup_Show("Not armed!", "Fox isn't armed!", MenuDisplay);
		return;
	}
	GSM_Disarm();

	InformationPopup_Show("Disarmed!", "Fox disarmed.", MenuDisplay);
}


void MenuDisplay_EnterTxPower(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	if (FoxState.Frequency.Is144MHz)
	{
		InformationPopup_Show("Not available!", "Only for 3.5MHz range", MenuDisplay);
		return;
	}

	NumberInputDisplay_Show("TX power",
		YHL_MENU_MIN_TX_POWER,
		YHL_MENU_MAX_TX_POWER,
		(int32_t)floor(FoxState.Power * YHL_MENU_TX_POWER_MULTIPLIER + 0.5f),
		YHL_MENU_TX_POWER_STEP,
		&MenuDisplay_FormatTxPower,
		&MenuDisplay_EnterTxPowerOnEnterHandler,
		MenuDisplay);
}


void MenuDisplay_EnterTxPowerOnEnterHandler(int32_t power)
{
	float actualPower = power / (float)YHL_MENU_TX_POWER_MULTIPLIER;

	FoxState_SetPower(actualPower);
	PendingCommandsFlags.NeedToSetPowerByHandpaws = true;

	MenuDisplay_DrawMenuDisplay();
}


void MenuDisplay_FormatTxPower(int32_t TxPower, char buffer[YHL_NUMBER_INPUT_DISPLAY_FORMATTER_BUFFER_SIZE])
{
	snprintf(buffer, YHL_NUMBER_INPUT_DISPLAY_FORMATTER_BUFFER_SIZE, "%.1f W", TxPower / (float)YHL_MENU_TX_POWER_MULTIPLIER);
}

void MenuDisplay_ShowArmedWarning(void)
{
	InformationPopup_Show("Fox is armed!", "Settings are locked!", MenuDisplay);
}


void MenuDisplay_EnterCurrentTime(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		MenuDisplay_ShowArmedWarning();
		return;
	}

	Time currentTimeAsTimeStruct;
	currentTimeAsTimeStruct.Days = 0;
	currentTimeAsTimeStruct.Hours = CurrentTime.Hours;
	currentTimeAsTimeStruct.Minutes = CurrentTime.Minutes;
	currentTimeAsTimeStruct.Seconds = CurrentTime.Seconds;

	uint32_t secondsSinceMidnight = TimeToTimespan(currentTimeAsTimeStruct);

	TimeInputDisplay_Show("Enter current time",
		0,
		YHL_TIME_DAY_IN_SECONDS - 1,
		secondsSinceMidnight,
		&MenuDisplay_EnterCurrentTimeEnterHandler,
		MenuDisplay);
}


void MenuDisplay_EnterCurrentTimeEnterHandler(uint32_t secondsSinceMidnight)
{
	Time newTime = TimespanToTime(secondsSinceMidnight);

	RTC_TimeTypeDef time;
	time.Hours = newTime.Hours;
	time.Minutes = newTime.Minutes;
	time.Seconds = newTime.Seconds;
	RTC_SetCurrentTime(time);

	MenuDisplay_DrawMenuDisplay();
}
