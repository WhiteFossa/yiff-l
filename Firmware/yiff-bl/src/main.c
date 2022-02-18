/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <main.h>

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace-impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])
{
	/* Mandatory for STM32 HAL*/
	HAL_Init();

	/* Setting up hardware */
	L2HAL_Init();
	HAL_InitHardware();

	/* Time for regulators spin-up */
	HAL_Delay(1000);

	/* Resetting commands flags */
	Main_InitPendingCommandsFlags();

	/* Detecting display */
	HL_TurnDisplayOn();

	/* Now since this we can draw */
	Main_InitDisplayAndFonts();

	/* Initializing logger */
	Log_Init();

	char textBuffer[33];
	snprintf(textBuffer, 33, "Fossa's DFU loader v%d", YBL_VERSION);
	Log_AddLine(textBuffer);

	/* Setting up CRC calculator */
	CRC_Context = L2HAL_CRC_Init();

	snprintf(textBuffer, 33, "CRC calculator ready");
	Log_AddLine(textBuffer);

	/* Connecting to EEPROM */
	EEPROMContext = L2HAL_24x_DetectEepromAtAddress(&I2C_Other, YBL_EEPROM_ADDRESS, true, YBL_EEPROM_PAGE_SIZE);
	if (!EEPROMContext.IsFound)
	{
		/* Unable to find EEPROM. */
		snprintf(textBuffer, 33, "Unable to connect to EEPROM! Halt.");
		Log_AddLine(textBuffer);

		while(true) {}
	}

	EEPROM_ReadConstantHeader(&EEPROM_ConstantHeader);
	bool isEEPROMCrcOK = EEPROM_CheckConstantHeader(&EEPROM_ConstantHeader);
	if (!isEEPROMCrcOK)
	{
		snprintf(textBuffer, 33, "Incorrect EEPROM CRC!");
		Log_AddLine(textBuffer);

		EnterDFUMode();
	}

	snprintf(textBuffer, 33, "EEPROM CRC is OK");
	Log_AddLine(textBuffer);

	/* Do we need to enter DFU mode? */
	if (EEPROM_ConstantHeader.IsEnterBootloader)
	{
		EnterDFUMode();
	}

	/* Always setting "Enter DFU mode" flag, main firmware have to clean it */
	EEPROM_ConstantHeader.IsEnterBootloader = true;
	EEPROM_WriteConstantHeader(&EEPROM_ConstantHeader);

	JumpToEntryPoint(YBL_MAIN_CODE_START);
}

void Main_InitDisplayAndFonts(void)
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

	/* Clearing display */
	FMGL_API_ClearScreen(&fmglContext);
	FMGL_API_PushFramebuffer(&fmglContext);
}

void Main_InitPendingCommandsFlags(void)
{
	PendingCommandsFlags.IsRebootToMainFirmware = false;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
