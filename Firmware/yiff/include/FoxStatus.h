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
	Time TxTime;

	/**
	 * Pause length.
	 */
	Time PauseTime;
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
	Standby,

	/**
	 * Fox is programmed, on countdown before start.
	 */
	BeforeStart,

	/**
	 * Fox is running, on countdown before finish.
	 */
	BeforeFinish
}
GlobalFoxStateEnum;

/**
 * Global (i.e. longer than cycle) fox state
 */
typedef struct
{
	/**
	 * When fox will turn on.
	 */
	Time StartTime;

	/**
	 * When fox will turn off.
	 */
	Time EndTime;

	/**
	 * Current global fox state.
	 */
	GlobalFoxStateEnum CurrentState;

	/**
	 * When state will be changed to next state.
	 */
	Time StateChangeTime;
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
	Tx,

	/**
	 * Transmitting end tone
	 */
	EndingTone,

	/**
	 * Pause
	 */
	Pause
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
	Time StateChangeTime;

	/**
	 * True, if ending tone is being transmitted currently.
	 */
	bool IsEndingTone;
}
CycleStateStruct;

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
	 * Current time without day
	 */
	Time CurrentTime;

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
}
FoxStateStruct;

/**
 * Call this before any operations with fox state
 */
void FoxState_Init(void);

/**
 * Call this after updating RTC date and time
 */
void FoxState_CorrectDateTime(void);


#endif /* INCLUDE_FOXSTATUS_H_ */
