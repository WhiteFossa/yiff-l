#include <l2hal_adf4001.h>
#include <l2hal_adf4001_private.h>

void L2HAL_ADF4001_WriteToPll(L2HAL_ADF4001_BytesToSendStruct bytes)
{
	/**
	 * TODO: Implement it
	 */
}

void L2HAL_ADF4001_WriteReferenceCounter(L2HAL_ADF4001_ReferenceCounterStruct* data)
{
	L2HAL_ADF4001_BytesToSendStruct toWrite;
	toWrite.Most = 0x00;
	toWrite.Middle = 0x00;
	toWrite.Least = 0x00;

	/* Control bits for reference counter latch */
	toWrite.Least |= L2HAL_ADF4001_CONTROLBITS_R;

	/* Selecting 6 least significant bits of R counter, shifting it left to 2 and writing into least byte */
	toWrite.Least |= ((data->ReferenceCounter & 0b111111) << 2);

	/* Selecting 8 most significant bits of R counter, shifting it right to 6 and writing into middle byte */
	toWrite.Middle |= ((data->ReferenceCounter & 0b11111111000000) >> 6);

	/* Anti-backlash */
	switch(data->AntiBacklashWidth)
	{
		case PulseWidth1_3:
			toWrite.Most |= (1 << L2HAL_ADF4001_ANTIBACKLASH0);
		break;

		case PulseWidth6_0:
			toWrite.Most |= (1 << L2HAL_ADF4001_ANTIBACKLASH1);
		break;
	}

	/* Lock detect precision */
	if (data->LockDetectPrecision == Cycles5)
	{
		toWrite.Most |= (1 << L2HAL_ADF4001_LOCK_DETECT_PRECISION);
	}

	L2HAL_ADF4001_WriteToPll(toWrite);
}


uint8_t L2HAL_ADF4001_WriteNCounter(L2HAL_ADF4001_NCounterStruct* data)
{
	L2HAL_ADF4001_BytesToSendStruct toWrite;
	toWrite.Most = 0x00;
	toWrite.Middle = 0x00;
	toWrite.Least = 0x00;

	/* Control bits for N counter latch */
	toWrite.Least |= L2HAL_ADF4001_CONTROLBITS_N;

	/* Selecting 8 least bits of N-counter and putting it into middle byte */
	toWrite.Middle |= (data->NCounter & 0b11111111);

	/* Selecting 5 most significant bits of N-counter, shifting it right to 8 and writting into most byte */
	toWrite.Most |= ((data->NCounter & 0b1111100000000) >> 8);

	if (data->CPGain == CP_GAIN_2)
	{
		toWrite.Most |= (1 << L2HAL_ADF4001_CP_GAIN);
	}

	return toWrite.Most;
}

