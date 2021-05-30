/*
 * GlobalVariables.h
 *
 *  Created on: May 30, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_GLOBALVARIABLES_H_
#define INCLUDE_GLOBALVARIABLES_H_

#include <FoxStatus.h>
#include <Buttons.h>
#include <MorsePlayer.h>
#include <EEPROM.h>
#include <UART.h>

/**
 * I2C bus handle (connected to display).
 */
I2C_HandleTypeDef I2C_Display;

/**
 * I2C bus handle (connected to all other devices).
 */
I2C_HandleTypeDef I2C_Other;

/**
 * Display driver context.
 */
L2HAL_SSD1327_ContextStruct L2HAL_SSD1327_Context;

/**
 * EEPROM context
 */
L2HAL_24x_ContextStruct EEPROMContext;

/**
 * CRC calculator context
 */
L2HAL_CRCContextStruct CRC_Context;

/**
 * UART Handle
 */
UART_HandleTypeDef UART_Handle;

/**
 * HC-06 context
 */
L2HAL_HC06_ContextStruct HC06_Context;

/**
 * Font to use
 */
FMGL_API_Font FMGL_FontTerminusRegular12;

/**
 * FMGL context.
 */
FMGL_API_DriverContext fmglContext;

/**
 * Common font settings.
 */
FMGL_API_FontSettings commonFont;

/**
 * Inverted common font settings.
 */
FMGL_API_FontSettings invertedCommonFont;

/**
 * Big font for displaying frequency settings.
 */
FMGL_API_FontSettings frequencyFont;

/**
 * Off color.
 */
FMGL_API_ColorStruct OffColor;

/**
 * On color.
 */
FMGL_API_ColorStruct OnColor;

/**
 * Fox state
 */
FoxStateStruct FoxState;

/**
 * If true, then fox name just changed, we need to write it to EEPROM from the main thread
 */
bool FoxStateNameChanged;

/**
 * Buttons text
 */
FoxButtonStruct LeftButton;
FoxButtonStruct RightButton;

/**
 * RTC handle
 */
RTC_HandleTypeDef RtcHandle;

/**
 * Second number at the moment of previous call of RTC_Poll();
 */
uint8_t PreviousSecond;

/**
 * How many listeners we have.
 */
uint8_t RtcOnNewSecondListenersCount;

/**
 * This functions listens for new second event.
 */
void (**RtcOnNewSecondListeners)(void);

/**
 * How many date and time change listeners we have.
 */
uint8_t RtcDateAndTimeChangeListenersCount;

/**
 * This functions listens for date and time change.
 */
void (**RtcDateAndTimeChangeListeners)(void);

/**
 * Current date and time.
 */
RTC_TimeTypeDef CurrentTime;
RTC_DateTypeDef CurrentDate;

/**
 *  Sequence to play
 */
MorseElementEnum* MorseSequenceToPlay;

/**
 * Current element in sequence
 */
MorseElementEnum* MorseCurrentElement;

/**
 * State machine state
 */
MorsePlayerSMStateEnum MorsePlayerState;

/**
 * Morse player current interval in milliseconds
 */
uint16_t MorsePlayerCurrentInterval;

/**
 * EEPROM constant header in its actual state
 */
EEPROMConstantHeaderStruct EEPROM_ConstantHeader;

/**
 * EEPROM header in its actual state
 */
EEPROMHeaderStruct EEPROM_Header;

/**
 * Current profile
 */
EEPROMProfileStruct EEPROM_CurrentProfile;


/**
 * Packets state machine state
 */
UART_PacketsSMStateEnum UART_PSMState;

/**
 * Transmission buffer (of dynamic size)
 */
uint8_t* UART_TxBuffer;

/**
 * Rx buffer (we work byte-by-byte, so 1 byte is enough)
 */
uint8_t UART_RxByteBuffer;

/**
 * Packet is being accumulated here
 */
uint8_t UART_RxPacketBuffer[YHL_UART_PACKET_MAX_SIZE];

/**
 * Current byte within UART_RxPacketBuffer
 */
uint8_t UART_RxPacketBufferIndex;

/**
 * Decreases each millisecond, when reaches 0, resets state machine to Listen state.
 */
uint16_t UART_RxTimeoutTimer;

/**
 * When first byte of packet is came, we are able to detect packet length. That length
 * is stored here
 */
uint8_t UART_ExpectedPacketLength;

/**
 * Pointer to function, called when new packet received. DO NOT FORGET
 * TO CALL free() ON PACKET AFTER PROCESSING
 */
void (*UART_OnNewPacket)(uint8_t packetFullLength, uint8_t* packet);

/**
 * True if there is ongoing semiblocking transmission
 */
volatile bool UART_TransmissionInProgress;

#endif /* INCLUDE_GLOBALVARIABLES_H_ */
