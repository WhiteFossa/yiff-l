/*
 * SelfDiagnostics.h
 *
 *  Created on: Oct 31, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_SELFDIAGNOSTICS_H_
#define INCLUDE_SELFDIAGNOSTICS_H_

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



#endif /* INCLUDE_SELFDIAGNOSTICS_H_ */
