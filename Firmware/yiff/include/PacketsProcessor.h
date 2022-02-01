/*
 * PacketsProcessor.h
 *
 *  Created on: May 8, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_PACKETSPROCESSOR_H_
#define INCLUDE_PACKETSPROCESSOR_H_

#include <Commands.h>
#include <l2hal.h>
#include <FoxStatus.h>
#include <EEPROM.h>
#include <HAL.h>
#include <RTC.h>
#include <UART.h>
#include <GlobalStateMachine.h>
#include <SelfDiagnostics.h>
#include <Sleepmodes.h>
#include <Version.h>

extern L2HAL_CRCContextStruct CRC_Context;
extern PendingCommandsFlagsStruct PendingCommandsFlags;
extern FoxStateStruct FoxState;
extern uint8_t SwitchToThisProfileId;
extern char SetThisProfileName[YHL_PROFILE_NAME_MEMORY_SIZE];

/**
 * Packet size minus this value is payload size
 */
#define YHL_PACKET_PROCESSOR_PAYLOAD_SIZE_DELTA 5U

/**
 * If first byte of payload is this byte, then it's a command to fox
 */
#define YHL_PACKET_PROCESSOR_COMMAND_TO_FOX 0x00U

/**
 * Start response with this byte
 */
#define YHL_PACKET_PROCESSOR_RESPONSE_FROM_FOX 0x01U

/**
 * Packet payload with command to fox can't be shorter then this
 */
#define YHL_PACKET_PROCESSOR_MIN_COMMAND_TO_FOX_PAYLOAD_LENGTH 2U

/**
 * Possible payload sizes
 */
#define YHL_PACKET_PROCESSOR_MIN_PAYLOAD_SIZE 1U
#define YHL_PACKET_PROCESSOR_MAX_PAYLOAD_SIZE 59U

/**
 * Responses to requests
 */
#define YHL_PACKET_PROCESSOR_SUCCESS 0x00
#define YHL_PACKET_PROCESSOR_FAILURE 0x01

/**
 * Start event message with this byte
 */
#define YHL_PACKET_PROCESSOR_EVENT_FROM_FOX 0x02U

/**
 * Possible commands to fox
 */
typedef enum
{
	SetDateAndTime = 0x00,

	SetName = 0x01,

	GetName = 0x02,

	GetProfilesCount = 0x03,

	GetProfileName = 0x04,

	AddNewProfile = 0x05,

	GetCurrentProfileId = 0x06,

	SwitchProfile = 0x07,

	SetProfileName = 0x08,

	GetFrequency = 0x09,

	SetFrequency = 0x0A,

	GetCode = 0x0B,

	SetCode = 0x0C,

	GetSpeed = 0x0D,

	SetSpeed = 0x0E,

	GetCycle = 0x0F,

	SetCycle = 0x10,

	GetEndingToneDuration = 0x11,

	SetEndingToneDuration = 0x12,

	GetBeginAndEndTimes = 0x13,

	IsFoxArmed = 0x14,

	ArmFox = 0x15,

	DisarmFox = 0x16,

	SetBeginAndEndTimes = 0x17,

	GetFoxPower = 0x18,

	SetFoxPower = 0x19,

	GetBatteryLevel = 0x1A,

	GetUAntADC = 0x1B,

	GetUBattADC = 0x1C,

	GetU80mADC = 0x1D,

	GetUbattVolts = 0x1E,

	GetU80mVolts = 0x1F,

	GetLastFailureCode = 0x20,

	NoOperation = 0x21,

	GetIdentificationData = 0x22,

	GetAntennaMatchingStatus = 0x23,

	GetAntennaMatchingData = 0x24,

	MarkMatchingAsSeen = 0x25,

	CheckForProfileSettingsChanges = 0x26,

	ResetLastFailureCode = 0x27,

	UpdateSerialNumber = 0x28,

	GetUbattADCToUbattVoltsFactors = 0x29,

	SetUbattADCToUbattVoltsFactors = 0x2A,

	GetUbattVoltsToBattLevelFactors = 0x2B,

	SetUbattVoltsToBattLevelFactors = 0x2C,

	GetU80mADCtoU80mVoltsFactors = 0x2D,

	SetU80mADCtoU80mVoltsFactors = 0x2E,

	GetP80mToU80mFactors = 0x2F,

	SetP80mToU80mFactors = 0x30,

	GetUantVolts = 0x31,

	GetUantADCToUantVoltsFactors = 0x32,

	SetUantADCToUantVoltsFactors = 0x33,

	ForceTxOn = 0x34,

	ReturnAfterForceTxOn = 0x35,

	GetRTCCalibrationValue = 0x36,

	SetRTCCalibrationValue = 0x37
}
CommandToFoxEnum;

/**
 * Possible events, generated by fox
 */
typedef enum
{
	EnteringSleepmode = 0x01,
}
EventsFromFoxEnum;

/**
 * Call this when new raw packet came from UART
 */
void OnNewRawPacket(uint8_t size, uint8_t* packet);

/**
 * Called when new packet with correct CRC arrives
 */
void OnNewPacket(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when new command to fox came. Payload is not a packet payload, but packet payload except first byte
 * (which is always YHL_PACKET_PROCESSOR_COMMAND_TO_FOX).
 */
void OnNewCommandToFox(uint8_t payloadSize, uint8_t* payload);

/**
 * Sends packet to smartphone
 */
void SendPacket(uint8_t payloadSize, uint8_t* payload);

/**
 * Send response to given command
 */
void SendResponse(CommandToFoxEnum responseTo, uint8_t payloadSize, uint8_t* payload);

/**
 * Send event from fox
 */
void SendEvent(EventsFromFoxEnum event, uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set date and time" comes
 */
void OnSetDateAndTime(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set fox name" comes
 */
void OnSetName(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get fox name" comes
 */
void OnGetName(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get profiles count" comes
 */
void OnGetProfilesCount(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get profile name" comes
 */
void OnGetProfileName(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Add new profile" comes
 */
void OnAddNewProfile(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get current profile ID" comes
 */
void OnGetCurrentProfileId(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Switch profile" comes
 */
void OnSwitchProfile(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set profile name" comes
 */
void OnSetProfileName(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get frequency" comes
 */
void OnGetFrequency(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set frequency" comes
 */
void OnSetFrequency(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get code" comes
 */
void OnGetCode(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set speed" comes
 */
void OnSetSpeed(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set code" comes
 */
void OnSetCode(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get speed" comes
 */
void OnGetSpeed(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get cycle parameters" comes
 */
void OnGetCycle(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set cycle parameters" comes
 */
void OnSetCycle(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get ending tone duration" comes
 */
void OnGetEndingToneDuration(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set ending tone duration" comes
 */
void OnSetEndingToneDuration(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get begin and end times" comes
 */
void OnGetBeginAndEndTimes(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Is fox armed" comes
 */
void OnIsFoxArmed(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Arm fox" comes
 */
void OnArmFox(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Disarm fox" comes
 */
void OnDisarmFox(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set begin and end times" comes
 */
void OnSetBeginAndEndTimes(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Get fox power" comes
 */
void OnGetFoxPower(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when command "Set fox power" comes
 */
void OnSetFoxPower(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get battery level" command comes
 */
void OnGetBatteryLevel(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get UAnt ADC" command comes
 */
void OnGetUAntADC(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get UBatt ADC" command comes
 */
void OnGetUBattADC(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get U80m ADC" command comes
 */
void OnGetU80mADC(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get UBatt Volts" command comes
 */
void OnGetUBattVolts(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get U80m Volts" command comes
 */
void OnGetU80mVolts(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get last failure code" command comes
 */
void OnGetLastFailureCode(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "No operation" command comes
 */
void OnNoOperation(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get identification data" command comes
 */
void OnGetIdentificationData(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get antenna matching status" command comes
 */
void OnGetAntennaMatchingStatus(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get antenna matching data" command comes
 */
void OnGetAntennaMatchingData(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Mark matching data as seen" command comes
 */
void OnMarkMatchingAsSeen(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Check for profile settings changes" command comes
 */
void OnCheckForProfileSettingsChanges(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Reset last failure code" command comes
 */
void OnResetLastFailureCode(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Update serial number" command comes
 */
void OnUpdateSerialNumber(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get Ubatt(ADC) to Ubatt(Volts) factors" command comes
 */
void OnGetUbattADCToUbattVoltsFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Set Ubatt(ADC) to Ubatt(Volts) factors" command comes
 */
void OnSetUbattADCToUbattVoltsFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get Ubatt(Volts) to battery level factors" command comes
 */
void OnGetUbattVoltsToBattLevelFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Set Ubatt(Volts) to battery level factors" command comes
 */
void OnSetUbattVoltsToBattLevelFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get U80m(ADC) to U80m(Volts) level factors" command comes
 */
void OnGetU80mADCtoU80mVoltsFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Set U80m(ADC) to U80m(Volts) level factors" command comes
 */
void OnSetU80mADCtoU80mVoltsFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get P80m to U80m factors" command comes
 */
void OnGetP80mToU80mFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Set P80m to U80m factors" command comes
 */
void OnSetP80mToU80mFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get Uant(volts) command comes
 */
void OnGetUantVolts(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get Uant(ADC) -> Uant(Volts) factors" command comes
 */
void OnGetUantADCToUantVoltsFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Set Uant(ADC) -> Uant(Volts) factors" command comes
 */
void OnSetUantADCToUantVoltsFactors(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Force TX On" service command comes
 */
void OnForceTxOn(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Return to normal operations after force TX On" command comes
 */
void OnReturnAfterForceTxOn(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Get RTC calibration value" command comes
 */
void OnGetRTCCalibrationValue(uint8_t payloadSize, uint8_t* payload);

/**
 * Called when "Set RTC calibration value" command comes
 */
void OnSetRTCCalibrationValue(uint8_t payloadSize, uint8_t* payload);

/**
 * Emits "Entering sleepmode" event
 */
void EmitEnteringSleepmodeEvent(void);

/**
 * Returns 0x00 if false, 0x01 if true
 */
uint8_t FromBool(bool data);

/**
 * If 0x00 then returns false, otherwise - true
 */
bool ToBool(uint8_t data);

/**
 * If 0x00 or 0x01 returns true, otherwise false
 */
bool IsBool(uint8_t data);

#endif /* INCLUDE_PACKETSPROCESSOR_H_ */
