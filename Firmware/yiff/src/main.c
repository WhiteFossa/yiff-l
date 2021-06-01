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
//#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include <main.h>
#include <printf.h>

int main(int argc, char* argv[])
{
	/* Setting up hardware */
	L2HAL_Init();
	HAL_IntiHardware();

	/* Setting up CRC calculator */
	CRC_Context = L2HAL_CRC_Init();

	/* Connecting to EEPROM */
	/* 24C256 at A0h. Page size is no more than 8! Address overflow otherwise */
	EEPROMContext = L2HAL_24x_DetectEepromAtAddress(&I2C_Other, YHL_EEPROM_ADDRESS, true, YHL_EEPROM_PAGE_SIZE);
	if (!EEPROMContext.IsFound)
	{
		/* Unable to find EEPROM */
		L2HAL_Error(Generic);
	}

	/* Initializing EEPROM data */
	EEPROM_Init();

	/* Starting RTC */
	InitRTC();

	/* Registering morse player in SysTick handler */
	MorsePlayerInit();
	L2HAL_SysTick_RegisterHandler(&MorseTickMs);

	HAL_Delay(100);

	/* Detecting display */
	L2HAL_SSD1327_Context = L2HAL_SSD1327_DetectDisplay(&I2C_Display);
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

	/* Initial fox state*/
	FoxState.BatteryLevel = 0.8f;

	FoxState.CurrentTime.Hours = 0;
	FoxState.CurrentTime.Minutes = 0;
	FoxState.CurrentTime.Seconds = 0;

	FoxState.IsTXOn = false;

	FoxState.GlobalState.CurrentState = Standby;
	FoxState.CycleState.CycleState = Pause;

	LeftButton.IsPressed = false;
	sprintf(LeftButton.Text, "Menu");

	RightButton.IsPressed = false;
	sprintf(RightButton.Text, "Bt. off");

	/* Initializing state machines */
	GSM_Init();
	CSM_Init();

	/* Loading non-profile-related data from EEPROM into fox state */
	strcpy(FoxState.Name, EEPROM_Header.Name);

	/* Loading current profile into fox state */
	EEPROM_LoadProfileIntoFoxState(&FoxState, &EEPROM_CurrentProfile);

	/* Adding seconds tick handler */
	RTC_AddOnNewSecondListener(&NewSecondCallback);

	/* Adding handlers to correct fox state after date/time change */
	RTC_AddOnDateOrTimeChangeListener(&FoxState_CorrectDateTime);

	/**
	 * Setting up UART
	 */
	UART_Init();

	/**
	 * Setting up HC-06 Bluetooth module
	 */
	HC06_Context = L2HAL_HC06_AttachToDevice(&UART_Handle);
	if (!HC06_Context.IsFound)
	{
		L2HAL_Error(Generic);
	}

	/** Do not set name or pin, it leads to un-pairing
	L2HAL_HC06_SetName(&HC06_Context, FoxState.Name);
	L2HAL_HC06_SetPIN(&HC06_Context, YHL_BLUETOOTH_PIN);
	*/

	/**
	 * Starting to listen for commands
	 */
	UART_StartListen(&OnNewRawPacket);

	/* Debugging stuff begin */

	/* Debugging stuff end */

	while(true)
	{
		RTC_Poll();

		/* Processing possible updates from smartphone */
		Main_ProcessFoxNameChange();
		Main_ProcessNewProfileAdd();
	}

	return 0;
}

void Main_ProcessFoxNameChange(void)
{
	if (PendingCommandsFlags.FoxStateNameChanged)
	{
		strcpy(EEPROM_Header.Name, FoxState.Name);
		EEPROM_UpdateHeader();

		// TODO: Restart HC-06 when hardware will be ready

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetName, 1, &response);

		PendingCommandsFlags.FoxStateNameChanged = false;
	}
}

void Main_ProcessNewProfileAdd(void)
{
	if (PendingCommandsFlags.NeedToAddNewProfile)
	{
		EEPROM_AddProfile();

		uint8_t result = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(AddNewProfile, 1, &result);

		PendingCommandsFlags.NeedToAddNewProfile = false;
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
