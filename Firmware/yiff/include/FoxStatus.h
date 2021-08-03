/*
 * FoxStatus.h
 *
 *  Created on: Apr 7, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_FOXSTATUS_H_
#define INCLUDE_FOXSTATUS_H_G

#include <Time.h>
#include <Commands.h>

extern PendingCommandsFlagsStruct PendingCommandsFlags;

#define YHL_FOX_NAME_BUFFER_LENGTH 33

/**
 * Allowed frequencies
 */
#define YHL_MIN_80M_FREQUENCY 3500000U
#define YHL_MAX_80M_FREQUENCY 3650000U
#define YHL_MIN_2M_FREQUENCY 144000000U
#define YHL_MAX_2M_FREQUENCY 146000000U

/**
 * Minimal allowed cyclic duration
 */
#define YHL_MIN_TX_DURATION 6U
#define YHL_MIN_PAUSE_DURATION 6U

/**
 * Maximal allowed ending tone
 */
#define YHL_MAX_ENDINGTONE_DURATION 5U

/**
 * Allowed powers (for 3.5MHz)
 */
#define YHL_MIN_POWER_80M 2.0f
#define YHL_MAX_POWER_80M 5.0f


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
	StatusDisplay = 0,

	/**
	 * Antenna matching display (3.5MHz)
	 */
	AntennaMatchingDisplay = 1
}
FoxDisplayEnum;

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
}
GlobalFoxStateStruct;

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

#define YHL_MATCHING_LEVELS_DATA_SIZE (HAL_AM_MAX_VALUE + 1) * sizeof(float)

/**
 * Matching display data
 */
typedef struct
{
	/**
	 * Other fields is meaningless if false
	 */
	bool IsMatchingInProgress;

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

/**
 * Call this before any operations with fox state
 */
void FoxState_Init(void);

/**
 * Call this after updating RTC date and time
 */
void FoxState_CorrectDateTime(void);

/**
 * Checks if given frequency valid or not
 */
bool FoxState_IsFrequencyValid(bool is144MHz, uint32_t frequency);

/**
 * Tries to set frequency, if frequency correct - sets it and returns true, otherwise false
 */
bool FoxState_SetFrequency(bool is144MHz, uint32_t frequency);

/**
 * Checks if given cycle durations are valid
 */
bool FoxState_IsCycleDurationsValid(uint32_t txTime, uint32_t pauseTime);

/**
 * Tries to set cycle durations. If durations are valid then sets it and returns true, otherwise false
 */
bool FoxState_SetCycleDurations(uint32_t txTime, uint32_t pauseTime);

/**
 * Returns true if ending tone duration is valid
 */
bool FoxState_IsEndingtoneDurationValid(uint8_t endingtoneDuration);

/**
 * Tries to set ending tone duration. If duration valid, sets it and returns true, otherwise false
 */
bool FoxState_SetEndingtoneDuration(uint8_t endingtoneDuration);

/**
 * Returns true if transmission begin and end times are valid
 */
bool FoxState_IsBeginAndEndTimesValid(uint32_t beginTime, uint32_t endTime);

/**
 * Tries to send transmission begin/end times, if they are valid sets it and returns true, otherwise false
 */
bool FoxState_SetBeginAndEndTimes(uint32_t beginTime, uint32_t endTime);

/**
 * Checks if requested power is valid for current fox mode. If valid returns true
 */
bool FoxState_IsPowerValid(float power);

/**
 * Tries to set power, if it is valid - sets it and returns true, otherwise false
 */
bool FoxState_SetPower(float power);

#endif /* INCLUDE_FOXSTATUS_H_ */
