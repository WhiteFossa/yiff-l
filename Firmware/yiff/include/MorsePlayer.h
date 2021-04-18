/*
 * MorsePlayer.h
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#ifndef INCLUDE_MORSEPLAYER_H_
#define INCLUDE_MORSEPLAYER_H_

/**
 * Length of dit in milliseconds
 */
#define MORSE_SLOW_FOX_DIT_LENGTH 140U
#define MORSE_FAST_FOX_DIT_LENGTH 70U

/**
 * Element of playable sequence
 */
typedef enum
{
	DitTone, /* . tone */
	DitPause, /* . pause */
	DahTone, /* - tone */
	DahPause, /* - pause */
	EoS /* End of sequence, start from begin */
}
MorseElementEnum;


/**
 * Morse player state machine state
 */
typedef enum
{
	/**
	 * Something is being played
	 */
	InInterval,

	/**
	 * Taking next sequence element
	 */
	TakingNext,

	/**
	 * Awaiting for start
	 */
	NotPlaying
}
MorsePlayerSMStateEnum;

/**
 *  Sequence to play
 */
MorseElementEnum* MorseSequenceToPlay;

/**
 * Current element in sequence
 */
MorseElementEnum* MorseCurrentElement;

/**
 * Finish fox sequence
 */
static const MorseElementEnum Morse_FoxFinish[] = {
		DahTone, DitPause, DahTone, DahPause, /* - - */
		DahTone, DitPause, DahTone, DitPause, DahTone, /* - - - */
		DahPause, DahPause, DahPause, EoS };

/**
 * Fox 1 sequence
 */
static const MorseElementEnum Morse_Fox1[] = {
		DahTone, DitPause, DahTone, DahPause, /* - - */
		DahTone, DitPause, DahTone, DitPause, DahTone, DahPause, /* - - - */
		DitTone, /* . */
		DahPause, DahPause, DahPause, EoS };

/**
 * Fox 2 sequence
 */
static const MorseElementEnum Morse_Fox2[] = {
		DahTone, DitPause, DahTone, DahPause, /* - - */
		DahTone, DitPause, DahTone, DitPause, DahTone, DahPause, /* - - - */
		DitTone, DitPause, DitTone, /* .. */
		DahPause, DahPause, DahPause, EoS };

/**
 * Fox 3 sequence
 */
static const MorseElementEnum Morse_Fox3[] = {
		DahTone, DitPause, DahTone, DahPause, /* - - */
		DahTone, DitPause, DahTone, DitPause, DahTone, DahPause, /* - - - */
		DitTone, DitPause, DitTone, DitPause, DitTone, /* ... */
		DahPause, DahPause, DahPause, EoS };

/**
 * Fox 4 sequence
 */
static const MorseElementEnum Morse_Fox4[] = {
		DahTone, DitPause, DahTone, DahPause, /* - - */
		DahTone, DitPause, DahTone, DitPause, DahTone, DahPause, /* - - - */
		DitTone, DitPause, DitTone, DitPause, DitTone, DitPause, DitTone, /* .... */
		DahPause, DahPause, DahPause, EoS };

/**
 * Fox 5 sequence
 */
static const MorseElementEnum Morse_Fox5[] = {
		DahTone, DitPause, DahTone, DahPause, /* - - */
		DahTone, DitPause, DahTone, DitPause, DahTone, DahPause, /* - - - */
		DitTone, DitPause, DitTone, DitPause, DitTone, DitPause, DitTone, DitPause, DitTone, /* ..... */
		DahPause, DahPause, DahPause, EoS };

/**
 * Beacon sequence
 */
static const MorseElementEnum Morse_Beacon[] = {
		DitTone, DitPause, DitTone, DitPause, DitTone, /* ... */
		DahPause, DahPause, DahPause, EoS };

/**
 * State machine state
 */
MorsePlayerSMStateEnum MorsePlayerState;

/**
 * Morse player current interval in milliseconds
 */
uint16_t MorsePlayerCurrentInterval;

/**
 * Call it before starting to call MorseTickMs()
 */
void MorsePlayerInit();

/**
 * Play this sequence
 */
void MorsePlayerPlay(MorseElementEnum* sequenceToPlay);

/**
 * Stops to play
 */
void MorsePlayerStop();

/**
 * Call this each millisecond (precisely, from SysTick).
 */
void MorseTickMs(void);

/**
 * Returns invterval in milliseconds for a given element of sequence
 */
uint16_t MorseGetInterval(MorseElementEnum element);

/**
 * Returns fox sequence depending on fox global state
 */
MorseElementEnum* MorseGetFoxSequence();

#endif /* INCLUDE_MORSEPLAYER_H_ */
