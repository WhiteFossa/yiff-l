/*
 * FoxStatusPrivate.h
 *
 *  Created on: Oct 9, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_FOXSTATUSPRIVATE_H_
#define INCLUDE_FOXSTATUSPRIVATE_H_

#include <stdbool.h>
#include <stdint.h>
#include <l2hal_aux.h>

#define YHL_FOX_NAME_MAX_LENGTH 32

#define YHL_FOX_NAME_BUFFER_LENGTH (YHL_FOX_NAME_MAX_LENGTH + 1)

/**
 * Set to HAL_AM_MAX_VALUE + 1
 */
#define YHL_MATCHING_LEVELS_COUNT 64

/**
 * Global fox state
 */
typedef enum
{
	/**
	 * Fox is in standby, awaiting to be programmed.
	 */
	GfsStandby,

	/**
	 * Fox is programmed, on countdown before start.
	 */
	GfsBeforeStart,

	/**
	 * Fox preparing for transmission
	 */
	GfsPreparation,

	/**
	 * Fox is ready to start
	 */
	GfsReady,

	/**
	 * Fox is running, on countdown before finish.
	 */
	GfsBeforeFinish
}
GlobalFoxStateEnum;

/**
 * What code to transmit
 */
typedef enum
{
	Finish = 0, /* MO */
	F1 = 1, /* MOE */
	F2 = 2, /* MOI */
	F3 = 3, /* MOS */
	F4 = 4, /* MOH */
	F5 = 5, /* MO5 */
	Beacon = 6 /* S */
}
FoxCodeEnum;

/**
 * What display to show
 */
typedef enum
{
	/**
	 * Status display
	 */
	StatusDisplay,

	/**
	 * Antenna matching display (3.5MHz)
	 */
	AntennaMatchingDisplay,

	/**
	 * Menu display
	 */
	MenuDisplay,

	/**
	 * Popup with information
	 */
	InformationPopupDisplay,

	/**
	 * Display, allowing to select an item
	 */
	ItemSelectionDisplay,

	/**
	 * Display, allowing to enter a number
	 */
	NumberInputDisplay,

	/**
	 * Display, allowing to enter a time
	 */
	TimeInputDisplay
}
FoxDisplayEnum;

/**
 * Fox cycle state
 */
typedef enum
{
	/**
	 * Transmitting
	 */
	CsTx,

	/**
	 * Transmitting end tone
	 */
	CsEndingTone,

	/**
	 * Pause
	 */
	CsPause,

	/**
	 * Preparation for transmission
	 */
	CsPreparation,

	/**
	 * Fox is ready for transmission
	 */
	CsReady
}
CycleStateEnum;

/**
 * Fox sleep modes
 */
typedef enum
{
	/**
	 * Fox is awake, controls are on
	 */
	SleepmodeAwake,

	/**
	 * MCU clocked at high speed, controls are off
	 */
	SleepmodeSleep,

	/**
	 * Regulators disabled, MCU clocked at low speed, controls are off
	 */
	SleepmodeDeepSleep
}
FoxState_SleepModeEnum;

/**
 * Antenna matching status
 */
typedef enum
{
	/**
	 * Matching never was initiated
	 */
	AntennaMatching_NeverInitiated = 0,

	/**
	 * Matching currently in progress
	 */
	AntennaMatching_InProgress = 1,

	/**
	 * Matching completed
	 */
	AntennaMatching_Completed = 2
}
FoxState_AntennaMatchingStatus;

/**
 * Fox frequency
 */
typedef struct
{
	/**
	 * If true, then 144MHz transmitter will be used.
	 */
	volatile bool Is144MHz;

	/**
	 * Desired frequency in hertzs.
	 */
	volatile uint32_t FrequencyHz;
}
FoxFrequencyStruct;

/**
 * Cycle settings
 */
typedef struct
{
	/**
	 * Is continuous transmission?
	 */
	volatile bool IsContinuous;

	/**
	 * Transmission length.
	 */
	volatile uint32_t TxTime;

	/**
	 * Pause length.
	 */
	volatile uint32_t PauseTime;
}
FoxCycleStruct;

/**
 * Global (i.e. longer than cycle) fox state
 */
typedef struct
{
	/**
	 * If true, then fox will start at start time
	 */
	bool IsArmed;

	/**
	 * Timespan, representing fox start time. This field is displayed on the
	 * fox screen / phone
	 */
	uint32_t StartTimespan;

	/**
	 * Timespan, representing fox finish time. This field is displayed on the
	 * fox screen / phone
	 */
	uint32_t EndTimespan;

	/**
	 * When fox will turn on (date and time).
	 */
	uint32_t StartDateTime;

	/**
	 * When fox will turn off (date and time).
	 */
	uint32_t EndDateTime;

	/**
	 * Current global fox state.
	 */
	GlobalFoxStateEnum CurrentState;

	/**
	 * When state will be changed to next state.
	 */
	uint32_t StateChangeTime;
}
GlobalFoxStateStruct;

typedef struct
{
	/**
	 * Current cycle state.
	 */
	CycleStateEnum CycleState;

	/**
	 * When state will be changed to the next one.
	 */
	uint32_t StateChangeTime;

	/**
	 * True, if ending tone is being transmitted currently.
	 */
	bool IsEndingTone;
}
CycleStateStruct;

/**
 * Stuff, related to sleep modes
 */
typedef struct
{
	/**
	 * Seconds before entering deeper sleep mode
	 */
	int16_t SleepTimer;

	/**
	 * Current sleep mode
	 */
	FoxState_SleepModeEnum Mode;
}
SleepmodesStruct;

/**
 * Global structure with antenna matching status
 */
typedef struct
{
	/**
	 * Matcher status
	 */
	FoxState_AntennaMatchingStatus Status;

	/**
	 * True if matching data is new for app
	 */
	bool IsNewForApp;

	/**
	 * Current matcher position
	 */
	uint8_t CurrentPosition;

	/**
	 * Current antenna voltage
	 */
	float CurrentVoltage;

	/**
	 * Best match position (will move during matching)
	 */
	uint8_t BestMatchPosition;

	/**
	 * Antenna voltage at BestMatchPosition
	 */
	float BestMatchVoltage;

	/**
	 * Matching voltages for last matching
	 */
	float MatchingVoltages[YHL_MATCHING_LEVELS_COUNT];
}
MatchingStatusStruct;

/**
 * Structure with various service settings
 */
typedef struct
{
	/**
	 * When executing a "Set Ubatt(ADC) to Ubatt(Volts) factors" command new A Factor will be stored here
	 */
	float SetThisUbattADCToUbattVoltsAFactor;

	/**
	 * When executing a "Set Ubatt(ADC) to Ubatt(Volts) factors" command new B Factor will be stored here
	 */
	float SetThisUbattADCToUbattVoltsBFactor;

	/**
	 * When executing a "Set Ubatt(Volts) to battery level factors" command new A Factor will be stored here
	 */
	float SetThisUbattVoltsToBattLevelAFactor;

	/**
	 * When executing a "Set Ubatt(Volts) to battery level factors" command new B Factor will be stored here
	 */
	float SetThisUbattVoltsToBattLevelBFactor;

	/**
	 * When executing a "Set U80m(ADC) to U80m(Volts) factors" command new A Factor will be stored here
	 */
	float SetThisU80mADCtoU80mVoltsAFactor;

	/**
	 * When executing a "Set U80m(ADC) to U80m(Volts) factors" command new B Factor will be stored here
	 */
	float SetThisU80mADCtoU80mVoltsBFactor;

	/**
	 * When executing a "Set P80m to U80m factors" command new A Factor will be stored here
	 */
	float SetThisP80mToU80mAFactor;

	/**
	 * When executing a "Set P80m to U80m factors" command new B Factor will be stored here
	 */
	float SetThisP80mToU80mBFactor;

	/**
	 * When executing a "Set Uant(ADC) to Uant(Volts) factors" command new A Factor will be stored here
	 */
	float SetThisUantADCToUantVoltsAFactor;

	/**
	 * When executing a "Set Uant(ADC) to Uant(Volts) factors" command new B Factor will be stored here
	 */
	float SetThisUantADCToUantVoltsBFactor;

	/**
	 * Flag, raised by service command, causing fox to transmit continuously
	 */
	bool IsForceTx;
}
ServiceSettingsStruct;

/**
 * Fox status
 */
typedef struct
{
	/**
	 * Battery level [0-1]
	 */
	float BatteryLevel;

	/**
	 * Current time (as timestamp)
	 */
	uint32_t CurrentTime;

	/**
	 * True if transmitter must be on by morse player.
	 */
	bool IsMorseTx;

	/**
	 * True if transmitter is on.
	 */
	bool IsTXOn;

	/**
	 * Fox name.
	 */
	char Name[YHL_FOX_NAME_BUFFER_LENGTH];

	/**
	 * Fox frequency
	 */
	FoxFrequencyStruct Frequency;

	/**
	 * Fox code.
	 */
	FoxCodeEnum Code;

	/**
	 * Is fast fox?
	 */
	bool IsFast;

	/**
	 * Fox cycle settings.
	 */
	FoxCycleStruct Cycle;

	/**
	 * Length of ending tone in seconds.
	 */
	uint8_t EndingToneLength;

	/**
	 * Global (long-time) fox state.
	 */
	GlobalFoxStateStruct GlobalState;

	/**
	 * Cycle state.
	 */
	CycleStateStruct CycleState;

	/**
	 * Fox power in watts, applicable only for 3.5MHz mode
	 */
	float Power;

	/**
	 * Force constant carrier on (for antenna matching)
	 */
	bool ForceCarrierOn;

	/**
	 * Supress carrier (for antenna matching)
	 */
	bool ForceCarrierOff;

	/**
	 * Current fox display
	 */
	FoxDisplayEnum CurrentDisplay;

	/**
	 * True if EEPROM headers is initialized and self-diagnostics can write data to EEPROM
	 */
	bool IsEEPROMHeadersInitialized;

	/**
	 * Disable all drawing operations if set to true. Set it to true when display is disabled.
	 */
	bool SupressDrawing;

	/**
	 * Stuff, related to sleepmodes
	 */
	SleepmodesStruct Sleepmodes;

	/**
	 * Stuff, related to antenna matching
	 */
	MatchingStatusStruct AntennaMatching;

	/**
	 * If not reported to phone manual profile changes exist
	 */
	bool IsNotReportedManualProfileChanges;

	/**
	 * Stuff, related to service settings
	 */
	ServiceSettingsStruct ServiceSettings;
}
FoxStateStruct;

/**
 * Flags for hardware controls, needed to switch context from interrupts to main loop
 */
typedef struct
{
	bool IsLeftButtonPressed;
	bool IsRightButtonPressed;
	bool IsEncoderButtonPressed;
	int8_t EncoderRotation;
}
HardwareControlsEventsStruct;

#endif /* INCLUDE_FOXSTATUSPRIVATE_H_ */
