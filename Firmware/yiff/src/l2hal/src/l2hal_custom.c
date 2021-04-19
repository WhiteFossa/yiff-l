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

#include <l2hal_custom.h>

void L2HAL_InitCustomHardware(void)
{
	/* Preparing I2C */
	L2HAL_SetupI2C();}

void L2HAL_SetupI2C(void)
{
	/* I2C for display */
	__HAL_RCC_I2C1_CLK_ENABLE();

	I2C_Display.Instance = I2C1;
	I2C_Display.Init.ClockSpeed = 400000; /* 400 KHz */
	I2C_Display.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2C_Display.Init.OwnAddress1 = 0x00;
	I2C_Display.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2C_Display.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2C_Display.Init.OwnAddress2 = 0x00;
	I2C_Display.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2C_Display.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if(HAL_I2C_Init(&I2C_Display) != HAL_OK)
	{
		/* Initialization Error */
		L2HAL_Error(Generic);
	}

	/* I2C for other devices */
	__HAL_RCC_I2C2_CLK_ENABLE();

	I2C_Other.Instance = I2C2;
	I2C_Other.Init.ClockSpeed = 400000; /* 400 KHz */
	I2C_Other.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2C_Other.Init.OwnAddress1 = 0x00;
	I2C_Other.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2C_Other.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2C_Other.Init.OwnAddress2 = 0x00;
	I2C_Other.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2C_Other.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if(HAL_I2C_Init(&I2C_Other) != HAL_OK)
	{
		/* Initialization Error */
		L2HAL_Error(Generic);
	}

}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1)
	{
		/**
		 * Setting up port
		 * I2C1 at PB6 (SCL) and PB7 (SDA)
		 */

		/* Clocking port */
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Display driver uses I2C interrupts exchange */
		HAL_NVIC_SetPriority(I2C1_ER_IRQn, I2C1_ER_IRQN_PRIORITY, I2C1_ER_IRQN_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
		HAL_NVIC_SetPriority(I2C1_EV_IRQn, I2C1_EV_IRQN_PRIORITY, I2C1_EV_IRQN_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
	}
	else if (hi2c->Instance == I2C2)
	{
		/**
		 * Setting up port
		 * I2C2 at PB10 (SCL) and PB11 (SDA)
		 */

		/* Clocking port */
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* Second bus uses I2C interrupts exchange too */
		HAL_NVIC_SetPriority(I2C2_ER_IRQn, I2C2_ER_IRQN_PRIORITY, I2C2_ER_IRQN_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
		HAL_NVIC_SetPriority(I2C2_EV_IRQn, I2C2_EV_IRQN_PRIORITY, I2C2_EV_IRQN_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
	}
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
	if (hi2c->Instance == I2C1)
	{
		HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
		HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);

		__HAL_RCC_I2C1_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
	}
	else if (hi2c->Instance == I2C2)
	{
		HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
		HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);

		__HAL_RCC_I2C2_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);
	}
}

void I2C1_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&I2C_Display);
}

void I2C1_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&I2C_Display);
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1)
	{
		L2HAL_SSD1327_InterruptTransferCompleted(&L2HAL_SSD1327_Context);
	}
}

void I2C2_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&I2C_Other);
}

void I2C2_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&I2C_Other);
}
