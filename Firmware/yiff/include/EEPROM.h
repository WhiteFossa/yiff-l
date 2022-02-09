/*
 * EEPROM.h
 *
 *  Created on: Apr 21, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_EEPROM_H_
#define INCLUDE_EEPROM_H_

#include <stdint.h>
#include <FoxStatus.h>
#include <l2hal.h>
#include <SelfDiagnostics.h>
#include <Random.h>
#include <Version.h>

extern L2HAL_CRCContextStruct CRC_Context;
extern FoxStateStruct FoxState;
extern L2HAL_24x_ContextStruct EEPROMContext;

#define YHL_EEPROM_DATA_VERSION 1U

/**
 * Postfix = HAL_GetTick() % REMAINDER
 */
#define YHL_EEPROM_FOX_NAME_POSTFIX_REMAINDER 10000

/**
 * How much profiles can we have
 */
#define YHL_MAX_PROFILES_COUNT 128U

/**
 * Max profile name length
 */
#define YHL_PROFILE_NAME_MAX_LENGTH 16U

/**
 * Profile name memory size
 */
#define YHL_PROFILE_NAME_MEMORY_SIZE (YHL_PROFILE_NAME_MAX_LENGTH + 1U)

/**
 * Newly-generated profiles have this name
 */
#define YHL_DEFAULT_PROFILE_NAME "New profile"

/**
 * Fox name postfix modulus. 10000 mean postfixes from 0000 to 9999
 */
#define YHL_DEFAULT_FOX_NAME_POSTFIX_MODULUS 10000U

/**
 * Default fox name template
 */
#define YHL_DEFAULT_FOX_NAME_TEMPLATE "YiffyFoxy%04d"

/**
 * Default factors for UBatt(ADC)->Volts conversion
 */
#define YHL_DEFAULT_ADC_UBATT_A 0.002448f;
#define YHL_DEFAULT_ADC_UBATT_B 0.3647f

/**
 * Default factors for UBatt(Volts)->Battery level conversion
 */
#define YHL_DEFAULT_BATTERY_LEVEL_A 0.4167f
#define YHL_DEFAULT_BATTERY_LEVEL_B -2.5002f

/**
 * Default factors for U80m(ADC)->Volts conversion
 */
#define YHL_DEFAULT_ADC_U80M_A 0.006281f
#define YHL_DEFAULT_ADC_U80M_B 0.4150f

/**
 * Default factors for U80m(volts) to P80m conversion
 */
#define YHL_DEFAULT_P80M_A 3.440f
#define YHL_DEFAULT_P80M_B 2.952f

/**
 * Default factors for Uant(ADC)->Volts conversion
 */
#define YHL_DEFAULT_ADC_UANT_A 0.08057
#define YHL_DEFAULT_ADC_UANT_B 0

/**
 * Default value for RTC calibration
 */
#define YHL_DEFAULT_RTC_CALIBRATION_VALUE 0x3F

/**
 * Default value for disarm battery percent (see EEPROMHeaderStruct field description)
 */
#define YHL_DEFAULT_DISARM_BATTERY_PERCENT 0.01f

/**
 * Minimal possible disarm battery percent
 */
#define YHL_MIN_DISARM_BATTERY_PERCENT 0.005f

/**
 * Maximal possible disarm battery percent
 */
#define YHL_MAX_DISARM_BATTERY_PERCENT 0.2f


/**
 * EEPROM constant header. It's structure must not change from version
 * to version
 */
typedef struct
{
	/**
	 * EEPROM version, bump each time when EEPROM structure changes
	 */
	volatile uint16_t EEPROMVersion;

	/**
	 * Hardware revision, needed by the bootloader
	 */
	volatile uint16_t HardwareRevision;

	/**
	 * Firmware version, needed by the bootloader
	 */
	volatile uint16_t FirmwareVersion;

	/**
	 * Absolute address of main header
	 */
	volatile uint16_t HeaderAddress;

	/**
	 * CRC
	 */
	volatile uint32_t CRCSum;
}
__attribute__((packed))
EEPROMConstantHeaderStruct;


/**
 * EEPROM header
 */
typedef struct
{
	/**
	 * Fox name
	 */
	volatile char Name[YHL_FOX_NAME_BUFFER_LENGTH];

	/**
	 * How many profiles do we have
	 */
	volatile uint8_t NumberOfProfiles;

	/**
	 * Start addresses of each profile
	 */
	volatile uint16_t ProfilesAddresses[YHL_MAX_PROFILES_COUNT];

	/**
	 * Profile in use (index for ProfilesAddresses)
	 */
	volatile uint8_t ProfileInUse;

	/**
	 * Factors to convert ADC measurements into volts
	 */
	volatile float UBattADCA;
	volatile float UBattADCB;

	/**
	 * Factors to convert Ubatt(Volts) to battery level
	 */
	volatile float BattLevelA;
	volatile float BattLevelB;

	/**
	 * Factors to calculate U80m(ADC) -> U80m(Volts)
	 */
	volatile float U80mADCA;
	volatile float U80mADCB;

	/**
	 * Factors to calculate P80m from U80m
	 */
	volatile float P80mA;
	volatile float P80mB;

	/**
	 * Factors to calculate Uant(volts) from Uant(ADC)
	 */
	volatile float UAntADCA;
	volatile float UAntADCB;

	/**
	 * Code of last failure
	 */
	volatile YhlFailureCausesEnum LastFailure;

	/**
	 * Device serial number
	 */
	volatile uint32_t SerialNumber;

	/**
	 * RTC calibration value
	 */
	volatile uint8_t RTCCalibrationValue;

	/**
	 * If battery charge percent is this value or less, then disarm the fox.
	 * The value must be [0; 1]
	 */
	volatile float DisarmBatteryPercent;

	/**
	 * CRC
	 */
	volatile uint32_t CRCSum;
}
__attribute__((packed))
EEPROMHeaderStruct;

/**
 * One profile.
 */
typedef struct
{
	/**
	 * Profile name.
	 */
	volatile char Name[YHL_PROFILE_NAME_MEMORY_SIZE];

	/**
	 * Fox frequency
	 */
	volatile FoxFrequencyStruct Frequency;

	/**
	 * Fox code.
	 */
	volatile FoxCodeEnum Code;

	/**
	 * Is fast fox?
	 */
	volatile bool IsFast;

	/**
	 * Fox cycle settings.
	 */
	volatile FoxCycleStruct Cycle;

	/**
	 * Length of ending tone in seconds.
	 */
	volatile uint8_t EndingToneLength;

	/**
	 * Fox power in watts, applicable only for 3.5MHz mode
	 */
	volatile float Power;

	/**
	 * Fox start timespan
	 */
	volatile uint32_t StartTimespan;

	/**
	 * Fox end timespan
	 */
	volatile uint32_t EndTimespan;

	/**
	 * CRC
	 */
	volatile uint32_t CRCSum;
}
__attribute__((packed))
EEPROMProfileStruct;

extern EEPROMConstantHeaderStruct EEPROM_ConstantHeader;
extern EEPROMHeaderStruct EEPROM_Header;
extern EEPROMProfileStruct EEPROM_CurrentProfile;

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
 * Load given profile into a fox state. Armed fox is disarmed before load.
 */
void EEPROM_LoadProfileIntoFoxState(FoxStateStruct* foxState, EEPROMProfileStruct* profile);

/**
 * Updates EEPROM header from EEPROM_Header global variable.
 */
void EEPROM_UpdateHeader(void);

/**
 * Switch to given profile (if it's not active already).
 */
void EEPROM_SwitchProfile(uint8_t profileId);

/**
 * Generates new default profile and adds it to the end of profiles list.
 */
void EEPROM_AddProfile(void);

/**
 * Get profile with given number
 */
EEPROMProfileStruct EEPROM_GetProfile(uint8_t profileId);

/**
 * Update profile with given ID by given data. If this is current profile, then reloads it into fox state.
 */
void EEPROM_UpdateProfile(EEPROMProfileStruct* profile, uint8_t profileId);

/**
 * Checks, if given profile ID valid or not
 */
bool EEPROM_IsProfileIdValid(uint8_t profileId);

/**
 * Save current profile to EEPROM
 */
void EEPROM_UpdateCurrentProfile(void);

/**
 * Called when firmware version is changed
 */
void EEPROM_OnSoftwareUpdate(void);

#endif /* INCLUDE_EEPROM_H_ */
