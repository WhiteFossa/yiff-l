/*
 * l2hal_adf4001_defines.h
 *
 *  Created on: Dec 6, 2020
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_DEFINES_H_
#define L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_DEFINES_H_

/**
 * Various bits defines for ADF4001
 */

/**
 * Command bits (define what register is addressed)
 */
#define L2HAL_ADF4001_CONTROLBITS_R 0b00
#define L2HAL_ADF4001_CONTROLBITS_N 0b01
#define L2HAL_ADF4001_CONTROLBITS_FUNCTION 0b10
#define L2HAL_ADF4001_CONTROLBITS_INITIALIZATION 0b11

/**
 * Antibacklash pulse width (in nanoseconds).
 */
enum L2HAL_ADF4001_AntiBacklashWidth
{
	ABWPulseWidth2_9,
	ABWPulseWidth1_3,
	ABWPulseWidth6_0,
};

/**
 * Positions of antibacklash width bits in most significant byte
 */
#define L2HAL_ADF4001_ANTIBACKLASH0 0
#define L2HAL_ADF4001_ANTIBACKLASH1 1


/**
 * Lock detect precision (in cycles)
 */
enum L2HAL_ADF4001_LockDetectPrecision
{
	LDPCycles3,
	LDPCycles5
};

/**
 * Position of lock detect precision bit in most significant byte
 */
#define L2HAL_ADF4001_LOCK_DETECT_PRECISION 4

/**
 * What charge pump current setting to use.return toWrite.Most;
 */
enum L2HAL_ADF4001_CPGain
{
	CPGain1,
	CPGain2
};

/**
 * Position of CP Gain bit in most significant byte
 */
#define L2HAL_ADF4001_CP_GAIN 5

/**
 * What to do with counters
 */
enum L2HAL_ADF4001_CountersReset
{
	CRNormal,
	CRHoldInReset
};

#define L2HAL_ADF4001_HOLD_IN_RESET 2

/**
 * Power down mode
 */
enum L2HAL_ADF4001_PowerDownMode
{
	PDNNormal,
	PDNAsynchronousPowerDown,
	PDNSynchronousPowerDown
};

#define L2HAL_ADF4001_POWER_DOWN1 3
#define L2HAL_ADF4001_POWER_DOWN2 5

/**
 * Signal at MUXOUT pin
 */
enum L2HAL_ADF4001_MuxoutControl
{
	MCThreeState,
	MCDigitalLockDetect,
	MCNDivider,
	MCAVDD,
	MCRDivider,
	MCOpenDrainLockDetect,
	MCSerialDataOutput,
	MCDGND
};

#define L2HAL_ADF4001_MUXOUT1 4
#define L2HAL_ADF4001_MUXOUT2 5
#define L2HAL_ADF4001_MUXOUT3 6

/**
 * Phase detector polarity
 */
enum L2HAL_ADF4001_PhaseDetectorPolarity
{
	PDPNegative,
	PDPPositive
};

#define L2HAL_ADF4001_PHASE_DETECTOR_POLARITY 7

/**
 * Charge pump state
 */
enum L2HAL_ADF4001_CPThreeState
{
	CPNormal,
	CPThreeState
};

#define L2HAL_ADF4001_CHARGE_PUMP_THREESTATE 0

/**
 * Fastlock enable
 */
enum L2HAL_ADF4001_FastlockEnable
{
	FLDisabled,
	FLEnabled
};

#define L2HAL_ADF4001_FASTLOCK_ENABLE 1

/**
 * Fastlock mode
 */
enum L2HAL_ADF4001_FastlockMode
{
	FLMode1,
	FLMode2
};

#define L2HAL_ADF4001_FASTLOCK_MODE 2

/**
 * Time between switching current settings (in phase detector cycles)
 */
enum L2HAL_ADF4001_TimerCounterControl
{
	TCC_3,
	TCC_7,
	TCC_11,
	TCC_15,
	TCC_19,
	TCC_23,
	TCC_27,
	TCC_31,
	TCC_35,
	TCC_39,
	TCC_43,
	TCC_47,
	TCC_51,
	TCC_55,
	TCC_59,
	TCC_63
};

#define L2HAL_ADF4001_TCC0 3
#define L2HAL_ADF4001_TCC1 4
#define L2HAL_ADF4001_TCC2 5
#define L2HAL_ADF4001_TCC3 6

/**
 * CP current (for 4.7k resistor) im mA
 */
enum L2HAL_ADF4001_CurrentSetting
{
	CS0_625,
	CS1_25,
	CS1_875,
	CS2_5,
	CS3_125,
	CS3_75,
	CS4_375,
	CS5_0
};

#define L2HAL_ADF4001_CURRENT_SETTING0 0
#define L2HAL_ADF4001_CURRENT_SETTING1 1
#define L2HAL_ADF4001_CURRENT_SETTING2 2

#endif /* L2HAL_DRIVERS_PLL_ADF4001_INCLUDE_L2HAL_ADF4001_DEFINES_H_ */
