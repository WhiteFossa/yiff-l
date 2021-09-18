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
	strcpy(MenuDisplay_RootNode.Name, "Fox settings");

	/* Root menu nodes */
	MenuDisplay_RootNode.NodesCount = 2;
	MenuDisplay_RootNode.Nodes = malloc(sizeof(MenuNode) * MenuDisplay_RootNode.NodesCount);

	/* Profile settings */
	MenuNode* profileSettingsNode = &((MenuNode*)MenuDisplay_RootNode.Nodes)[0];
	profileSettingsNode->Parent = &MenuDisplay_RootNode;
	strcpy(profileSettingsNode->Name, "Profile settings");
	profileSettingsNode->LeavesCount = 2;
	profileSettingsNode->Leaves = malloc(sizeof(MenuLeaf) * profileSettingsNode->LeavesCount);

	/* Profile settings -> Show current profile */
	strcpy(profileSettingsNode->Leaves[0].Name, "Show current profile");
	strcpy(profileSettingsNode->Leaves[0].LeftButtonText, "Show");
	profileSettingsNode->Leaves[0].LeftButtonAction = &MenuDisplay_ShowCurrentProfileInformationPopup;

	/* Profile settings -> Select profile*/
	strcpy(profileSettingsNode->Leaves[1].Name, "Select profile");
	strcpy(profileSettingsNode->Leaves[1].LeftButtonText, "Select");
	profileSettingsNode->Leaves[1].LeftButtonAction = &MenuDisplay_SelectCurrentProfile;

	profileSettingsNode->NodesCount = 0;
	profileSettingsNode->Nodes = NULL;


	/* Edit current profile */
	MenuNode* editCurrentProfileNode = &((MenuNode*)MenuDisplay_RootNode.Nodes)[1];
	editCurrentProfileNode->Parent = &MenuDisplay_RootNode;
	strcpy(editCurrentProfileNode->Name, "Edit current profile");
	editCurrentProfileNode->LeavesCount = 0;
	editCurrentProfileNode->Leaves = malloc(sizeof(MenuLeaf) * profileSettingsNode->LeavesCount);

	editCurrentProfileNode->NodesCount = 0;
	editCurrentProfileNode->Nodes = NULL;


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
		strcpy(dst, src);
	}

	/* Leaves then */
	for (uint8_t leavesCounter = 0; leavesCounter < MenuDisplay_CurrentNode.LeavesCount; leavesCounter ++)
	{
		uint8_t baseCount = MenuDisplay_CurrentNode.NodesCount;
		char* dst = (char*)(MenuDisplay_CurrentNodeLines + (baseCount + leavesCounter) * YHL_MENU_MAX_ITEM_TEXT_MEMORY_SIZE);
		char* src = MenuDisplay_CurrentNode.Leaves[leavesCounter].Name;
		strcpy(dst, src);
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
		strcpy(LeftButton.Text, "Menu");
		strcpy(RightButton.Text, "Bt. off");

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
		strcpy(dst, src);
	}

	MenuDisplay_DrawMenuLines(MenuDisplay_WindowLinesCount, window, MenuDisplay_ActiveLineIndex);

	/* Buttons texts */
	int8_t leafIndex = MenuDisplay_GetCurrentLeafIndex(MenuDisplay_GetCurrentNodeActiveLineIndex());
	if (YHL_MENU_NOT_A_LEAF == leafIndex)
	{
		/* It's a node, left button text is the same for all nodes */
		strcpy(LeftButton.Text, YHL_MENU_LEFT_BUTTON_TEXT_FOR_NODES);
	}
	else
	{
		strcpy(LeftButton.Text, MenuDisplay_CurrentNode.Leaves[leafIndex].LeftButtonText);
	}

	strcpy(RightButton.Text, "Back");

	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

uint8_t MenuDisplay_GetCurrentNodeActiveLineIndex(void)
{
	return MenuDisplay_BaseLine + MenuDisplay_ActiveLineIndex;
}

int8_t MenuDisplay_GetCurrentSubNodeIndex(uint8_t currentNodeActiveLineIndex)
{
	if (currentNodeActiveLineIndex > MenuDisplay_CurrentNode.NodesCount - 1)
	{
		return YHL_MENU_NOT_A_NODE;
	}

	return currentNodeActiveLineIndex;
}

int8_t MenuDisplay_GetCurrentLeafIndex(uint8_t currentNodeActiveLineIndex)
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

	int8_t subNodeIndex = MenuDisplay_GetCurrentSubNodeIndex(currentNodeActiveLineIndex);

	if (subNodeIndex != YHL_MENU_NOT_A_NODE)
	{
		MenuNode* subNode = &((MenuNode*)MenuDisplay_CurrentNode.Nodes)[subNodeIndex];
		MenuDisplay_ActionOnNodeHandler(subNode, action);
		return;
	}

	int8_t leafIndex = MenuDisplay_GetCurrentLeafIndex(currentNodeActiveLineIndex);
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
	int8_t activeLineIndex = MenuDisplay_ActiveLineIndex;

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

	MenuDisplay_ActiveLineIndex = activeLineIndex;

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

	MenuDisplay_BaseLine = baseLineTmp;
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
		strcpy(dst, src);
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

