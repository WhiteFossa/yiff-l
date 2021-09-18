/*
 * MenuDisplay.c
 *
 *  Created on: Aug 3, 2021
 *      Author: fossa
 */

#include <Menu/MenuDisplay.h>

void Menu_InitMenuDisplay(void)
{
	Menu_RootNode.Parent = NULL;
	strcpy(Menu_RootNode.Name, "Fox settings");

	/* Root menu nodes */
	Menu_RootNode.NodesCount = 2;
	Menu_RootNode.Nodes = malloc(sizeof(MenuNode) * Menu_RootNode.NodesCount);

	/* Profile settings */
	MenuNode* profileSettingsNode = &((MenuNode*)Menu_RootNode.Nodes)[0];
	profileSettingsNode->Parent = &Menu_RootNode;
	strcpy(profileSettingsNode->Name, "Profile settings");
	profileSettingsNode->LeavesCount = 2;
	profileSettingsNode->Leaves = malloc(sizeof(MenuLeaf) * profileSettingsNode->LeavesCount);

	/* Profile settings -> Show current profile */
	strcpy(profileSettingsNode->Leaves[0].Name, "Show current profile");
	strcpy(profileSettingsNode->Leaves[0].LeftButtonText, "Show");
	profileSettingsNode->Leaves[0].LeftButtonAction = &Menu_ShowCurrentProfileInformationPopup;

	/* Profile settings -> Select profile*/
	strcpy(profileSettingsNode->Leaves[1].Name, "Select profile");
	strcpy(profileSettingsNode->Leaves[1].LeftButtonText, "Select");
	profileSettingsNode->Leaves[1].LeftButtonAction = &Menu_SelectCurrentProfile;

	profileSettingsNode->NodesCount = 0;
	profileSettingsNode->Nodes = NULL;


	/* Edit current profile */
	MenuNode* editCurrentProfileNode = &((MenuNode*)Menu_RootNode.Nodes)[1];
	editCurrentProfileNode->Parent = &Menu_RootNode;
	strcpy(editCurrentProfileNode->Name, "Edit current profile");
	editCurrentProfileNode->LeavesCount = 0;
	editCurrentProfileNode->Leaves = malloc(sizeof(MenuLeaf) * profileSettingsNode->LeavesCount);

	editCurrentProfileNode->NodesCount = 0;
	editCurrentProfileNode->Nodes = NULL;


	/* Root menu leaves */
	Menu_RootNode.LeavesCount = 0;
	Menu_RootNode.Leaves = malloc(sizeof(MenuLeaf) * Menu_RootNode.LeavesCount);

	ActiveLineIndex = 0;
	CurrentNodeLines = NULL;
	MenuDisplay_ProfilesNames = NULL;

	Menu_SwitchNode(&Menu_RootNode);
}



void Menu_SwitchNode(MenuNode* nodePtr)
{
	Menu_CurrentNode = *nodePtr;

	CurrentNodeLinesCount = Menu_CurrentNode.NodesCount + Menu_CurrentNode.LeavesCount;
	if (CurrentNodeLines != NULL)
	{
		free(CurrentNodeLines);
	}

	CurrentNodeLines = malloc(CurrentNodeLinesCount * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);

	/* Nodes first */
	for (uint8_t nodesCounter = 0; nodesCounter < Menu_CurrentNode.NodesCount; nodesCounter ++)
	{
		char* dst = (char*)(CurrentNodeLines + nodesCounter * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
		MenuNode node = ((MenuNode*)Menu_CurrentNode.Nodes)[nodesCounter];
		char* src = node.Name;
		strcpy(dst, src);
	}

	/* Leaves then */
	for (uint8_t leavesCounter = 0; leavesCounter < Menu_CurrentNode.LeavesCount; leavesCounter ++)
	{
		uint8_t baseCount = Menu_CurrentNode.NodesCount;
		char* dst = (char*)(CurrentNodeLines + (baseCount + leavesCounter) * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
		char* src = Menu_CurrentNode.Leaves[leavesCounter].Name;
		strcpy(dst, src);
	}

	/* How many lines could we display */
	if (CurrentNodeLinesCount > YHL_MENU_NUMBER_OF_LINES)
	{
		WindowLinesCount = YHL_MENU_NUMBER_OF_LINES;
	}
	else
	{
		WindowLinesCount = CurrentNodeLinesCount;
	}

	BaseLine = 0;
	ActiveLineIndex = 0;
}

void Menu_GoToParentNode(void)
{
	MenuNode* parent = Menu_CurrentNode.Parent;
	if (NULL == parent)
	{
		/* Exiting menu */
		FoxState.CurrentDisplay = StatusDisplay;
		strcpy(LeftButton.Text, "Menu");
		strcpy(RightButton.Text, "Bt. off");

		FMGL_API_ClearScreen(&fmglContext);
		return;
	}

	Menu_SwitchNode(parent);
	Menu_DrawMenuDisplay();
}

void Menu_DrawMenuDisplay(void)
{
	if (FoxState.CurrentDisplay != MenuDisplay)
	{
		L2HAL_Error(Generic);
	}

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	/* Display window - this lines will be displayed*/
	char window[YHL_MENU_NUMBER_OF_LINES][YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE];

	for (uint8_t linesCounter = 0; linesCounter < WindowLinesCount; linesCounter ++)
	{
		char* src = (char*)(CurrentNodeLines + (BaseLine + linesCounter) * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
		char* dst = window[linesCounter];
		strcpy(dst, src);
	}

	Menu_DrawMenuLines(WindowLinesCount, window, ActiveLineIndex);

	/* Buttons texts */
	int8_t leafIndex = Menu_GetCurrentLeafIndex(Menu_GetCurrentNodeActiveLineIndex());
	if (YHL_MENU_NOT_A_LEAF == leafIndex)
	{
		/* It's a node, left button text is the same for all nodes */
		strcpy(LeftButton.Text, YHL_MENU_LEFT_BUTTON_TEXT_FOR_NODES);
	}
	else
	{
		strcpy(LeftButton.Text, Menu_CurrentNode.Leaves[leafIndex].LeftButtonText);
	}

	strcpy(RightButton.Text, "Back");

	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

uint8_t Menu_GetCurrentNodeActiveLineIndex(void)
{
	return BaseLine + ActiveLineIndex;
}

int8_t Menu_GetCurrentSubNodeIndex(uint8_t currentNodeActiveLineIndex)
{
	if (currentNodeActiveLineIndex > Menu_CurrentNode.NodesCount - 1)
	{
		return YHL_MENU_NOT_A_NODE;
	}

	return currentNodeActiveLineIndex;
}

int8_t Menu_GetCurrentLeafIndex(uint8_t currentNodeActiveLineIndex)
{
	int subNodeIndex = Menu_GetCurrentSubNodeIndex(currentNodeActiveLineIndex);

	if (subNodeIndex != YHL_MENU_NOT_A_NODE)
	{
		/* It's a node, not a leaf */
		return YHL_MENU_NOT_A_LEAF;
	}

	return currentNodeActiveLineIndex - Menu_CurrentNode.NodesCount;
}

void Menu_LeftButtonHandler(void)
{
	Menu_OnAction(MenuLeftButtonClick);
}

void Menu_OnAction(MenuActionEnum action)
{
	uint8_t currentNodeActiveLineIndex = Menu_GetCurrentNodeActiveLineIndex();

	int8_t subNodeIndex = Menu_GetCurrentSubNodeIndex(currentNodeActiveLineIndex);

	if (subNodeIndex != YHL_MENU_NOT_A_NODE)
	{
		MenuNode* subNode = &((MenuNode*)Menu_CurrentNode.Nodes)[subNodeIndex];
		Menu_ActionOnNodeHandler(subNode, action);
		return;
	}

	int8_t leafIndex = Menu_GetCurrentLeafIndex(currentNodeActiveLineIndex);
	if (YHL_MENU_NOT_A_LEAF == leafIndex)
	{
		/* Not a node, not a leaf, definitely a bug */
		L2HAL_Error(Generic);
	}

	MenuLeaf* leafPtr = &Menu_CurrentNode.Leaves[leafIndex];
	Menu_ActionOnLeafHandler(leafPtr, action);
}

void Menu_ActionOnNodeHandler(MenuNode* node, MenuActionEnum action)
{
	if (MenuLeftButtonClick == action || MenuEncoderClick == action)
	{
		/* Entering menu node */
		Menu_SwitchNode(node);
		Menu_DrawMenuDisplay();
		return;
	}

	if (MenuRightButtonClick == action)
	{
		/* Going back */
		Menu_GoToParentNode();
	}
}

void Menu_ActionOnLeafHandler(MenuLeaf* leaf, MenuActionEnum action)
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
		Menu_GoToParentNode();
	}
}

void Menu_RightButtonHandler(void)
{
	Menu_OnAction(MenuRightButtonClick);
}

void Menu_EncoderClickHandler(void)
{
	Menu_OnAction(MenuEncoderClick);
}

void Menu_EncoderRotationHandler(int8_t direction)
{
	int8_t activeLineIndex = ActiveLineIndex;

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

		Menu_ScrollUpHandler();
	}
	else if (activeLineIndex > WindowLinesCount - 1)
	{
		/* Scroll down */
		activeLineIndex = WindowLinesCount - 1;

		Menu_ScrollDownHandler();
	}

	ActiveLineIndex = activeLineIndex;

	Menu_DrawMenuDisplay();
}

void Menu_DrawMenuLines(uint8_t linesCount, char* lines, uint8_t activeLineIndex)
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

void Menu_ScrollUpHandler(void)
{
	int16_t baseLineTmp = BaseLine;

	baseLineTmp --;

	if (baseLineTmp < 0)
	{
		baseLineTmp = 0;
	}

	BaseLine = baseLineTmp;
}


void Menu_ScrollDownHandler(void)
{
	if (CurrentNodeLinesCount <= YHL_MENU_NUMBER_OF_LINES)
	{
		/* Scrolling disabled because of too few items */
		BaseLine = 0;
		return;
	}

	BaseLine ++;

	uint8_t offscreenItemsCount = CurrentNodeLinesCount - YHL_MENU_NUMBER_OF_LINES;

	if (BaseLine > offscreenItemsCount)
	{
		BaseLine = offscreenItemsCount;
	}
}

void Menu_ShowCurrentProfileInformationPopup(void)
{
	InformationPopup_Show("Current profile:", EEPROM_GetProfile(EEPROM_Header.ProfileInUse).Name, MenuDisplay);
}

void Menu_SelectCurrentProfile(void)
{
	MenuDisplay_ProfilesNames = malloc(YHL_PROFILE_NAME_MEMORY_SIZE * EEPROM_Header.NumberOfProfiles);
	for (uint8_t profile = 0; profile < EEPROM_Header.NumberOfProfiles; profile ++)
	{
		char* src = EEPROM_GetProfile(profile).Name;
		char* dst = MenuDisplay_ProfilesNames + YHL_PROFILE_NAME_MEMORY_SIZE * profile;
		strcpy(dst, src);
	}

	ItemSelectionDisplay_Show("Select profile",
			MenuDisplay_ProfilesNames,
			YHL_PROFILE_NAME_MEMORY_SIZE,
			EEPROM_Header.NumberOfProfiles,
			EEPROM_Header.ProfileInUse,
			&Menu_SelectCurrentProfileCloseHandler,
			MenuDisplay);
}

void Menu_SelectCurrentProfileCloseHandler(uint8_t profileIndex)
{
	free(MenuDisplay_ProfilesNames);
	MenuDisplay_ProfilesNames = NULL;

	if (profileIndex != EEPROM_Header.ProfileInUse)
	{
		EEPROM_SwitchProfile(profileIndex);
	}

	Menu_DrawMenuDisplay();
}

