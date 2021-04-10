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

#include <mcus/stm32f103c8t6/drivers/input/buttons/include/l2hal_stm32f103c8t6_buttons.h>

void L2HAL_Buttons_DefaultPinInitializer(GPIO_TypeDef* port, uint32_t pins)
{
	/* Clocking port in */
	if (GPIOA == port)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (GPIOB == port)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (GPIOC == port)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (GPIOD == port)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else
	{
		/* Non-existing port */
		L2HAL_Error(WrongArgument);
	}


	/* Initializing pin */
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Mode = L2HAL_BUTTONS_DEFAULT_PIN_MODE;
	GPIO_InitStruct.Pull = L2HAL_BUTTONS_DEFAULT_PIN_PULL_MODE;

	GPIO_InitStruct.Pin = pins;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}

