/*
 * FoxStatus.c
 *
 *  Created on: May 14, 2021
 *      Author: fossa
 */

#include <main.h>

void FoxState_Init(void)
{
	FoxStateNameChanged = false;
}

void FoxState_CorrectDateTime(void)
{
	FoxState.CurrentTime.Hours = CurrentTime.Hours;
	FoxState.CurrentTime.Minutes = CurrentTime.Minutes;
	FoxState.CurrentTime.Seconds = CurrentTime.Seconds;
}
