/*
 * HardwareLogic.c
 *
 *  Created on: Apr 17, 2021
 *      Author: fossa
 */

#include <main.h>

void ProcessManipulatorFoxStateChange()
{
	if (FoxState.IsMorseTx || FoxState.CycleState.IsEndingTone)
	{
		HAL_SwitchManipulator(true);
	}
	else
	{
		HAL_SwitchManipulator(false);
	}
}
