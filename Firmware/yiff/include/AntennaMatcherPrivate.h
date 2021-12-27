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
 * Antenna matching value at which most powerful signal is generated
 */
uint8_t AMSM_OptimalAntennaMatching;

/**
 * Current matcher position
 */
uint8_t AMSM_CurrentMatcherPosition;

/**
 * Current maximal antenna voltage
 */
float AMSM_CurrentMaximalAntennaVoltage;

/**
 * Multipurpose timer variable
 */
uint16_t AMSM_Timer;

/**
 * Do we need to arm fox after antenna matching?
 */
bool AMSM_IsArmFoxAfterMatching;

/**
 * Supress transmitter carrier
 */
void AMSM_SupressCarrier(bool isSupress);

/**
 * Force transmitter carrier
 */
void AMSM_ForceCarrier(bool isForce);

#endif /* INCLUDE_ANTENNAMATCHERPRIVATE_H_ */
