/*
 * SelfDiagnostics.h
 *
 *  Created on: Oct 31, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_SELFDIAGNOSTICS_H_
#define INCLUDE_SELFDIAGNOSTICS_H_

#include <l2hal.h>

/**
 * Possible failure causes
 */
typedef enum
{
	/**
	 * No failure
	 */
	YhlFailureCause_OK = 0,

	/**
	 * Generic failure. Avoid to use it
	 */
	YhlFailureCause_Generic = 1
}
YhlFailureCausesEnum;

/**
 * Log failure cause into EEPROM, shutdown most of the hardware and halt.
 */
void SelfDiagnostics_HaltOnFailure(YhlFailureCausesEnum failureCause);


#endif /* INCLUDE_SELFDIAGNOSTICS_H_ */
