/*
 * MorsePlayerPrivate.h
 *
 *  Created on: May 31, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MORSEPLAYERPRIVATE_H_
#define INCLUDE_MORSEPLAYERPRIVATE_H_

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


#endif /* INCLUDE_MORSEPLAYERPRIVATE_H_ */
