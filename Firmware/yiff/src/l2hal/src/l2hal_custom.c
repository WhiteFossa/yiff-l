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

	/* And SPI */
	L2HAL_SetupSPI();
}

void L2HAL_SetupI2C(void)
{
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

	/* UART1 (for bluetooth) */
	UART_Handle.Instance = USART1;
	UART_Handle.Init.BaudRate = 115200;
	UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART_Handle.Init.StopBits = UART_STOPBITS_1;
	UART_Handle.Init.Parity = UART_PARITY_NONE;
	UART_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART_Handle.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_DeInit(&UART_Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	if(HAL_UART_Init(&UART_Handle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == L2HAL_DISPLAY_BUS)
	{
		/**
		 * I2C for display
		 * Setting up port
		 * I2C1 at PB6 (SCL) and PB7 (SDA)
		 */

		/* Clocking port */
		L2HAL_DISPLAY_BUS_CLOCK_PORT();

		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = L2HAL_DISPLAY_BUS_SCL | L2HAL_DISPLAY_BUS_SDA;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

		HAL_GPIO_Init(L2HAL_DISPLAY_BUS_PORT, &GPIO_InitStruct);

		/* Display driver uses I2C interrupts exchange */
		HAL_NVIC_SetPriority(I2C1_ER_IRQn, I2C1_ER_IRQN_PRIORITY, I2C1_ER_IRQN_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
		HAL_NVIC_SetPriority(I2C1_EV_IRQn, I2C1_EV_IRQN_PRIORITY, I2C1_EV_IRQN_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
	}
	else if (hi2c->Instance == I2C2)
	{
		/**
		 * I2C for other devices
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

		/* Second bus may use I2C interrupts exchange too */
		HAL_NVIC_SetPriority(I2C2_ER_IRQn, I2C2_ER_IRQN_PRIORITY, I2C2_ER_IRQN_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
		HAL_NVIC_SetPriority(I2C2_EV_IRQn, I2C2_EV_IRQN_PRIORITY, I2C2_EV_IRQN_SUBPRIORITY);
		HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
	}
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
	if (hi2c->Instance == L2HAL_DISPLAY_BUS)
	{
		HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
		HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);

		L2HAL_DISPLAY_BUS_CLOCK_DISABLE();
		HAL_GPIO_DeInit(L2HAL_DISPLAY_BUS_PORT, L2HAL_DISPLAY_BUS_SCL | L2HAL_DISPLAY_BUS_SDA);
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

void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
	__HAL_RCC_CRC_CLK_ENABLE();
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc)
{
	__HAL_RCC_CRC_CLK_DISABLE();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();

		/* TX */
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* RX */
		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		__HAL_RCC_USART1_FORCE_RESET();
		__HAL_RCC_USART1_RELEASE_RESET();

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UART_Handle);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	/* Clocking in */
	__HAL_RCC_ADC1_CLK_ENABLE();

	RCC_PeriphCLKInitTypeDef  PeriphClkInit;
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpioInit;
	gpioInit.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
	gpioInit.Mode = GPIO_MODE_ANALOG;
	gpioInit.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpioInit);

	HAL_NVIC_SetPriority(ADC1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
	__HAL_RCC_ADC1_FORCE_RESET();
	__HAL_RCC_ADC1_RELEASE_RESET();

	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);

	HAL_NVIC_DisableIRQ(ADC1_IRQn);
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
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_7);
	}
}

void L2HAL_SetupSPI(void)
{
	SPIHandle.Instance = SPI1;
	SPIHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	SPIHandle.Init.Direction = SPI_DIRECTION_2LINES;
	SPIHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
	SPIHandle.Init.CLKPolarity = SPI_POLARITY_HIGH;
	SPIHandle.Init.DataSize = SPI_DATASIZE_8BIT;
	SPIHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	SPIHandle.Init.TIMode = SPI_TIMODE_DISABLE;
	SPIHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	SPIHandle.Init.CRCPolynomial = 0;
	SPIHandle.Init.NSS = SPI_NSS_SOFT;
	SPIHandle.Init.Mode = SPI_MODE_MASTER;

	if(HAL_SPI_Init(&SPIHandle) != HAL_OK)
	{
		/* Initialization Error */
		L2HAL_Error(Generic);
	}
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == TIM3)
	{
		/* Returning PB0 to normal output mode (it's used as manipulator for 3.5MHz tract) */

		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}


