/*
 * EEPROM.h
 *
 *  Created on: Apr 21, 2021
 *      Author: fossa
 */

#include <stdint.h>

#ifndef INCLUDE_EEPROM_H_
#define INCLUDE_EEPROM_H_

#define YHL_EEPROM_DATA_VERSION 1U

/**
 * How much profiles can we have
 */
#define YHL_MAX_PROFILES_COUNT 128U

/**
 * EEPROM header (placed at EEPROM starting 1st byte
 */
typedef struct
{
	/**
	 * EEPROM version, bump each time when structure changes
	 */
	uint16_t EEPROMVersion;

	/**
	 * How many profiles do we have
	 */
	uint8_t NumberOfProfiles;

	/**
	 * Start addresses of each profile
	 */
	uint16_t ProfilesAddresses[YHL_MAX_PROFILES_COUNT];

	/**
	 * Profile in use (index for ProfilesAddresses)
	 */
	uint8_t ProfileInUse;

	/**
	 * CRC
	 */
	uint32_t CRCSum;
}
EEPROMHeaderStruct;

/**
 * One profile.
 */
typedef struct
{

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
	 * Fox power in watts, applicable only for 3.5MHz mode
	 */
	float Power;

	/**
	 * Is fox armed to start at given time
	 */
	bool IsFoxArmed;

	/**
	 * Fox will start at given time if armed.
	 */
	Time StartTime;

	/**
	 * Fox will turn off at given time if armed.
	 */
	Time EndTime;

	/**
	 * CRC
	 */
	uint32_t CRCSum;

}
EEPROMProfileStruct;


/**
 * Initialize EEPROM (call this if EEPROM Header CRC don't match).
 */
void EEPROM_Format(void);

/**
 * Write EEPROM header (it always go from 0x00 address in EEPROM)
 */
void EEPROM_WriteHeader(EEPROMHeaderStruct* header);

/**
 * Read EERPOM header
 */
void EEPROM_ReadHeader(EEPROMHeaderStruct* header);

/**
 * Check EEPROM header for CRC correctness. Returns true if CRC match.
 */
bool EEPROM_CheckHeader(EEPROMHeaderStruct* header);

#endif /* INCLUDE_EEPROM_H_ */
