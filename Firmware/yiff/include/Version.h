/*
 * Version.h
 *
 *  Created on: Nov 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_VERSION_H_
#define INCLUDE_VERSION_H_

/**
 * Report this signature to indicate that this device is a Fossa's Fox
 */
#define YHL_VER_FOX_SIGNATURE 0xF055AF06U

/**
 * Fox communication protocol version
 */
#define YHL_VER_PROTOCOL_VERSION 1U

/**
 * Hardware revision, for what this firmware is designed
 */
#define YHL_VER_HARDWARE_REVISION 1U

/**
 * Firmware version
 */
#define YHL_VER_SOFTWARE_VERSION 2U

/**
 * Default serial number for newly-formatted EERPOM. 0 means "need to be set"
 */
#define YHL_VER_DEFAULT_SERIAL_NUMBER 0U


#endif /* INCLUDE_VERSION_H_ */
