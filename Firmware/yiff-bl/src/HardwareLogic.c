/*
 * HardwareLogic.c
 *
 *  Created on: Feb 10, 2022
 *      Author: fossa
 */

#include <HardwareLogic.h>

void HL_TurnDisplayOn(void)
{
	/* Detecting display (might require some attempts) */
	int8_t attemptsCounter = YBL_HL_INITIALIZE_DISPLAY_ATTEMPS_COUNT;
	while (attemptsCounter >= 0)
	{
		/* De-initializing I2C bus and pulling pins down to remove parasitic power */
		if (HAL_GetDisplayBusInitializationStatus())
		{
			HAL_DeInitializeDisplayBus();
		}
		HAL_SupressDisplayParasiticPower();

		/* Powering off */
		HAL_SwitchDisplayPower(false);

		HAL_Delay(YBL_HL_DISPLAY_POWERDOWN_TIME); /* Time to power down*/

		/* Powering on */
		HAL_SwitchDisplayPower(true);
		HAL_Delay(YBL_HL_DISPLAY_REGULATOR_SPINUP_TIME); /* Time to spin regulator up */

		HAL_InitializeDisplayBus();

		L2HAL_SSD1327_Context = L2HAL_SSD1327_DetectDisplay(&I2C_Display);
		if (L2HAL_SSD1327_Context.IsFound)
		{
			/* Found */
			break;
		}

		attemptsCounter --;
	}

	if (!L2HAL_SSD1327_Context.IsFound)
	{
		/* Display not found */
		L2HAL_Error(Generic);
	}

	L2HAL_SSD1327_InitDisplay(&L2HAL_SSD1327_Context);
}
