/*
 * l2hal_ad9835.h
 *
 *  Created on: Dec 20, 2020
 *      Author: fossa
 */

#ifndef L2HAL_DRIVERS_DDS_AD9835_INCLUDE_L2HAL_AD9835_H_
#define L2HAL_DRIVERS_DDS_AD9835_INCLUDE_L2HAL_AD9835_H_

#include <l2hal_aux.h>
#include <l2hal_ad9835_private.h>

/**
 * Initialize AD9835.
 */
void L2HAL_AD9835_Init(L2HAL_AD9835_ContextStruct* context);

/**
 * Select source for PHASE and FREQ.
 * @sync FSEL/PSEL synchronous load.
 * @useBits If true, then use control bits, otherwise - control pins.
 */
void L2HAL_AD9835_SelectPFSource(L2HAL_AD9835_ContextStruct* context, bool sync, bool useBits);

/**
 * Power control and reset.
 * @sleep If true, then DDS is put to sleep.
 * @reset Reset phase accumulator.
 * @clear Reset SYNC and SELSRC.
 */
void L2HAL_AD9835_PowerControl(L2HAL_AD9835_ContextStruct* context, bool sleep, bool reset, bool clear);

/**
 * Select given phase register (if bits used as registers sources).
 */
void L2HAL_AD9835_SelectPhaseRegister(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_PhaseRegisters phaseRegister);

/**
 * Select given frequency register (if bits used as registers sources).
 */
void L2HAL_AD9835_SelectFrequencyRegister(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_FrequencyRegisters frequencyRegister);

/**
 * As L2HAL_AD9835_SelectPhaseRegister() and L2HAL_AD9835_SelectFrequencyRegister(), but sets both registers using only one write
 */
void L2HAL_AD9835_SelectFrequencyAndPhaRegisters(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_FrequencyRegisters frequencyRegister,
		enum L2HAL_AD9835_PhaseRegisters phaseRegister);

/**
 * Writes given 8 frequency bits to defer register. Feed with destination frequency register and data.
 */
void L2HAL_AD9835_Write8FrequencyBitsToDeferRegister(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_Registers frequencyRegister, uint8_t data);

/**
 * Writes 8 given bits + 8 bits from defer register into given frequency register.
 */
void L2HAL_AD9835_Write16FrequencyBits(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_Registers frequencyRegister, uint8_t data);

/**
 * As L2HAL_AD9835_Write8FrequencyBitsToDeferRegister(), but for phase register
 */
void L2HAL_AD9835_Write8PhaseBitsToDeferRegister(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_Registers phaseRegister, uint8_t data);

/**
 * As L2HAL_AD9835_Write16FrequencyBits(), but for phase register
 */
void L2HAL_AD9835_Write16PhaseBits(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_Registers phaseRegister, uint8_t data);

/**
 * Write full frequency word into given frequency register.
 */
void L2HAL_AD9835_WriteFrequencyWord(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_FrequencyRegisters frequencyRegister, uint32_t data);

/**
 * As L2HAL_AD9835_WriteFrequencyWord(), but for phase register.
 */
void L2HAL_AD9835_WritePhaseWord(L2HAL_AD9835_ContextStruct* context, enum L2HAL_AD9835_PhaseRegisters phaseRegister, uint16_t data);

#endif /* L2HAL_DRIVERS_DDS_AD9835_INCLUDE_L2HAL_AD9835_H_ */
