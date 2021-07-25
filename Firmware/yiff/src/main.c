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
	HL_Init();

	/* Hardware debug
	 * TODO: Remove me */
	HAL_SwitchDisplayPower(true);
	HAL_ActivateFox(true);
	HAL_SwitchBluetoothPower(true);

	HAL_SwitchUBattCheck(true);

	HAL_Delay(1000); /* To give regulators time to spin up */

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

	// TODO: Uncomment me to force EEPROM format
	//EEPROM_Format();

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
	FoxState.BatteryLevel = 1.0f;

	FoxState.CurrentTime.Hours = 0;
	FoxState.CurrentTime.Minutes = 0;
	FoxState.CurrentTime.Seconds = 0;

	FoxState.IsTXOn = false;

	FoxState.GlobalState.IsArmed = false;
	FoxState.GlobalState.CurrentState = GfsStandby;
	FoxState.CycleState.CycleState = CsPause;

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

	/**
	 * Starting to listen for commands
	 */
	UART_StartListen(&OnNewRawPacket);

	/**
	 * Initializing hardware controls
	 */
	HAL_RegisterLeftButtonHandler(Main_OnLeftButtonPressed);
	HAL_RegisterRightButtonHandler(Main_OnRightButtonPressed);
	HAL_RegisterEncoderButtonHandler(Main_OnEncoderButtonPressed);
	HAL_RegisterEncoderRotationHandler(Main_OnEncoderRotation);

	/* Debugging stuff begin */

	/* Debugging stuff end */

	while(true)
	{
		Main_MeasureBatteryLevel();

		/* Processing possible updates from smartphone */
		Main_ProcessFoxNameChange();
		Main_ProcessNewProfileAdd();
		Main_ProcessProfileSwitch();
		Main_ProcessSetProfileName();
		Main_ProcessSetFrequency();
		Main_ProcessSetCode();
		Main_ProcessSetSpeed();
		Main_ProcessSetCycle();
		Main_ProcessSetEndingToneDuration();
		Main_ProcessSetBeginAndEndTimes();
		Main_ProcessSetPower();
		Main_ProcessFoxArming();

		/* Flush profile changes to EEPROM, must be called last in sequence */
		Main_FlushProfileToEEPROM();

		RTC_Poll();

		/* TODO: Delete me, debug */
		LeftButton.IsPressed = false;
		RightButton.IsPressed = false;
	}

	return 0;
}

void Main_ProcessFoxNameChange(void)
{
	if (PendingCommandsFlags.FoxStateNameChanged)
	{
		strcpy(EEPROM_Header.Name, FoxState.Name);
		EEPROM_UpdateHeader();

		HL_RenameBluetoothDevice(FoxState.Name);

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

void Main_ProcessProfileSwitch(void)
{
	if (PendingCommandsFlags.NeedToSwitchProfile)
	{
		EEPROM_SwitchProfile(SwitchToThisProfileId);

		uint8_t result = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SwitchProfile, 1, &result);

		PendingCommandsFlags.NeedToSwitchProfile = false;
	}
}

void Main_ProcessSetProfileName(void)
{
	if (PendingCommandsFlags.NeedToSetProfileName)
	{
		strcpy(EEPROM_CurrentProfile.Name, SetThisProfileName);
		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = true;

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetProfileName, 1, &response);

		PendingCommandsFlags.NeedToSetProfileName = false;
	}
}

void Main_ProcessSetFrequency(void)
{
	if (PendingCommandsFlags.NeedToSetFrequency)
	{
		EEPROM_CurrentProfile.Frequency = FoxState.Frequency;
		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = true;

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetFrequency, 1, &response);

		PendingCommandsFlags.NeedToSetFrequency = false;
	}
}

void Main_ProcessSetCode(void)
{
	if (PendingCommandsFlags.NeedToSetCode)
	{
		EEPROM_CurrentProfile.Code = FoxState.Code;
		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = true;

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetCode, 1, &response);

		PendingCommandsFlags.NeedToSetCode = false;
	}
}

void Main_ProcessSetSpeed(void)
{
	if (PendingCommandsFlags.NeedToSetSpeed)
	{
		EEPROM_CurrentProfile.IsFast = FoxState.IsFast;
		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = true;

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetSpeed, 1, &response);

		PendingCommandsFlags.NeedToSetSpeed = false;
	}
}

void Main_ProcessSetCycle(void)
{
	if (PendingCommandsFlags.NeedToSetCycle)
	{
		EEPROM_CurrentProfile.Cycle = FoxState.Cycle;
		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = true;

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetCycle, 1, &response);

		PendingCommandsFlags.NeedToSetCycle = false;
	}
}

void Main_ProcessSetEndingToneDuration(void)
{
	if (PendingCommandsFlags.NeedToSetEndingToneDuration)
	{
		EEPROM_CurrentProfile.EndingToneLength = FoxState.EndingToneLength;
		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = true;

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetEndingToneDuration, 1, &response);

		PendingCommandsFlags.NeedToSetEndingToneDuration = false;
	}
}

void Main_ProcessSetBeginAndEndTimes(void)
{
	if (PendingCommandsFlags.NeedToSetBeginAndEndTimes)
	{
		EEPROM_CurrentProfile.StartTime = FoxState.GlobalState.StartTime;
		EEPROM_CurrentProfile.EndTime = FoxState.GlobalState.EndTime;
		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = true;

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetBeginAndEndTimes, 1, &response);

		PendingCommandsFlags.NeedToSetBeginAndEndTimes = false;
	}
}

void Main_ProcessSetPower(void)
{
	if (PendingCommandsFlags.NeedToSetPower)
	{
		EEPROM_CurrentProfile.Power = FoxState.Power;
		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = true;

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetFoxPower, 1, &response);

		PendingCommandsFlags.NeedToSetPower = false;
	}
}

void Main_ProcessFoxArming(void)
{
	if (PendingCommandsFlags.NeedToArmFox)
	{
		if (!FoxState.GlobalState.IsArmed)
		{
			/* Immediately reporting that arming in progress */
			uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
			SendResponse(ArmFox, 1, &response);

			Main_PrepareAndMatchAntenna();
			HL_UnPrepareFoxFromCycle();

			GSM_Arm();

			EmitFoxArmedEvent();
		}
		else
		{
			uint8_t response = YHL_PACKET_PROCESSOR_FAILURE;
			SendResponse(ArmFox, 1, &response);
		}

		PendingCommandsFlags.NeedToArmFox = false;
	}
}

void Main_PrepareAndMatchAntenna(void)
{
	if (FoxState.Frequency.Is144MHz)
	{
		HL_PrepareFoxForCycle();
		return; /* No need to match antenna at 144MHz*/
	}

	/* 3.5MHz */
	/* TODO: Show "Matching in progress" display */
	HL_PrepareAndMatch80m();
	/* TODO: Hide "Matching in progress" display */
}

void Main_FlushProfileToEEPROM(void)
{
	if (PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM)
	{
		EEPROM_UpdateCurrentProfile();

		PendingCommandsFlags.NeedToFlushCurrentProfileToEEPROM = false;
	}
}

void Main_MeasureBatteryLevel(void)
{
	FoxState.BatteryLevel = HAL_GetBatteryLevel();
}

void Main_OnLeftButtonPressed(void)
{
	LeftButton.IsPressed = true;
}

void Main_OnRightButtonPressed(void)
{
	RightButton.IsPressed = true;
}

void Main_OnEncoderButtonPressed(void)
{

}

void Main_OnEncoderRotation(int8_t direction)
{

}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
