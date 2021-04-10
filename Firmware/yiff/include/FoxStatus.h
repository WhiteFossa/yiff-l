/*
 * FoxStatus.h
 *
 *  Created on: Apr 7, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_FOXSTATUS_H_
#define INCLUDE_FOXSTATUS_H_

#define YHL_FOX_NAME_BUFFER_LENGTH 33

typedef struct
{
	uint8_t Hours;
	uint8_t Minutes;
	uint8_t Seconds;
}
Time;

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
	Finish, /* MO */
	F1, /* MOE */
	F2, /* MOI */
	F3, /* MOS */
	F4, /* MOH */
	F5, /* MO5 */
	Beacon /* S */
}
FoxCodeEnum;

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
}
FoxStateStruct;

/**
 * Fox state
 */
FoxStateStruct FoxState;


#endif /* INCLUDE_FOXSTATUS_H_ */
