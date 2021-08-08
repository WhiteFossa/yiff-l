/*
 * MenuDisplay.c
 *
 *  Created on: Aug 3, 2021
 *      Author: fossa
 */

#include <MenuDisplay.h>

void Menu_InitMenuDisplay(void)
{
	ActiveLineIndex = 0;

	Menu_RootNode.Parent = NULL;
	strcpy(Menu_RootNode.Name, "Menu root");

	Menu_RootNode.LeafsCount = 3;
	Menu_RootNode.Leafs = malloc(sizeof(MenuLeaf) * Menu_RootNode.LeafsCount);

	strcpy(Menu_RootNode.Leafs[0].Name, "Test leaf 1");
	strcpy(Menu_RootNode.Leafs[1].Name, "Test leaf 2");
	strcpy(Menu_RootNode.Leafs[2].Name, "Test leaf 3");

	Menu_RootNode.NodesCount = 2;
	Menu_RootNode.Nodes = malloc(sizeof(MenuNode) * Menu_RootNode.NodesCount);

	MenuNode node1 = ((MenuNode*)Menu_RootNode.Nodes)[0];
	node1.Parent = &Menu_RootNode;
	strcpy(node1.Name, "Menu node 1");
	node1.LeafsCount = 1;
	node1.Leafs = malloc(sizeof(MenuLeaf) * 1);
	strcpy(node1.Leafs[0].Name, "Test leaf 1-1");
	node1.NodesCount = 0;
	node1.Nodes = NULL;
	strcpy(node1.RightButtonText, "Test1");
	node1.RightButtonAction = Menu_Node1_RightButtonAction;

	MenuNode node2 = ((MenuNode*)Menu_RootNode.Nodes)[1];
	node2.Parent = &Menu_RootNode;
	strcpy(node2.Name, "Menu node 2");
	node2.LeafsCount = 1;
	node2.Leafs = malloc(sizeof(MenuLeaf) * 1);
	strcpy(node2.Leafs[0].Name, "Test leaf 2-1");
	node2.NodesCount = 0;
	node2.Nodes = NULL;
	strcpy(node1.RightButtonText, "Test2");
	node2.RightButtonAction = Menu_Node1_RightButtonAction;

	strcpy(Menu_RootNode.RightButtonText, "Exit");
	Menu_RootNode.RightButtonAction = Menu_RootNode_RightButtonAction;

	Menu_CurrentNode = Menu_RootNode;
}

void Menu_DrawMenuDisplay(void)
{
	if (FoxState.CurrentDisplay != MenuDisplay)
	{
		L2HAL_Error(Generic);
	}

	FMGL_API_SetActiveColor(&fmglContext, OnColor);

	FMGL_API_ClearScreen(&fmglContext);

	//FMGL_API_RenderTextWithLineBreaks(&fmglContext, &commonFont, 0, 0, NULL, NULL, false, "Menu will be here");
	char testLines[8][32];
	strcpy(testLines[0], "Line 1");
	strcpy(testLines[1], "Line 2");
	strcpy(testLines[2], "Line 3");
	strcpy(testLines[3], "Line 4");
	strcpy(testLines[4], "Line 5");
	strcpy(testLines[5], "Line 6");
	strcpy(testLines[6], "Line 7");
	strcpy(testLines[7], "Line 8");

	Menu_DrawMenuLines(8, testLines, ActiveLineIndex);

	/* Buttons texts */
	strcpy(LeftButton.Text, "Activ.");
	strcpy(RightButton.Text, Menu_CurrentNode.RightButtonText);

	DrawButtons();

	FMGL_API_PushFramebuffer(&fmglContext);
}

void Menu_RightButtonHandler(void)
{
	Menu_CurrentNode.RightButtonAction();
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
		activeLineIndex = 0;
	}
	else if (activeLineIndex > 7)
	{
		activeLineIndex = 7;
	}

	ActiveLineIndex = activeLineIndex;

	Menu_DrawMenuDisplay();
}

void Menu_DrawMenuLines(uint8_t linesCount, char** lines, uint8_t activeLineIndex)
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

		FMGL_API_RenderTextWithLineBreaks(&fmglContext, &font, 0, lineTop, NULL, NULL, false, "Menu will be here");
	}
}

void Menu_RootNode_RightButtonAction(void)
{
	/* Restoring status display */
	FoxState.CurrentDisplay = StatusDisplay;
	strcpy(LeftButton.Text, "Menu");
	strcpy(RightButton.Text, "Bt. off");

	FMGL_API_ClearScreen(&fmglContext);
}

void Menu_Node1_RightButtonAction(void)
{

}

void Menu_Node2_RightButtonAction(void)
{

}
