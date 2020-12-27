/*
	This file is part of Fossa's MCU Graphics Library (FMGL).

	FMGL is free software:
	you can redistribute it and/or modify it under the terms of the
	GNU General Public License as published by the Free Software
	Foundation, either version 3 of the License, or (at your option)
	any later version.

	FMGL is distributed in the hope
	that it will be useful, but WITHOUT ANY WARRANTY; without even
	the implied warranty of MERCHANTABILITY or FITNESS FOR A
	PARTICULAR PURPOSE.  See the GNU General Public License for more
	details.

	You should have received a copy of the GNU General Public License
	along with FMGL.

	If not, see <http://www.gnu.org/licenses/>.

	-------------------------------------------------------------------------

	Created by White Fossa, also known as Artyom Vetrov.

	Feel free to contact: whitefossa@gmail.com

	Repository: https://github.com/WhiteFossa/stm32-l2hal

	If repository above is not available, try my LiveJournal:
	fossa-white.livejournal.com

	or as last measure try to search for #WhiteFossa hashtag.

	-------------------------------------------------------------------------
 */

#ifndef __MAIN_H
#define __MAIN_H

#include <l2hal.h>
#include <fmgl.h>
#include <terminusRegular12.h>
#include <l2hal_ad9835.h>

/* PLL-related stuff */
#define Fmclk 50000000UL

/* 80m: 3.50-3.65 x 1*/
#define FREGmin 300647711UL /* 3.50MHz */
#define FREGmax 313532613UL  /* 3.65MHz */

#define FREGstep 85899UL /* 1KHz step */


/* 2m: 144-146 x 1 = 18.0 - 18.25 x 8*/
//#define FREGmin 1546188227UL /* 18.00MHz */
//#define FREGmax 1567663063UL  /* 18.25MHz */

//#define FREGstep 107374UL /* 1.25KHz step */


/**
 * FMGL context.
 */
FMGL_API_DriverContext fmglContext;

/**
 * Font settings.
 */
FMGL_API_FontSettings fontSettings;

/**
 * FREG (in DDS)
 */
uint32_t freg;

/**
 * FREG (new)
 */
uint32_t newFreg;

/* Callback function for encoder */
void EncoderCallback(L2HAL_Encoders_EncoderStruct* encoderStruct, L2HAL_Encoders_Direction direction);

/* Callback function for encoder button */
void EncoderButtonCallback(L2HAL_Buttons_ButtonStruct* button, GPIO_PinState newPinState, uint16_t* portData);

void DrawFrequencies();

uint32_t GetHzFromFreg(uint32_t freg);

#endif /* __MAIN_H */
