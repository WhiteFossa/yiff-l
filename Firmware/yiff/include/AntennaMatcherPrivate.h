/*
 * AntennaMatcherPrivate.h
 *
 *  Created on: Dec 27, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_ANTENNAMATCHERPRIVATE_H_
#define INCLUDE_ANTENNAMATCHERPRIVATE_H_

#include <AntennaMatcher.h>

/**
 * Current state of antenna matcher
 */
AntennaMatcherStateEnum AMSM_State;

/**
 * Multipurpose timer variable
 */
uint16_t AMSM_Timer;

/**
 * Do we need to arm fox after antenna matching?
 */
bool AMSM_IsArmFoxAfterMatching;

/**
 * Do we need to force TX after matching?
 */
bool AMSM_IsForceTxAfterMatching;

/**
 * Supress transmitter carrier
 */
void AMSM_SupressCarrier(bool isSupress);

/**
 * Force transmitter carrier
 */
void AMSM_ForceCarrier(bool isForce);

/**
 * Reset matcher values
 */
void AMSM_ResetValues(void);

#endif /* INCLUDE_ANTENNAMATCHERPRIVATE_H_ */
