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


// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include "main.h"

int main(int argc, char* argv[])
{
	/* Setting up hardware */
	L2HAL_Init();

	InitRTC();

	HAL_Delay(100);

	/* Detecting display */
	L2HAL_SSD1327_Context = L2HAL_SSD1327_DetectDisplay(&I2CHandle);
	L2HAL_SSD1327_InitDisplay(&L2HAL_SSD1327_Context);

	/* Colors */
	OffColor.R = 0;
	OffColor.G = 0;
	OffColor.B = 0;

	OnColor.R = 255;
	OnColor.G = 255;
	OnColor.B = 255;

	/* Attaching FMGL to display */
	fmglContext = FMGL_API_AttachToDriver(&L2HAL_SSD1327_Context, &L2HAL_SSD1327_GetWidth, &L2HAL_SSD1327_GetHeight, &L2HAL_SSD1327_SetActiveColor,
			&L2HAL_SSD1327_DrawPixel, &L2HAL_SSD1327_GetPixel, &L2HAL_SSD1327_PushFramebuffer, OffColor);

	/* Initializing font */
	FMGL_API_Font font= FMGL_FontTerminusRegular12Init();

	FMGL_API_XBMTransparencyMode transparencyMode = FMGL_XBMTransparencyModeTransparentInactive;

	/* Common font settings */
	commonFont.Font = &font;
	commonFont.Scale = 1;
	commonFont.CharactersSpacing = 0;
	commonFont.LinesSpacing = 0;
	commonFont.BackgroundColor = &OffColor;
	commonFont.FontColor = &OnColor;
	commonFont.Transparency = &transparencyMode;

	/* Inverted common font */
	invertedCommonFont.Font = &font;
	invertedCommonFont.Scale = 1;
	invertedCommonFont.CharactersSpacing = 0;
	invertedCommonFont.LinesSpacing = 0;
	invertedCommonFont.BackgroundColor = &OnColor;
	invertedCommonFont.FontColor = &OffColor;
	invertedCommonFont.Transparency = &transparencyMode;

	/* Frequency font settings */
	frequencyFont.Font = &font;
	frequencyFont.Scale = 2;
	frequencyFont.CharactersSpacing = 0;
	frequencyFont.LinesSpacing = 0;
	frequencyFont.BackgroundColor = &OffColor;
	frequencyFont.FontColor = &OnColor;
	frequencyFont.Transparency = &transparencyMode;

	/* Clearing display */
	FMGL_API_ClearScreen(&fmglContext);
	FMGL_API_PushFramebuffer(&fmglContext);

	FoxState.BatteryLevel = 0.8;

	FoxState.CurrentTime.Hours = 13;
	FoxState.CurrentTime.Minutes = 37;
	FoxState.CurrentTime.Seconds = 30;

	FoxState.IsTXOn = true;

	sprintf(FoxState.Name, "Yiffiest Foxy #1");

	FoxState.Frequency.Is144MHz = false;
	FoxState.Frequency.FrequencyHz = 3500000;

	FoxState.Code = Beacon;

	FoxState.Cycle.IsContinuous = false;
	FoxState.Cycle.TxTime.Hours = 0;
	FoxState.Cycle.TxTime.Minutes = 1;
	FoxState.Cycle.TxTime.Seconds = 0;
	FoxState.Cycle.PauseTime.Hours = 0;
	FoxState.Cycle.PauseTime.Minutes = 4;
	FoxState.Cycle.PauseTime.Seconds = 0;

	FoxState.EndingToneLength = 5;

	FoxState.GlobalState.CurrentState = BeforeFinish;
	FoxState.GlobalState.StateChangeTime.Hours = 14;
	FoxState.GlobalState.StateChangeTime.Minutes = 00;
	FoxState.GlobalState.StateChangeTime.Seconds = 00;

	FoxState.CycleState.CycleState = Pause;
	FoxState.CycleState.StateChangeTime.Hours = 13;
	FoxState.CycleState.StateChangeTime.Minutes = 38;
	FoxState.CycleState.StateChangeTime.Seconds = 00;

	FoxState.Power = 3.0f;

	LeftButton.IsPressed = true;
	sprintf(LeftButton.Text, "Menu");

	RightButton.IsPressed = false;
	sprintf(RightButton.Text, "Bt. off");

	while(true)
	{
		RTC_Poll(&NewSecondCallback);
	}

	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
