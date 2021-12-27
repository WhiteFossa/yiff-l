/*
 * SelfDiagnostics.h
 *
 *  Created on: Oct 31, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_SELFDIAGNOSTICS_H_
#define INCLUDE_SELFDIAGNOSTICS_H_

#include <l2hal.h>
#include <FoxStatus.h>

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
	YhlFailureCause_Generic = 1,

	/**
	 * Display not found
	 */
	YhlFailureCause_DisplayNotFound = 2,

	/**
	 * Bluetooth adapter not found
	 */
	YhlFailureCause_BluetoothNotFound = 3,

	/**
	 * Negative time since cycle start in cycle state machine
	 */
	YhlFailureCause_NegativeTimeSinceCycleStart = 4,

	/**
	 * Unknown cycle state in CSM_Tick()
	 */
	YhlFailureCause_UnknownCycleStateInCSMTick = 5,

	/**
	 * Unknown cycle state in CSM_RecalculateStateChangeTime()
	 */
	YhlFailureCause_UnknownCycleStateInCSMRecalculateStateChangeTime = 6,

	/**
	 * StartTime >= EndTime in GSM_Arm()
	 */
	YhlFailureCause_StartTimeGreaterOrEqualFinishTimeInGSMArm = 7,

	/**
	 * Unknown global state in GSM_Tick()
	 */
	YhlFailureCause_UnknownGlobalStateInGSMTick = 8,

	/**
	 * Failure during generic ADC setup
	 */
	YhlFailureCause_ADCGenericSetupFailure = 9,

	/**
	 * Failure to setup ADC for Uant measurement
	 */
	YhlFailureCause_ADCUantSetupFailure = 10,

	/**
	 * Failure to setup ADC for Ubatt measurement
	 */
	YhlFailureCause_ADCUbattSetupFailure = 11,

	/**
	 * Failure to setup ADC for U80m measurement
	 */
	YhlFailureCause_ADCU80mSetupFailure = 12,

	/**
	 * Unknown ADC channel in HAL_AddNewADCMeasurement()
	 */
	YhlFailureCause_UnknownADCChannelInHALAddNewADCMeasurement = 13,

	/**
	 * Failed to connect to U80m regulator
	 */
	YhlFailureCause_U80mRegulatorNotFound = 14,

	/**
	 * Failed to init tone generator timer
	 */
	YhlFailureCause_FailedToInitToneGenerator = 15,

	/**
	 * Failed to config tone generator timer channel
	 */
	YhlFailureCause_FailedToConfigToneGeneratorChannel = 16,

	/**
	 * Failed to start tone generator timer
	 */
	YhlFailureCause_FailedToStartToneGeneratorTimer = 17,

	/**
	 * Failed to stop tone generator timer
	 */
	YhlFailureCause_FailedToStopToneGeneratorTimer = 18,

	/**
	 * Incorrect antenna matching channel
	 */
	YhlFailureCause_WrongAntennaMatchingChannel = 19,

	/**
	 * Incorrect antenna matching value
	 */
	YhlFailureCause_WrongAntennaMatchingValue = 20,

	/**
	 * Incorrect target U80m
	 */
	YhlFailureCause_WrongTargetU80m = 21,

	/**
	 * Attempt to prepare fox for 3.5MHz transmission when frequency set to 144MHz
	 */
	YhlFailureCause_AttemptToPrepareFoxFor80mWhile2m = 22,

	/**
	 * Attempt to prepare fox for 144MHz transmission when frequency set to 3.5MHz
	 */
	YhlFailureCause_AttemptToPrepareFoxFor2mWhile80m = 23,

	/**
	 * Fox is already prepared for cycle in HL_PrepareFoxForCycle()
	 */
	YhlFailureCause_FoxAlreadyPreparedForCycleInHLPrepareFoxForCycle = 24,

	/**
	 * Fox is not prepared for cycle when HL_UnPrepareFoxFromCycle() call happened
	 */
	YhlFailureCause_FoxIsNotPreparedForCycleInHLUnPrepareFoxFromCycle = 25,

	/**
	 * Attempt to setup 3.5MHz antenna when fox is set up for 144MHz transmission
	 */
	YhlFailureCause_AttemptToSetup80mAntennaWhile2m = 26,

	/**
	 * Attempt to setup 3.5MHz antenna when fox is not prepared for the cycle
	 */
	YhlFailureCause_AttemptToSetup80mAntennaWhileFoxIsNotPrepared = 27,

	/**
	 * Call HL_OnU80mLock() when U80m is already locked
	 */
	YhlFailureCause_U80mAlreadyLocked = 28,

	/**
	 * Fox is setup for 144MHz transmission, but HL_PrepareAndMatch80m() call is happened
	 */
	YhlFailureCause_2mOnHLPrepareAndMatch80m = 29,

	/**
	 * Somehow fox is already prepared when HL_PrepareAndMatch80m() call happened
	 */
	YhlFailureCause_FoxIsPreparedOnHLPrepareAndMatch80m = 30,

	/**
	 * Failed to reconnect to bluetooth after device rename
	 */
	YhlFailureCause_FailedToReconnectToBluetoothAfterRename = 31,

	/**
	 * DrawMatchingDisplay() is called, but current display is not matching display
	 */
	YhlFailureCause_DrawMatchingDisplayWhenNoMatchingDisplay = 32,

	/**
	 * DrawMatchingDisplay() is called, but no antenna matching in progress
	 */
	YhlFailureCause_DrawMatchingDisplayWhenNoMatchingInProgress = 33,

	/**
	 * Morse sequence starts with End of Sequence element
	 */
	YhlFailureCause_SequenceStartsWithEoS = 34,

	/**
	 * Wrong morse player state in MorseTickMs()
	 */
	YhlFailureCause_WrongMorsePlayerStateInMorseTickMs = 35,

	/**
	 * Wrong element in MorseGetInterval()
	 */
	YhlFailureCause_WrongElementInMorseGetInterval = 36,

	/**
	 * Wrong fox code in MorseGetFoxSequence()
	 */
	YhlFailureCause_WrongFoxCodeInMorseGetFoxSequence = 37,

	/**
	 * Wrong payload size in SendPacket()
	 */
	YhlFailureCause_WrongPayloadSizeInSendPacket = 38,

	/**
	 * Failed to start low speed external oscillator
	 */
	YhlFailureCause_FailedToStartLSE = 39,

	/**
	 * Failed to switch RTC source to LSE
	 */
	YhlFailureCause_FailedToSwitchToLSE = 40,

	/**
	 * Failed to init RTC
	 */
	YhlFailureCause_FailedToInitRTC = 41,

	/**
	 * Failed to set RTC date
	 */
	YhlFailureCause_FailedToSetRTCDate = 42,

	/**
	 * Failed to set RTC time
	 */
	YhlFailureCause_FailedToSetRTCTime = 43,

	/**
	 * Failed to get RTC time
	 */
	YhlFailureCause_FailedToGetRTCTime = 44,

	/**
	 * Failed to get RTC date
	 */
	YhlFailureCause_FailedToGetRTCDate = 45,

	/**
	 * Too much on second change listeners
	 */
	YhlFailureCause_TooMuchSecondChangeListeners = 46,

	/**
	 * Too much date or time listeners
	 */
	YhlFailureCause_TooMuchDateOrTimeListeners = 47,

	/**
	 * Wrong week day in GetWeekdayFromDayNumber()
	 */
	YhlFailureCause_WrongWeekDayInGetWeekdayFromDayNumber = 48,

	/**
	 * Wrong fox code in DrawFoxCode
	 */
	YhlFailureCause_WrongFoxCodeInDrawFoxCode = 49,

	/**
	 * Wrong cycle state in DrawFoxCycleState()
	 */
	YhlFailureCause_WrongCycleStateInDrawFoxCycleState = 50,

	/**
	 * Wrong global fox state in DrawGlobalState
	 */
	YhlFailureCause_WrongGlobalFoxStateInDrawGlobalState = 51,

	/**
	 * Wrong packet machine state in HAL_UART_RxCpltCallback
	 */
	YhlFailureCause_WrongPSMStateInHALUARTRxCpltCallback = 52,

	/**
	 * Failed to abort UART listening
	 */
	YhlFailureCause_FailedToAbortUartListening = 53,

	/**
	 * Failed to transmit over UART (blocking)
	 */
	YhlFailureCause_FailedToTransmitOverUart = 54,

	/**
	 * Failed to receive over UART (blocking)
	 */
	YhlFailureCause_FailedToReceiveOverUart = 55,

	/**
	 * Failed to transmit over UART (interrupts)
	 */
	YhlFailureCause_FailedToTransmitOverUartIt = 56,

	/**
	 * Failed to receive over UART (interrupts)
	 */
	YhlFailureCause_FailedToReceiveOverUartIt = 57,

	/**
	 * No OnClose handler specified for item selection display
	 */
	YhlFailureCause_NoCloseHandlerInItemSelectionDisplay = 58,

	/**
	 * MenuDisplay_DrawMenuDisplay() is called, but current display is not menu display
	 */
	YhlFailureCause_CurrentDisplayIsNotMenuDisplay = 59,

	/**
	 * Menu element both not a node and not a leaf
	 */
	YhlFailureCause_NotANodeNotALeaf = 60,

	/**
	 * Wrong index in MenuDisplay_GetFrequencyRangeByIndex()
	 */
	YhlFailureCause_WrongIndexInMenuDisplayGetFrequencyRangeByIndex = 61,

	/**
	 * Wrong index in MenuDisplay_IndexToFoxSpeed()
	 */
	YhlFailureCause_WrongIndexInMenuDisplayIndexToFoxSpeed = 62,

	/**
	 * Wrong index in MenuDisplay_IndexToCycleType()
	 */
	YhlFailureCause_WrongIndexInMenuDisplayIndexToCycleType = 63,

	/**
	 * No formatter in number input display
	 */
	YhlFailureCause_NoFomatterInNumberInputDisplay = 64,

	/**
	 * No OnClose() handler in number input display
	 */
	YhlFailureCause_NoCloseHandlerInNumberInputDisplay = 65,

	/**
	 * No OnClose() handler in time input display
	 */
	YhlFailureCause_NoCloseHandlerInTimeInputDisplay = 66,

	/**
	 * Wrong active value in TimeInputDisplay_CostructTimeFormatString()
	 */
	YhlFailureCause_WrongActiveValueInTimeInputDisplayCostructTimeFormatString = 67,

	/**
	 * Wrong active value in TimeInputDisplay_EncoderClickHandler()
	 */
	YhlFailureCause_WrongActiveValueInTimeInputDisplayEncoderClickHandler = 68,

	/**
	 * Wrong active value in TimeInputDisplay_EncoderRotationHandler()
	 */
	YhlFailureCause_WrongActiveValueInTimeInputDisplayEncoderRotationHandler = 69,

	/**
	 * Unable to find EEPROM
	 */
	YhlFailureCause_UnableToFindEEPROM = 70,

	/**
	 * Faulty EEPROM (during headers initialization)
	 */
	YhlFailureCause_FaultyEEPROMInHeaders = 71,

	/**
	 * Faulty EEPROM (during profiles initialization)
	 */
	YhlFailureCause_FaultyEEPROMInProfiles = 72,

	/**
	 * Wrong profile id in EEPROM_SwitchProfile()
	 */
	YhlFailureCause_WrongProfileIdInEEPROMSwitchProfile = 73,

	/**
	 * Attempt to add to many profiles
	 */
	YhlFailureCause_TooManyProfiles = 74,

	/**
	 * Wrong profile id in EEPROM_GetProfile()
	 */
	YhlFailureCause_WrongProfileIdInEEPROMGetProfile = 75,

	/**
	 * Wrong profile id in EEPROM_UpdateProfile()
	 */
	YhlFailureCause_WrongProfileIdInEEPROMUpdateProfile = 76,

	/**
	 * Failed to de-initialize display bus
	 */
	YhlFailureCause_FailedToDeInitializeDisplayBus = 77,

	/**
	 * Double call of HAL_DeInitializeDisplayBus()
	 */
	YhlFailureCause_DisplayBusAlreadyDeinitialized = 78,

	/**
	 * Double call of HAL_InitializeDisplayBus()
	 */
	YhlFailureCause_DisplayBusAlreadyInitialized = 79,

	/**
	 * Failed to initialize display bus
	 */
	YhlFailureCause_FailedToInitializeDisplayBus = 80,

	/**
	 * Failed to enable HSI while entering economy mode
	 */
	YhlFailureCause_EconomyModeFailedToEnableHSI = 81,

	/**
	 * Failed to switch to HSI while entering economy mode
	 */
	YhlFailureCause_EconomyModeFailedToSwitchToHSI = 82,

	/**
	 * Failed to disable HSE and PLL while entering economy mode
	 */
	YhlFailureCause_EconomyModeFailedToDisableHSEAndPLL = 83,

	/**
	 * We are already in economy mode
	 */
	YhlFailureCause_AlreadyInEconomyMode = 84,

	/**
	 * We aren't in economy mode
	 */
	YhlFailureCause_NotInEconomyMode = 85,

	/**
	 * Failed to switch on HSE and PLL when exiting economy mode
	 */
	YhlFailureCause_EconomyModeFailedToSwitchOnHSEandPLL = 86,

	/**
	 * Failed to switch to PLL when exiting economy mode
	 */
	YhlFailureCause_EconomyModeFailedToSwitchToPLL = 87,

	/**
	 * Failed to disable ADC
	 */
	YhlFailureCause_FailedToDisableADC = 88,

	/**
	 * Attempt to go to sleep while already sleeping
	 */
	YhlFailureCause_AlreadySleeping = 89,

	/**
	 * Attempt to wake while not sleeping
	 */
	YhlFailureCause_NotSleeping = 90,

	/**
	 * Already deep sleeping
	 */
	YhlFailureCause_AlreadyDeepSleeping = 91,

	/**
	 * Not deep sleeping
	 */
	YhlFailureCause_NotDeepSleeping = 92,

	/**
	 * We can't go deeper then deep sleep in Sleepmodes_OnNewSecond()
	 */
	YhlFailureCause_CantGoDeeperInSleepmodesOnNewSecond = 93,

	/**
	 * Wrong sleepmode in Sleepmodes_OnNewSecond()
	 */
	YhlFailureCause_WrongSleepmodeInSleepmodesOnNewSecond = 94,

	/**
	 * Wrong sleepmode in Sleepmodes_PreventSleep()
	 */
	YhlFailureCause_WrongSleepmodeInSleepmodesPreventSleep = 95,

	/**
	 * Failed to initialize bluetooth's UART
	 */
	YhlFailureCause_FailedToInitializeBluetoothUART = 96,

	/**
	 * Failed to deinitialize bluetooth's UART in HAL_EnableUART()
	 */
	YhlFailureCause_FailedToDeinitializeBluetoothUARTInHALEnableUART = 97,

	/**
	 * Failed to deinitialize bluetooth's UART in HAL_DisableUART()
	 */
	YhlFailureCause_FailedToDeinitializeBluetoothUARTInHALDisableUART = 98,

	/**
	 * Fox start or end timespans are invalid
	 */
	YhlFailureCause_InvalidStartOrEndTimespans = 99,

	/**
	 * Fox is already armed
	 */
	YhlFailureCause_AlreadyArmed = 100,

	/**
	 * Null pointer is given to TimeInputDisplay_CostructTimeFormatString()
	 */
	YhlFailureCause_NullPtrInTimeInputDisplayCostructTimeFormatString = 101,

	/**
	 * New packet came via UART, but previous still not processed yet
	 */
	YhlFailureCause_NewPacketWhilePreviousIsNotProcessed = 102,

	/**
	 * Wrong antenna matcher state machine state in AMSM_MoveToNextState()
	 */
	YhlFailureCause_WrongAMSMStateInAMSMMoveToNextState = 103,

	/**
	 * Antenna matching already in progress, but new request to match came
	 */
	YhlFailureCause_AntennaMatchingAlreadyInProgress = 104,
}
YhlFailureCausesEnum;

extern L2HAL_24x_ContextStruct EEPROMContext;
extern FoxStateStruct FoxState;

/**
 * Log failure cause into EEPROM, shutdown most of the hardware and halt.
 */
void SelfDiagnostics_HaltOnFailure(YhlFailureCausesEnum failureCause);


#endif /* INCLUDE_SELFDIAGNOSTICS_H_ */
