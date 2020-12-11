/*
 * l2hal_adf4001.h
 *
 *  Created on: Dec 6, 2020
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_H_
#define L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_H_

#include <stdint.h>
#include <stm32f1xx_hal.h>
#include <l2hal_adf4001_defines.h>
#include <l2hal_adf4001_private.h>
#include <l2hal_custom.h>

/**
 * PLL context, SPI connection and various stuff is stored here
 */
typedef struct
{

}
L2HAL_ADF4001_ContextStruct;

/**
 * Fill this struct to generate reference counter latch content.
 */
typedef struct
{
	 /**
	  * Refence counter value
	  */
	uint16_t ReferenceCounter;

	/**
	 * Anti-backlash width.
	 */
	enum L2HAL_ADF4001_AntiBacklashWidth AntiBacklashWidth;

	/**
	 * Lock detect precision.
	 */
	enum L2HAL_ADF4001_LockDetectPrecision LockDetectPrecision;
}
L2HAL_ADF4001_ReferenceCounterStruct;


/**
 * Fill this struct to generate N-counter latch content.
 */
typedef struct
{
	/**
	 * N-counter value.
	 */
	uint16_t NCounter;

	/**
	 * Charge pump current setting to use.
	 */
	enum L2HAL_ADF4001_CPGain CPGain;
}
L2HAL_ADF4001_NCounterStruct;

/**
 * Fill this struct to generate functional/initialization latch content.
 */
typedef struct
{
	/**
	 * What to do with counters.
	 */
	enum L2HAL_ADF4001_CountersReset CountersReset;

	/**
	 * Power down mode.
	 */
	enum L2HAL_ADF4001_PowerDownMode PowerDownMode;

	/**
	 * MUXOUT pin signal.
	 */
	enum L2HAL_ADF4001_MuxoutControl MuxoutControl;

	/**
	 * Phase detector polarity.
	 */
	enum L2HAL_ADF4001_PhaseDetectorPolarity PhaseDetectorPolarity;

	/**
	 * Charge pump state.
	 */
	enum L2HAL_ADF4001_CPThreeState CPThreeState;

	/**
	 * Fastlock enable.
	 */
	enum L2HAL_ADF4001_FastlockEnable FastlockEnable;

	/**
	 * Fastlock mode.
	 */
	enum L2HAL_ADF4001_FastlockMode FastlockMode;

	/**
	 * Time between switching CP currents.
	 */
	enum L2HAL_ADF4001_TimerCounterControl TimerCounterControl;

	/**
	 * CP currents settings.
	 */
	enum L2HAL_ADF4001_CurrentSetting CurrentSetting1;
	enum L2HAL_ADF4001_CurrentSetting CurrentSetting2;
}
L2HAL_ADF4001_FunctionStruct;

/**
 * Write reference counter data into PLL
 */
void L2HAL_ADF4001_WriteReferenceCounter(L2HAL_ADF4001_ReferenceCounterStruct* data);

/**
 * Write N counter data into PLL
 */
void L2HAL_ADF4001_WriteNCounter(L2HAL_ADF4001_NCounterStruct* data);

/**
 * Write settings into PLL function latch.
 */
void L2HAL_ADF4001_WriteFunctionLatch(L2HAL_ADF4001_FunctionStruct* data);

/**
 * Write settings into PLL initialization latch.
 */
void L2HAL_ADF4001_WriteInitializationLatch(L2HAL_ADF4001_FunctionStruct* data);

/**
 * Private stuff going below
 */

/**
 * Function, generating everything for function/initialization latches except control bits.
 */
L2HAL_ADF4001_BytesToSendStruct L2HAL_ADF4001_PrepareFinctionAndInitializationData(L2HAL_ADF4001_FunctionStruct* data);

uint8_t L2HAL_ADF4001_GenerateCurrentSettingsByte(enum L2HAL_ADF4001_CurrentSetting current);

#endif /* L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_H_ */
