/*
 * FoxStatusPrivate.h
 *
 *  Created on: Oct 9, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_FOXSTATUSPRIVATE_H_
#define INCLUDE_FOXSTATUSPRIVATE_H_

#include <stdint.h>
#include <l2hal_aux.h>

#define YHL_FOX_NAME_BUFFER_LENGTH 33

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
 * Fox frequency
 */
typedef struct
{
	/**
	 * If true, then 144MHz transmitter will be used.
	 */
	bool Is144MHz;

	/**
	 * Desired frequency in hertzs.
	 */
	uint32_t FrequencyHz;
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
	bool IsContinuous;

	/**
	 * Transmission length.
	 */
	uint32_t TxTime;

	/**
	 * Pause length.
	 */
	uint32_t PauseTime;
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
	 * When fox will turn on.
	 */
	uint32_t StartTime;

	/**
	 * When fox will turn off.
	 */
	uint32_t EndTime;

	/**
	 * Current global fox state.
	 */
	GlobalFoxStateEnum CurrentState;

	/**
	 * When state will be changed to next state.
	 */
	uint32_t StateChangeTime;

	/**
	 * True if antenna matching in progress
	 */
	bool IsMatchingInProgress;
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
 * Matching display data. Meaningless if Globalstate.IsMatchingInProgress == false
 */
typedef struct
{
	/**
	 * Matching step
	 */
	uint8_t MatchingStep;

	/**
	 * Matching levels. Allocated only when matching in progress,
	 * contains HAL_AM_MAX_VALUE + 1 elements.
	 */
	float* MatchingLevels;
}
MatchingDisplayStruct;

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
	 * Data for matching display
	 */
	MatchingDisplayStruct MatchingDisplayData;
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
