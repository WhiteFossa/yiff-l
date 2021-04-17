/*
 * RTC.c
 *
 *  Created on: Apr 15, 2021
 *      Author: fossa
 */

#include <RTC.h>

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	/* Clocking in backup domain */
	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();

	/* LSE (32.768Hz crystal) as clock source */
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	/* Clocking in RTC */
	__HAL_RCC_RTC_ENABLE();
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_DISABLE();
}

void InitRTC()
{
	RtcHandle.Instance = RTC;
	RtcHandle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	/* Resetting date/time */
	CurrentDate.Year = 21U;
	CurrentDate.Month = RTC_MONTH_JANUARY;
	CurrentDate.Date = 1U;
	CurrentDate.WeekDay = RTC_WEEKDAY_FRIDAY;

	if (HAL_RTC_SetDate(&RtcHandle, &CurrentDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	CurrentTime.Hours = 0U;
	CurrentTime.Minutes = 0U;
	CurrentTime.Seconds = 0U;

	if (HAL_RTC_SetTime(&RtcHandle, &CurrentTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	PreviousSecond = 0U;

	/* We have no listeners initially */
	RtcListenersCount = 0;
	RtcListeners = malloc(0);
}

void RTC_Poll(void (*callbackPtr)())
{
	if (HAL_RTC_GetTime(&RtcHandle, &CurrentTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		L2HAL_Error(Generic);
	}

	if (PreviousSecond != CurrentTime.Seconds)
	{
		if (HAL_RTC_GetDate(&RtcHandle, &CurrentDate, RTC_FORMAT_BIN) != HAL_OK)
		{
			L2HAL_Error(Generic);
		}

		/* Calling listeners */
		for (uint8_t i = 0; i < RtcListenersCount; i++)
		{
			RtcListeners[i]();
		}
	}

	PreviousSecond = CurrentTime.Seconds;
}

void RTC_AddListener(void (*listener)())
{
	if (255 == RtcListenersCount)
	{
		/* No more, please */
		L2HAL_Error(Generic);
	}

	RtcListenersCount ++;
	RtcListeners = realloc(RtcListeners, RtcListenersCount * sizeof(void*));

	RtcListeners[RtcListenersCount - 1] = listener;
}