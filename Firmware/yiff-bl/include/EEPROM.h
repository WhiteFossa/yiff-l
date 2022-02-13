/*
 * EEPROM.h
 *
 *  Created on: Feb 13, 2022
 *      Author: fossa
 */

#ifndef INCLUDE_EEPROM_H_
#define INCLUDE_EEPROM_H_

#include <stdbool.h>
#include <stdint.h>
#include <l2hal_crc.h>
#include <l2hal_24x.h>

extern L2HAL_CRCContextStruct CRC_Context;
extern L2HAL_24x_ContextStruct EEPROMContext;

/**
 * EEPROM-related stuff
 */
#define YBL_EEPROM_ADDRESS 0xA0
#define YBL_EEPROM_PAGE_SIZE 64

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
	 * If true, then on reset we need to enter bootloader
	 */
	volatile bool IsEnterBootloader;

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
 * Read constant EEPROM header (it always go from 0x00 address in EEPROM)
 */
void EEPROM_ReadConstantHeader(EEPROMConstantHeaderStruct* constantHeader);

/**
 * Check EEPROM constant header CRC.
 */
bool EEPROM_CheckConstantHeader(EEPROMConstantHeaderStruct* constantHeader);

#endif /* INCLUDE_EEPROM_H_ */
