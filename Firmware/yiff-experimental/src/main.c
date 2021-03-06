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

	HalInitHardware();

	HAL_Delay(100);

	/* Detecting display */
	L2HAL_SSD1306_Context.I2CHandle = &I2CHandle;
	L2HAL_SSD1306_DetectDisplay(&L2HAL_SSD1306_Context);
	L2HAL_SSD1306_TurnDisplayOn(&L2HAL_SSD1306_Context);

	/* Colors */
	FMGL_API_ColorStruct OffColor;
	OffColor.R = 0;
	OffColor.G = 0;
	OffColor.B = 0;

	FMGL_API_ColorStruct OnColor;
	OnColor.R = FMGL_API_MAX_CHANNEL_BRIGHTNESS;
	OnColor.G = FMGL_API_MAX_CHANNEL_BRIGHTNESS;
	OnColor.B = FMGL_API_MAX_CHANNEL_BRIGHTNESS;

	/* Attaching FMGL to display */
	fmglContext = FMGL_API_AttachToDriver(&L2HAL_SSD1306_Context, &L2HAL_SSD1306_GetWidth, &L2HAL_SSD1306_GetHeight, &L2HAL_SSD1306_SetActiveColor,
			&L2HAL_SSD1306_DrawPixel, &L2HAL_SSD1306_GetPixel, &L2HAL_SSD1306_PushFramebuffer, OffColor);

	/* Initializing font */
	FMGL_API_Font font= FMGL_FontTerminusRegular12Init();

	FMGL_API_XBMTransparencyMode transparencyMode = FMGL_XBMTransparencyModeNormal;

	/* Font settings */
	fontSettings.Font = &font;
	fontSettings.Scale = 1;
	fontSettings.CharactersSpacing = 0;
	fontSettings.LinesSpacing = 0;
	fontSettings.FontColor = &OnColor;
	fontSettings.BackgroundColor = &OffColor;
	fontSettings.Transparency = &transparencyMode;

	/* Clearing display */
	FMGL_API_ClearScreen(&fmglContext);
	FMGL_API_PushFramebuffer(&fmglContext);

	inputMode = ImFreq;
	DrawInputMode();

	freg = FREGmin;
	HalSetPllFrequency(freg);
	DrawFrequencies();

	antennaMatching = MIN_ANTENNA_MATCHING;
	DrawAntennaMatching();

	DrawDetectorLevel();

	/* Bug somewhere in encoder driver - if button and encoder registrations are swapped, then encoder always
	 * counter-clockwise. */
	/* Encoder button */
	L2HAL_Buttons_AddButton(&L2HAL_Buttons_Context, GPIOA, GPIO_PIN_2, NULL, 10, &EncoderButtonCallback);

	/* Attaching encoder */
	L2HAL_Encoders_AddEncoder(&L2HAL_Encoders_Context, GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1, GPIO_PIN_RESET, 10, NULL, &EncoderCallback);

	manipulationCounter = 500;
	isManipulationOn = false;
	modulationFlag = false;

	/* Registering systick handler */
	L2HAL_SysTick_RegisterHandler(&MySysTickHandler);

	while(true)
	{
		L2HAL_Buttons_Poll(&L2HAL_Buttons_Context);

		if (HalIsDetectorValueUpdated)
		{
			DrawDetectorLevel();
			HalIsDetectorValueUpdated = false;
		}
	}

	return 0;
}

void EncoderCallback(L2HAL_Encoders_EncoderStruct* encoderStruct, L2HAL_Encoders_Direction direction)
{
	int8_t tmpMatching;

	switch (inputMode)
	{
		case ImFreq:
			if (Clockwise == direction)
			{
				freg += FREGstep;
			}
			else
			{
				freg -= FREGstep;
			}

			if (freg > FREGmax)
			{
				freg = FREGmax;
			}

			if (freg < FREGmin)
			{
				freg = FREGmin;
			}

			HalSetPllFrequency(freg);

			DrawFrequencies();
		break;

		case ImMatching:
			tmpMatching = antennaMatching;

			if (Clockwise == direction)
			{
				tmpMatching ++;
			}
			else
			{
				tmpMatching --;
			}

			if (tmpMatching < MIN_ANTENNA_MATCHING)
			{
				tmpMatching = MIN_ANTENNA_MATCHING;
			}

			if (tmpMatching > MAX_ANTENNA_MATCHING)
			{
				tmpMatching = MAX_ANTENNA_MATCHING;
			}

			antennaMatching = tmpMatching;

			HalSetAntennaMatching(antennaMatching);

			DrawAntennaMatching();
		break;
	}
}

void EncoderButtonCallback(L2HAL_Buttons_ButtonStruct* button, GPIO_PinState newPinState, uint16_t* portData)
{
	if (newPinState != GPIO_PIN_RESET)
	{
		return;
	}

	switch (inputMode)
	{
		case ImFreq:
			inputMode = ImMatching;
		break;

		case ImMatching:
			inputMode = ImFreq;
		break;
	}

	DrawInputMode();
}

void DrawFrequencies()
{
	char buffer[32];

	/* Current frequency */
	sprintf(buffer, "Freq: %d KHz", GetKHzFromFreg(freg));
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &fontSettings, 12, 0, NULL, NULL, false, buffer);

	FMGL_API_PushFramebuffer(&fmglContext);
}

void DrawDetectorLevel()
{
	char buffer[32];

	sprintf(buffer, "Detector: %04d", HalDetectorAverage);

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &fontSettings, 12, 32, NULL, NULL, false, buffer);

	FMGL_API_PushFramebuffer(&fmglContext);
}

void DrawAntennaMatching()
{
	char buffer[32];

	sprintf(buffer, "Matching: %02d", antennaMatching);

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &fontSettings, 12, 16, NULL, NULL, false, buffer);

	FMGL_API_PushFramebuffer(&fmglContext);
}

uint32_t GetKHzFromFreg(uint32_t freg)
{
#ifndef USE_144
	return (uint32_t)((float)Fmclk * (float)freg / (float)4294967296);
#else
	return (uint32_t)((float)8 * (float)Fmclk * (float)freg / (float)4294967296);
#endif
}

void DrawInputMode()
{
	char buffer[2];

	sprintf(buffer, " ");
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &fontSettings, 0, 0, NULL, NULL, false, buffer);
	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &fontSettings, 0, 16, NULL, NULL, false, buffer);

	sprintf(buffer, ">");

	uint16_t yPos;

	switch (inputMode)
	{
		case ImFreq:
			yPos = 0;
		break;

		case ImMatching:
			yPos = 16;
		break;
	}

	FMGL_API_RenderTextWithLineBreaks(&fmglContext, &fontSettings, 0, yPos, NULL, NULL, false, buffer);

	FMGL_API_PushFramebuffer(&fmglContext);
}

void MySysTickHandler(void)
{
#ifdef USE_144
		modulationFlag = !modulationFlag;
		if (isManipulationOn)
		{
			HalSwitchManipulator(modulationFlag);
		}
		else
		{
			HalSwitchManipulator(false);
		}
#endif

	if (manipulationCounter > 0)
	{
		manipulationCounter --;
	}

	if (0 == manipulationCounter)
	{
		manipulationCounter = 500;

		isManipulationOn = !isManipulationOn;
#ifndef USE_144
		HalSwitchManipulator(isManipulationOn);
#endif
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
