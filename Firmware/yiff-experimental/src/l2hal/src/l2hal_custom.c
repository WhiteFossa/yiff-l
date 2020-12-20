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
	L2HAL_SetupI2C();

	/* Preparing SPI */
	L2HAL_SetupSPI();

	/* Preparing buttons driver */
	L2HAL_SetupButtons();

	/* Preparing encoders driver */
	L2HAL_SetupEncoders();
}

void L2HAL_SetupI2C(void)
{
	/* Clocking I2C */
	__HAL_RCC_I2C1_CLK_ENABLE();

	/**
	 * Setting up I2C
	 */

	I2CHandle.Instance = I2C1;
	I2CHandle.Init.ClockSpeed = 400000; /* 400 KHz */
	I2CHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2CHandle.Init.OwnAddress1 = 0x00;
	I2CHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2CHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2CHandle.Init.OwnAddress2 = 0x00;
	I2CHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2CHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if(HAL_I2C_Init(&I2CHandle) != HAL_OK)
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
}


void L2HAL_SetupButtons(void)
{
	L2HAL_Buttons_Context.SkipPinsInitialization = false;
	L2HAL_Buttons_Context.CustomPinInitializer = NULL;

	L2HAL_Buttons_Init(&L2HAL_Buttons_Context);
}

void L2HAL_SetupEncoders(void)
{
	L2HAL_Encoders_Context.ButtonsDriverContext = &L2HAL_Buttons_Context;
	L2HAL_Encoders_Init(&L2HAL_Encoders_Context);
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
}

void I2C1_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&I2CHandle);
}

void I2C1_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&I2CHandle);
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1)
	{
		L2HAL_SSD1306_InterruptTransferCompleted(&L2HAL_SSD1306_Context);
	}
}

void L2HAL_SetupSPI(void)
{
	SPIHandle.Instance				 = SPI1;
	SPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	SPIHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SPIHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SPIHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	SPIHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SPIHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SPIHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	SPIHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SPIHandle.Init.CRCPolynomial     = 0;
	SPIHandle.Init.NSS               = SPI_NSS_SOFT;
	SPIHandle.Init.Mode = SPI_MODE_MASTER;

	if(HAL_SPI_Init(&SPIHandle) != HAL_OK)
	{
		/* Initialization Error */
		L2HAL_Error(Generic);
	}
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
		/**
		 * Setting up port and SPI
		 * SPI1 at PA5 (SCK) and PA7 (MOSI)
		 */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_SPI1_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_7;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
}
