/*
 * MorsePlayer.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

void MorsePlayerInit()
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
		L2HAL_Error(Generic);
	}

	MorsePlayerState = TakingNext;
}

void MorsePlayerStop()
{
	FoxState.IsMorseTx = false;
	MorsePlayerState = NotPlaying;
	ProcessManipulatorFoxStateChange();
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
				ProcessManipulatorFoxStateChange();
			}
			else if (DitPause == *MorseCurrentElement || DahPause == *MorseCurrentElement)
			{
				FoxState.IsMorseTx = false;
				ProcessManipulatorFoxStateChange();
			}

			MorsePlayerCurrentInterval = MorseGetInterval(*MorseCurrentElement);

			MorseCurrentElement ++;
			MorsePlayerState = InInterval;
			break;

		default:
			L2HAL_Error(Generic);
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
			return timebase * 3U;

		case EoS:
			return 0;

		default:
			L2HAL_Error(Generic);
	}
}

MorseElementEnum* MorseGetFoxSequence()
{
	switch(FoxState.Code)
	{
		case Finish:
			return Morse_FoxFinish;

		case F1:
			return Morse_Fox1;

		case F2:
			return Morse_Fox2;

		case F3:
			return Morse_Fox3;

		case F4:
			return Morse_Fox4;

		case F5:
			return Morse_Fox5;

		case Beacon:
			return Morse_Beacon;

		default:
			L2HAL_Error(Generic);
	}
}

