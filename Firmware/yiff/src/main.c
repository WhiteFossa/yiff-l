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
#include <MainPrivate.h>
#include <printf.h>

int main(int argc, char* argv[])
{
	/* Initializing variables */
	FoxState_Init();

	/* Setting up hardware */
	L2HAL_Init();
	HL_Init();

	/* Initial powering on */
	HAL_ActivateFox(true);
	HAL_SwitchBluetoothPower(true);
	HAL_SwitchUBattCheck(true);

	HAL_Delay(1000); /* To give regulators time to spin up */

	/* Detecting display */
	HL_TurnDisplayOn();

	/* Connecting to display and showing boot screen */
	Main_InitDisplayAndShowBootScreen();

	/* HAL need SysTick calls */
	L2HAL_SysTick_RegisterHandler(&HAL_OnTick);

	/* Setting up CRC calculator */
	CRC_Context = L2HAL_CRC_Init();

	/* Connecting to EEPROM */
	/* 24C256 at A0h. Page size is no more than 8! Address overflow otherwise */
	EEPROMContext = L2HAL_24x_DetectEepromAtAddress(&I2C_Other, YHL_EEPROM_ADDRESS, true, YHL_EEPROM_PAGE_SIZE);
	if (!EEPROMContext.IsFound)
	{
		/* Unable to find EEPROM. */
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_UnableToFindEEPROM);
	}

	/* Gethering initial enthropy for random generator */
	Rand_GatherEnthropy();

	/* TODO: Uncomment me to force EEPROM format */
	/* EEPROM_Format(); */

	/* Initializing EEPROM data */
	EEPROM_Init();

	/* Starting RTC */
	InitRTC();

	/* Registering morse player in SysTick handler */
	MorsePlayerInit();
	L2HAL_SysTick_RegisterHandler(&MorseTickMs);

	LeftButton.IsPressed = false;
	RightButton.IsPressed = false;
	Main_SetDefaultButtonsActions();

	/* Initializing state machines */
	GSM_Init();
	CSM_Init();

	/* Loading non-profile-related data from EEPROM into fox state */
	strncpy(FoxState.Name, EEPROM_Header.Name, YHL_FOX_NAME_BUFFER_LENGTH);

	/* Loading current profile into fox state */
	EEPROM_LoadProfileIntoFoxState(&FoxState, &EEPROM_CurrentProfile);

	/* Adding seconds tick handler */
	RTC_AddOnNewSecondListener(&NewSecondCallback);

	/* Adding handlers to correct fox state after date/time change */
	RTC_AddOnDateOrTimeChangeListener(&FoxState_CorrectDateTime);

	/* Setting up UART */
	UART_Init();

	/* Setting up HC-06 Bluetooth module */
	HL_TurnBluetoothOn();

	/* Initializing sleepmode timers */
	Sleepmodes_Init();

	/* Initializing hardware controls */
	HardwareControlsEvents.IsLeftButtonPressed = false;
	HardwareControlsEvents.IsRightButtonPressed = false;
	HardwareControlsEvents.IsEncoderButtonPressed = false;
	HardwareControlsEvents.EncoderRotation = HAL_ENCODER_ROTATION_NONE;

	HAL_RegisterLeftButtonHandler(Main_OnLeftButtonPressedInterrupt);
	HAL_RegisterRightButtonHandler(Main_OnRightButtonPressedInterrupt);
	HAL_RegisterEncoderButtonHandler(Main_OnEncoderButtonPressedInterrupt);
	HAL_RegisterEncoderRotationHandler(Main_OnEncoderRotationInterrupt);

	/* Preparing menus */
	MenuDisplay_InitMenuDisplay();

	/* Clearing display after boot screen */
	FMGL_API_ClearScreen(&fmglContext);
	FMGL_API_PushFramebuffer(&fmglContext);

	/* Debugging stuff begin */

	/* Debugging stuff end */

	/* Starting to listen for commands */
	UART_StartListen();

	while(true)
	{
		/* Processing incoming packets */
		Main_ProcessIncomingPackets();

		/* Preventing sleep if fox is transmitting and so on */
		Main_ControlSleep();

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

		/* Hardware controls events */
		Main_CheckLeftButtonPressedEvent();
		Main_CheckRightButtonPressedEvent();
		Main_CheckEncoderButtonPressedEvent();
		Main_CheckEncoderRotationEvent();

		RTC_Poll();
	}

	return 0;
}

void Main_ProcessFoxNameChange(void)
{
	if (PendingCommandsFlags.FoxStateNameChanged)
	{
		strncpy(EEPROM_Header.Name, FoxState.Name, YHL_FOX_NAME_BUFFER_LENGTH);
		EEPROM_UpdateHeader();

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetName, 1, &response);

		/* Pause to let response to be transmitted before terminating bluetooth connection */
		HAL_Delay(YHL_PAUSE_BEFORE_RENAMING_BLUETOOTH);

		HL_RenameBluetoothDevice(FoxState.Name);

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
		strncpy(EEPROM_CurrentProfile.Name, SetThisProfileName, YHL_PROFILE_NAME_MEMORY_SIZE);
		EEPROM_UpdateCurrentProfile();

		uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
		SendResponse(SetProfileName, 1, &response);

		PendingCommandsFlags.NeedToSetProfileName = false;
	}
}

void Main_ProcessSetFrequency(void)
{
	if (PendingCommandsFlags.NeedToSetFrequency || PendingCommandsFlags.NeedToSetFrequencyByHandpaws)
	{
		EEPROM_CurrentProfile.Frequency = FoxState.Frequency;
		EEPROM_UpdateCurrentProfile();

		if (PendingCommandsFlags.NeedToSetFrequency)
		{
			uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
			SendResponse(SetFrequency, 1, &response);
			PendingCommandsFlags.NeedToSetFrequency = false;
		}

		if (PendingCommandsFlags.NeedToSetFrequencyByHandpaws)
		{
			EmitProfileSettingsChangedEvent();
			PendingCommandsFlags.NeedToSetFrequencyByHandpaws = false;
		}
	}
}

void Main_ProcessSetCode(void)
{
	if (PendingCommandsFlags.NeedToSetCode || PendingCommandsFlags.NeedToSetCodeByHandpaws)
	{
		EEPROM_CurrentProfile.Code = FoxState.Code;
		EEPROM_UpdateCurrentProfile();

		if (PendingCommandsFlags.NeedToSetCode)
		{
			uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
			SendResponse(SetCode, 1, &response);
			PendingCommandsFlags.NeedToSetCode = false;
		}

		if (PendingCommandsFlags.NeedToSetCodeByHandpaws)
		{
			EmitProfileSettingsChangedEvent();
			PendingCommandsFlags.NeedToSetCodeByHandpaws = false;
		}
	}
}

void Main_ProcessSetSpeed(void)
{
	if (PendingCommandsFlags.NeedToSetSpeed || PendingCommandsFlags.NeedToSetSpeedByHandpaws)
	{
		EEPROM_CurrentProfile.IsFast = FoxState.IsFast;
		EEPROM_UpdateCurrentProfile();

		if (PendingCommandsFlags.NeedToSetSpeed)
		{
			uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
			SendResponse(SetSpeed, 1, &response);
			PendingCommandsFlags.NeedToSetSpeed = false;
		}

		if (PendingCommandsFlags.NeedToSetSpeedByHandpaws)
		{
			EmitProfileSettingsChangedEvent();
			PendingCommandsFlags.NeedToSetSpeedByHandpaws = false;
		}
	}
}

void Main_ProcessSetCycle(void)
{
	if (PendingCommandsFlags.NeedToSetCycle || PendingCommandsFlags.NeedToSetCycleByHandpaws)
	{
		EEPROM_CurrentProfile.Cycle = FoxState.Cycle;
		EEPROM_UpdateCurrentProfile();

		if (PendingCommandsFlags.NeedToSetCycle)
		{
			uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
			SendResponse(SetCycle, 1, &response);
			PendingCommandsFlags.NeedToSetCycle = false;
		}

		if (PendingCommandsFlags.NeedToSetCycleByHandpaws)
		{
			EmitProfileSettingsChangedEvent();
			PendingCommandsFlags.NeedToSetCycleByHandpaws = false;
		}
	}
}

void Main_ProcessSetEndingToneDuration(void)
{
	if (PendingCommandsFlags.NeedToSetEndingToneDuration || PendingCommandsFlags.NeedToSetEndingToneDurationByHandpaws)
	{
		EEPROM_CurrentProfile.EndingToneLength = FoxState.EndingToneLength;
		EEPROM_UpdateCurrentProfile();

		if (PendingCommandsFlags.NeedToSetEndingToneDuration)
		{
			uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
			SendResponse(SetEndingToneDuration, 1, &response);
			PendingCommandsFlags.NeedToSetEndingToneDuration = false;
		}

		if (PendingCommandsFlags.NeedToSetEndingToneDurationByHandpaws)
		{
			EmitProfileSettingsChangedEvent();
			PendingCommandsFlags.NeedToSetEndingToneDurationByHandpaws = false;
		}
	}
}

void Main_ProcessSetBeginAndEndTimes(void)
{
	if (PendingCommandsFlags.NeedToSetBeginAndEndTimes || PendingCommandsFlags.NeedToSetBeginAndEndTimesByHandpaws)
	{
		EEPROM_CurrentProfile.StartTimespan = FoxState.GlobalState.StartTimespan;
		EEPROM_CurrentProfile.EndTimespan = FoxState.GlobalState.EndTimespan;
		EEPROM_UpdateCurrentProfile();

		if (PendingCommandsFlags.NeedToSetBeginAndEndTimes)
		{
			uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
			SendResponse(SetBeginAndEndTimes, 1, &response);
			PendingCommandsFlags.NeedToSetBeginAndEndTimes = false;
		}

		if (PendingCommandsFlags.NeedToSetBeginAndEndTimesByHandpaws)
		{
			EmitProfileSettingsChangedEvent();
			PendingCommandsFlags.NeedToSetBeginAndEndTimesByHandpaws = false;
		}
	}
}

void Main_ProcessSetPower(void)
{
	if (PendingCommandsFlags.NeedToSetPower || PendingCommandsFlags.NeedToSetPowerByHandpaws)
	{
		EEPROM_CurrentProfile.Power = FoxState.Power;
		EEPROM_UpdateCurrentProfile();

		if (PendingCommandsFlags.NeedToSetPower)
		{
			uint8_t response = YHL_PACKET_PROCESSOR_SUCCESS;
			SendResponse(SetFoxPower, 1, &response);
			PendingCommandsFlags.NeedToSetPower = false;
		}

		if (PendingCommandsFlags.NeedToSetPowerByHandpaws)
		{
			EmitProfileSettingsChangedEvent();
			PendingCommandsFlags.NeedToSetPowerByHandpaws = false;
		}
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

			Main_ProcessFoxArmingCommon();
		}
		else
		{
			uint8_t response = YHL_PACKET_PROCESSOR_FAILURE;
			SendResponse(ArmFox, 1, &response);
		}

		PendingCommandsFlags.NeedToArmFox = false;
	}

	if (PendingCommandsFlags.NeedToArmFoxByHandpaws)
	{
		Main_ProcessFoxArmingCommon();

		PendingCommandsFlags.NeedToArmFoxByHandpaws = false;
	}
}

void Main_ProcessFoxArmingCommon(void)
{
	if (FoxState.GlobalState.IsArmed)
	{
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_AlreadyArmed);
	}

	EmitFoxArmingInitiatedEvent();

	Main_PrepareAndMatchAntenna();
	HL_UnPrepareFoxFromCycle();

	GSM_Arm();

	EmitFoxArmedEvent();
}

void Main_PrepareAndMatchAntenna(void)
{
	/* Waking fox fully to show matching display */
	Sleepmodes_PreventSleep();

	if (FoxState.Frequency.Is144MHz)
	{
		HL_PrepareFoxForCycle();
		return; /* No need to match antenna at 144MHz*/
	}

	memset(FoxState.MatchingDisplayData.MatchingLevels, 0x00, YHL_MATCHING_LEVELS_COUNT * sizeof(float));
	FoxState.MatchingDisplayData.MatchingStep = 0;
	FoxState.GlobalState.IsMatchingInProgress = true;
	FoxState.CurrentDisplay = AntennaMatchingDisplay;

	HL_PrepareAndMatch80m();

	FoxState.CurrentDisplay = StatusDisplay;
	Main_SetDefaultButtonsActions();
	FoxState.GlobalState.IsMatchingInProgress = false;

	FMGL_API_ClearScreen(&fmglContext);

}

void Main_MeasureBatteryLevel(void)
{
	FoxState.BatteryLevel = HAL_GetBatteryLevel();
}

void Main_OnLeftButtonPressedInterrupt(void)
{
	HardwareControlsEvents.IsLeftButtonPressed = true;
}

void Main_OnRightButtonPressedInterrupt(void)
{
	HardwareControlsEvents.IsRightButtonPressed = true;
}

void Main_OnEncoderButtonPressedInterrupt(void)
{
	HardwareControlsEvents.IsEncoderButtonPressed = true;
}

void Main_OnEncoderRotationInterrupt(int8_t direction)
{
	HardwareControlsEvents.EncoderRotation = direction;
}

void Main_CheckLeftButtonPressedEvent(void)
{
	if (HardwareControlsEvents.IsLeftButtonPressed)
	{
		Main_OnLeftButtonPressed();

		HardwareControlsEvents.IsLeftButtonPressed = false;
	}
}

void Main_OnLeftButtonPressed(void)
{
	FoxState_SleepModeEnum oldSleepmode = FoxState.Sleepmodes.Mode;
	Sleepmodes_PreventSleep();

	if (oldSleepmode != SleepmodeAwake)
	{
		/* Fox used to sleep, we don't want to process this event */
		return;
	}

	if (StatusDisplay == FoxState.CurrentDisplay)
	{
		Main_EnterMenu();
	}
	else if (MenuDisplay == FoxState.CurrentDisplay)
	{
		MenuDisplay_LeftButtonHandler();
	}
	else if (InformationPopupDisplay == FoxState.CurrentDisplay)
	{
		InformationPopup_Close();
		MenuDisplay_DrawMenuDisplay();
	}
	else if (ItemSelectionDisplay == FoxState.CurrentDisplay)
	{
		ItemSelectionDisplay_LeftClickHandler();
	}
	else if (NumberInputDisplay == FoxState.CurrentDisplay)
	{
		NumberInputDisplay_LeftClickHandler();
	}
	else if (TimeInputDisplay == FoxState.CurrentDisplay)
	{
		TimeInputDisplay_LeftClickHandler();
	}
}

void Main_CheckRightButtonPressedEvent(void)
{
	if (HardwareControlsEvents.IsRightButtonPressed)
	{
		Main_OnRightButtonPressed();

		HardwareControlsEvents.IsRightButtonPressed = false;
	}
}

void Main_OnRightButtonPressed(void)
{
	FoxState_SleepModeEnum oldSleepmode = FoxState.Sleepmodes.Mode;
	Sleepmodes_PreventSleep();

	if (oldSleepmode != SleepmodeAwake)
	{
		/* Fox used to sleep, we don't want to process this event */
		return;
	}

	if (StatusDisplay == FoxState.CurrentDisplay)
	{
		/* Going to sleep */
		Sleepmodes_EnterSleep();
	}
	else if (MenuDisplay == FoxState.CurrentDisplay)
	{
		MenuDisplay_RightButtonHandler();
	}
	else if (InformationPopupDisplay == FoxState.CurrentDisplay)
	{
		InformationPopup_Close();
		MenuDisplay_DrawMenuDisplay();
	}
	else if (ItemSelectionDisplay == FoxState.CurrentDisplay)
	{
		ItemSelectionDisplay_RightClickHandler();
	}
	else if (NumberInputDisplay == FoxState.CurrentDisplay)
	{
		NumberInputDisplay_RightClickHandler();
	}
	else if (TimeInputDisplay == FoxState.CurrentDisplay)
	{
		TimeInputDisplay_RightClickHandler();
	}
}

void Main_CheckEncoderButtonPressedEvent(void)
{
	if (HardwareControlsEvents.IsEncoderButtonPressed)
	{
		Main_OnEncoderButtonPressed();

		HardwareControlsEvents.IsEncoderButtonPressed = false;
	}
}

void Main_OnEncoderButtonPressed(void)
{
	FoxState_SleepModeEnum oldSleepmode = FoxState.Sleepmodes.Mode;
	Sleepmodes_PreventSleep();

	if (oldSleepmode != SleepmodeAwake)
	{
		/* Fox used to sleep, we don't want to process this event */
		return;
	}

	if (MenuDisplay == FoxState.CurrentDisplay)
	{
		MenuDisplay_EncoderClickHandler();
	}
	else if (InformationPopupDisplay == FoxState.CurrentDisplay)
	{
		InformationPopup_Close();
		MenuDisplay_DrawMenuDisplay();
	}
	else if (ItemSelectionDisplay == FoxState.CurrentDisplay)
	{
		ItemSelectionDisplay_EncoderClickHandler();
	}
	else if (NumberInputDisplay == FoxState.CurrentDisplay)
	{
		NumberInputDisplay_EncoderClickHandler();
	}
	else if (TimeInputDisplay == FoxState.CurrentDisplay)
	{
		TimeInputDisplay_EncoderClickHandler();
	}
}

void Main_CheckEncoderRotationEvent(void)
{
	if (HardwareControlsEvents.EncoderRotation != HAL_ENCODER_ROTATION_NONE)
	{
		Main_OnEncoderRotation(HardwareControlsEvents.EncoderRotation);

		HardwareControlsEvents.EncoderRotation = HAL_ENCODER_ROTATION_NONE;
	}
}

void Main_OnEncoderRotation(int8_t direction)
{
	FoxState_SleepModeEnum oldSleepmode = FoxState.Sleepmodes.Mode;
	Sleepmodes_PreventSleep();

	if (oldSleepmode != SleepmodeAwake)
	{
		/* Fox used to sleep, we don't want to process this event */
		return;
	}

	if (MenuDisplay == FoxState.CurrentDisplay)
	{
		MenuDisplay_EncoderRotationHandler(direction);
	}
	else if (ItemSelectionDisplay == FoxState.CurrentDisplay)
	{
		ItemSelectionDisplay_EncoderRotationHandler(direction);
	}
	else if (NumberInputDisplay == FoxState.CurrentDisplay)
	{
		NumberInputDisplay_EncoderRotationHandler(direction);
	}
	else if (TimeInputDisplay == FoxState.CurrentDisplay)
	{
		TimeInputDisplay_EncoderRotationHandler(direction);
	}
}

void Main_EnterMenu(void)
{
	FoxState.CurrentDisplay = MenuDisplay;
	MenuDisplay_DrawMenuDisplay();
}

void Main_SetDefaultButtonsActions(void)
{
	snprintf(LeftButton.Text, YHL_MAX_BUTTON_TEXT_MEMORY_SIZE, "Menu");
	snprintf(RightButton.Text, YHL_MAX_BUTTON_TEXT_MEMORY_SIZE, "Sleep");
}

void Main_ControlSleep(void)
{
	if (FoxState.GlobalState.CurrentState == GfsPreparation)
	{
		/* During preparation we can't sleep at all to be able to show matching display */
		Sleepmodes_PreventSleep();
	}
	else if (
			FoxState.GlobalState.CurrentState == GfsReady
			||
			FoxState.GlobalState.CurrentState == GfsBeforeFinish
			)
	{
		/* When fox is in cycle we can't sleep deeply */
		Sleepmodes_PreventDeepSleep();
	}
}

void Main_InitDisplayAndShowBootScreen(void)
{
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
	FMGL_FontTerminusRegular12 = FMGL_FontTerminusRegular12Init();

	transparencyMode = FMGL_XBMTransparencyModeTransparentInactive;

	/* Common font settings */
	commonFont.Font = &FMGL_FontTerminusRegular12;
	commonFont.Scale = 1;
	commonFont.CharactersSpacing = 0;
	commonFont.LinesSpacing = 0;
	commonFont.BackgroundColor = &OffColor;
	commonFont.FontColor = &OnColor;
	commonFont.Transparency = &transparencyMode;

	/* Inverted common font */
	invertedCommonFont.Font = &FMGL_FontTerminusRegular12;
	invertedCommonFont.Scale = 1;
	invertedCommonFont.CharactersSpacing = 0;
	invertedCommonFont.LinesSpacing = 0;
	invertedCommonFont.BackgroundColor = &OnColor;
	invertedCommonFont.FontColor = &OffColor;
	invertedCommonFont.Transparency = &transparencyMode;

	/* Frequency font settings */
	frequencyFont.Font = &FMGL_FontTerminusRegular12;
	frequencyFont.Scale = 2;
	frequencyFont.CharactersSpacing = 0;
	frequencyFont.LinesSpacing = 0;
	frequencyFont.BackgroundColor = &OffColor;
	frequencyFont.FontColor = &OnColor;
	frequencyFont.Transparency = &transparencyMode;

	/* Clearing display */
	FMGL_API_ClearScreen(&fmglContext);
	FMGL_API_PushFramebuffer(&fmglContext);

	/* Splash screen */
	FMGL_API_XBMImage bootLogo;
	bootLogo.Width = YHL_BOOT_LOGO_WIDTH;
	bootLogo.Height = YHL_BOOT_LOGO_HEIGHT;
	bootLogo.Raster = (uint8_t*)boot_logo_bits;

	FMGL_API_RenderXBM(&fmglContext, &bootLogo, 0, 0, 1, 1, OnColor, OffColor, FMGL_XBMTransparencyModeNormal);
	FMGL_API_PushFramebuffer(&fmglContext);
}

void Main_ProcessIncomingPackets(void)
{
	if (UART_IsPacketReady)
	{
		OnNewRawPacket(UART_ReceivedPacketFullLength, UART_ReceivedPacket);

		UART_IsPacketReady = false;
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
