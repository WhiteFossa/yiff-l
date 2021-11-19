/*
 * FoxStatus.h
 *
 *  Created on: Apr 7, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_FOXSTATUS_H_
#define INCLUDE_FOXSTATUS_H_G

#include <stdint.h>
#include <l2hal_aux.h>
#include <FoxStatusPrivate.h>
#include <Time.h>
#include <Commands.h>

extern PendingCommandsFlagsStruct PendingCommandsFlags;
extern RTC_TimeTypeDef CurrentTime;
extern RTC_DateTypeDef CurrentDate;

/**
 * Allowed frequencies
 */
#define YHL_MIN_80M_FREQUENCY 3500000U
#define YHL_MAX_80M_FREQUENCY 3650000U
#define YHL_MIN_2M_FREQUENCY 144000000U
#define YHL_MAX_2M_FREQUENCY 146000000U

/**
 * Default frequencies (for new profile and after range switch)
 */
#define YHL_DEFAULT_80M_FREQUENCY 3575000U
#define YHL_DEFAULT_144M_FREQUENCY 145000000U

/**
 * Minimal allowed cyclic duration
 */
#define YHL_MIN_TX_DURATION 6U
#define YHL_MIN_PAUSE_DURATION 6U

/**
 * Maximal allowed ending tone
 */
#define YHL_MAX_ENDINGTONE_DURATION 5U

/**
 * Allowed powers (for 3.5MHz)
 */
#define YHL_MIN_POWER_80M 2.0f
#define YHL_MAX_POWER_80M 5.0f


/**
 * Call this before any operations with fox state
 */
void FoxState_Init(void);

/**
 * Call this after updating RTC date and time
 */
void FoxState_CorrectDateTime(void);

/**
 * Checks if given frequency valid or not
 */
bool FoxState_IsFrequencyValid(bool is144MHz, uint32_t frequency);

/**
 * Tries to set frequency, if frequency correct - sets it and returns true, otherwise false
 */
bool FoxState_SetFrequency(bool is144MHz, uint32_t frequency);

/**
 * Checks if given cycle durations are valid
 */
bool FoxState_IsCycleDurationsValid(uint32_t txTime, uint32_t pauseTime);

/**
 * Tries to set cycle durations. If durations are valid then sets it and returns true, otherwise false
 */
bool FoxState_SetCycleDurations(uint32_t txTime, uint32_t pauseTime);

/**
 * Returns true if ending tone duration is valid
 */
bool FoxState_IsEndingtoneDurationValid(uint8_t endingtoneDuration);

/**
 * Tries to set ending tone duration. If duration valid, sets it and returns true, otherwise false
 */
bool FoxState_SetEndingtoneDuration(uint8_t endingtoneDuration);

/**
 * Returns true if transmission begin and end times are valid
 */
bool FoxState_IsBeginAndEndTimesValid(uint32_t beginTime, uint32_t endTime);

/**
 * Tries to send transmission begin/end times, if they are valid sets it and returns true, otherwise false
 */
bool FoxState_SetBeginAndEndTimes(uint32_t beginTime, uint32_t endTime);

/**
 * Checks if requested power is valid for current fox mode. If valid returns true
 */
bool FoxState_IsPowerValid(float power);

/**
 * Tries to set power, if it is valid - sets it and returns true, otherwise false
 */
bool FoxState_SetPower(float power);

#endif /* INCLUDE_FOXSTATUS_H_ */
