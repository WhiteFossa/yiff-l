/*
 * InformationPopupDisplay.h
 *
 *  Created on: Sep 11, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_INFORMATIONPOPUPDISPLAY_H_
#define INCLUDE_INFORMATIONPOPUPDISPLAY_H_


/**
 * Information popup - displays information and closes on any button press.
 */

/**
 * Position of title top
 */
#define YHL_INFORMAION_POPUP_TITLE_TOP 0

#include <FoxStatus.h>
#include <GraphicsBase.h>
#include <Buttons.h>

extern FoxStateStruct FoxState;
extern FoxButtonStruct LeftButton;
extern FoxButtonStruct RightButton;

/**
 * Display information popup
 */
void InformationPopup_Show(char* title, char* message, FoxDisplayEnum previousDisplay);

/**
 * Close information popup
 */
void InformationPopup_Close(void);

#endif /* INCLUDE_INFORMATIONPOPUPDISPLAY_H_ */
