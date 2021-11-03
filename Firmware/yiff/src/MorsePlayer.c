/*
 * MorsePlayer.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <MorsePlayer.h>
#include <MorsePlayerPrivate.h>
#include <FoxStatusPrivate.h>

void MorsePlayerInit(void)
{
	MorsePlayerState = NotPlaying;
}

void MorsePlayerPlay(MorseElementEnum* sequenceToPlay)
{
	MorseSequenceToPlay = sequenceToPlay;
	MorseCurrentElement = MorseSequenceToPlay;

	if (EoS == *MorseCurrentElement)
	{
		/* Sequence mustn't start with End of Sequence element */
		SelfDiagnostics_HaltOnFailure(YhlFailureCause_SequenceStartsWithEoS);
	}

	MorsePlayerState = TakingNext;
}

void MorsePlayerStop(void)
{
	FoxState.IsMorseTx = false;
	MorsePlayerState = NotPlaying;
	HL_ProcessManipulatorFoxStateChange();
}

void MorseTickMs(void)
{
	switch(MorsePlayerState)
	{
		case NotPlaying:
			return;

		case InInterval:
			if (0 == MorsePlayerCurrentInterval)
			{
				MorsePlayerState = TakingNext;
			}
			else
			{
				MorsePlayerCurrentInterval --;
			}
			break;

		case TakingNext:
			if (EoS == *MorseCurrentElement)
			{
				/* End of sequence reached, looping */
				MorseCurrentElement = MorseSequenceToPlay;
			}

			if (DitTone == *MorseCurrentElement || DahTone == *MorseCurrentElement)
			{
				FoxState.IsMorseTx = true;
				HL_ProcessManipulatorFoxStateChange();
			}
			else if (DitPause == *MorseCurrentElement || DahPause == *MorseCurrentElement)
			{
				FoxState.IsMorseTx = false;
				HL_ProcessManipulatorFoxStateChange();
			}

			MorsePlayerCurrentInterval = MorseGetInterval(*MorseCurrentElement);

			MorseCurrentElement ++;
			MorsePlayerState = InInterval;
			break;

		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongMorsePlayerStateInMorseTickMs);
	}

}

uint16_t MorseGetInterval(MorseElementEnum element)
{
	uint16_t timebase;

	if (FoxState.IsFast)
	{
		timebase = MORSE_FAST_FOX_DIT_LENGTH;
	}
	else
	{
		timebase = MORSE_SLOW_FOX_DIT_LENGTH;
	}

	switch(element)
	{
		case DitTone:
		case DitPause:
			return timebase;

		case DahTone:
		case DahPause:
			return (uint16_t)(timebase * 3U);

		case EoS:
			return 0;

		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongElementInMorseGetInterval);
			return 0; /* Will never happen */
	}
}

MorseElementEnum* MorseGetFoxSequence(void)
{
	switch(FoxState.Code)
	{
		case Finish:
			return (MorseElementEnum*)Morse_FoxFinish;

		case F1:
			return (MorseElementEnum*)Morse_Fox1;

		case F2:
			return (MorseElementEnum*)Morse_Fox2;

		case F3:
			return (MorseElementEnum*)Morse_Fox3;

		case F4:
			return (MorseElementEnum*)Morse_Fox4;

		case F5:
			return (MorseElementEnum*)Morse_Fox5;

		case Beacon:
			return (MorseElementEnum*)Morse_Beacon;

		default:
			SelfDiagnostics_HaltOnFailure(YhlFailureCause_WrongFoxCodeInMorseGetFoxSequence);
			return NULL; /* Will never happen */
	}
}

