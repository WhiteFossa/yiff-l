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
 * Max profile name length
 */
#define YHL_MAX_PROFILE_NAME_LENGTH 17U

/**
 * Newly-generated profiles have this name
 */
#define YHL_DEFAULT_PROFILE_NAME "New profile"

/**
 * Fox will have this name after EEPROM formatting
 */
#define YHL_DEFAULT_FOX_NAME "Yiffy foxy"

/**
 * EEPROM constant header. It's structure must not change from version
 * to version
 */
typedef struct
{
	/**
	 * EEPROM version, bump each time when EEPROM structure changes
	 */
	uint16_t Version;

	/**
	 * Absolute address of main header
	 */
	uint16_t HeaderAddress;

	/**
	 * CRC
	 */
	uint32_t CRCSum;
}
EEPROMConstantHeaderStruct;


/**
 * EEPROM header
 */
typedef struct
{
	/**
	 * Fox name
	 */
	char Name[YHL_FOX_NAME_BUFFER_LENGTH];

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
	 * Profile name.
	 */
	char Name[YHL_MAX_PROFILE_NAME_LENGTH];

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
 * EEPROM constant header in its actual state
 */
EEPROMConstantHeaderStruct EEPROM_ConstantHeader;

/**
 * EEPROM header in its actual state
 */
EEPROMHeaderStruct EEPROM_Header;

/**
 * Current profile
 */
EEPROMProfileStruct EEPROM_CurrentProfile;

/**
 * Initialize EEPROM (call this if EEPROM Header CRC don't match).
 */
void EEPROM_Format(void);

/**
 * Write constant EEPROM header (it always go from 0x00 address in EEPROM)
 */
void EEPROM_WriteConstantHeader(EEPROMConstantHeaderStruct* constantHeader);

/**
 * Read constant EEPROM header (it always go from 0x00 address in EEPROM)
 */
void EEPROM_ReadConstantHeader(EEPROMConstantHeaderStruct* constantHeader);

/**
 * Check EEPROM constant header CRC.
 */
bool EEPROM_CheckConstantHeader(EEPROMConstantHeaderStruct* constantHeader);

/**
 * Write EEPROM header
 */
void EEPROM_WriteHeader(EEPROMHeaderStruct* header, uint16_t address);

/**
 * Read EERPOM header
 */
void EEPROM_ReadHeader(EEPROMHeaderStruct* header, uint16_t address);

/**
 * Check EEPROM header for CRC correctness. Returns true if CRC match.
 */
bool EEPROM_CheckHeader(EEPROMHeaderStruct* header);

/**
 * Initializes EEPROM, formatting it if needed and loads data into global variables
 */
void EEPROM_Init(void);

/**
 * Read EEPROM headers into corresponding global variables and checks it.
 * If headers are damaged then returns false.
 */
bool EEPROM_InitHeaders(void);

/**
 * Read profile from given address into given structure
 */
void EEPROM_ReadProfile(EEPROMProfileStruct* profile, uint16_t address);

/**
 * Checks profile and returns false if CRC is incorrect.
 */
bool EEPROM_CheckProfile(EEPROMProfileStruct* profile);

/**
 * Writes given profile at given address
 */
void EEPROM_WriteProfile(EEPROMProfileStruct* profile, uint16_t address);

/**
 * Doesn't write anything to EEPROM, just generates default profile
 */
EEPROMProfileStruct EEPROM_GenerateDefaultProfile(void);

/**
 * Load given profile into a fox state. Running fox is cancelled before load.
 */
EEPROM_LoadProfileIntoFoxState(FoxStateStruct* foxState, EEPROMProfileStruct* profile);

#endif /* INCLUDE_EEPROM_H_ */
