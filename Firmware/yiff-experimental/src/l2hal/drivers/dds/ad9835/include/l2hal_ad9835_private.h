/*
 * l2hal_ad9835_private.h
 *
 *  Created on: Dec 20, 2020
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_DDS_AD9835_INCLUDE_L2HAL_AD9835_PRIVATE_H_
#define L2HAL_DRIVERS_DDS_AD9835_INCLUDE_L2HAL_AD9835_PRIVATE_H_

#include <stdint.h>
#include <stm32f1xx_hal.h>

#define L2HAL_AD9835_SMALLDELAY 500UL

/**
 * Various commands
 */
enum L2HAL_AD9835_Commands
{
	Write16PhaseBits = 0b00000000,
	Write8PhaseBitsToDefer = 0b00010000,
	Write16FrequencyBits = 0b00100000,
	Write8FrequencyBitsToDefer = 0b00110000,
	SelectPhaseReg = 0b01000000,
	SelectFrequencyReg = 0b01010000,
	SelectPhaseAndFrequencyRegs = 0b01100000
};

/**
 * Registers addresses
 */
enum L2HAL_AD9835_Registers
{
	Freg08LLSBs = 0b0000,
	Freg08HLSBs = 0b0001,
	Freg08LMSBs = 0b0010,
	Freg08HMSBs = 0b0011,
	Freg18LLSBs = 0b0100,
	Freg18HLSBs = 0b0101,
	Freg18LMSBs = 0b0110,
	Freg18HMSBs = 0b0111,
	Phase08LSBs = 0b1000,
	Phase08MSBs = 0b1001,
	Phase18LSBs = 0b1010,
	Phase18MSBs = 0b1011,
	Phase28LSBs = 0b1100,
	Phase28MSBs = 0b1101,
	Phase38LSBs = 0b1110,
	Phase38MSBs = 0b1111
};

/**
 * Phase registers
 */
enum L2HAL_AD9835_PhaseRegisters
{
	Phase0 = 0b000,
	Phase1 = 0b010,
	Phase2 = 0b100,
	Phase3 = 0b110
};

/**
 * Frequency registers
 */
enum L2HAL_AD9835_FrequencyRegisters
{
	Freg0 = 0b0000,
	Freg1 = 0b1000
};

/**
 * DDS context, SPI connection and various stuff is stored here
 */
typedef struct
{
	/**
	 * SPI bus handle
	 */
	SPI_HandleTypeDef* SPIHandle;

	/**
	 * AD9835 FSYNC pin is connected to this port and pin.
	 */
	GPIO_TypeDef* FSYNCPort;
	uint32_t FSYNCPin;

}
L2HAL_AD9835_ContextStruct;

/**
 * Data to send to PLL
 */
typedef struct
{
	uint8_t Msb;
	uint8_t Lsb;
}
L2HAL_AD9835_DataStruct;

void L2HAL_AD9835_WriteToDDS(L2HAL_AD9835_ContextStruct* context, L2HAL_AD9835_DataStruct* data);

/**
 * Small dumb delay (some microseconds)
 */
void L2HAL_AD9835_SmallDelay();

#endif /* L2HAL_DRIVERS_DDS_AD9835_INCLUDE_L2HAL_AD9835_PRIVATE_H_ */
