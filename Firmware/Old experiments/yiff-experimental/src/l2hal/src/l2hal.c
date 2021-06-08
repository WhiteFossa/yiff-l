/*
	This file is part of Fossa's STM32 level 2 HAL.

	STM32 level 2 HAL is free software:
	you can redistribute it and/or modify it under the terms of the
	GNU General Public License as published by the Free Software
	Foundation, either version 3 of the License, or (at your option)
	any later version.

	STM32 level 2 HAL is distributed in the hope
	that it will be useful, but WITHOUT ANY WARRANTY; without even
	the implied warranty of MERCHANTABILITY or FITNESS FOR A
	PARTICULAR PURPOSE.  See the GNU General Public License for more
	details.

	You should have received a copy of the GNU General Public License
	along with STM32 level 2 HAL.

	If not, see <http://www.gnu.org/licenses/>.

	-------------------------------------------------------------------------

	Created by White Fossa, also known as Artyom Vetrov.

	Feel free to contact: whitefossa@gmail.com

	Repository: https://github.com/WhiteFossa/stm32-l2hal

	If repository above is not available, try my LiveJournal:
	fossa-white.livejournal.com

	or as last measure try to search for #WhiteFossa hashtag.

	-------------------------------------------------------------------------
 */

#include <l2hal.h>

void L2HAL_Init(void)
{
	/* Setting up clocks */
	L2HAL_SetupClocks();

	/* Setting up SysTick handler */
	L2HAL_SysTick_Init();

	/* Initializing custom hardware */
	L2HAL_InitCustomHardware();
}

void L2HAL_SetupClocks(void)
{
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitTypeDef oscinitstruct = {0};
	oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLState = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
	{
		/* Initialization Error */
		L2HAL_Error(Generic);
	}

	/* Select PLL as system clock source and configure the HCLK (AHB), PCLK1 (APB1), PCLK2 (APB2) clocks */
	RCC_ClkInitTypeDef clkinitstruct = {0};
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* 72 MHz */
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1; /* AHB at 72 MHz*/
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2; /* APB1 at 36 MHz*/
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1; /* APB2 at 72 MHz */

	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
	{
		/* Initialization Error */
		L2HAL_Error(Generic);
	}
}

