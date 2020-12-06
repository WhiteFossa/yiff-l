#include <l2hal_adf4001.h>
#include <l2hal_adf4001_private.h>

void WriteToPll(L2HAL_ADF4001_BytesToSendStruct bytes)
{
	/**
	 * TODO: Implement it
	 */
}

uint8_t L2HAL_ADF4001_WriteReferenceCounter(L2HAL_ADF4001_ReferenceCounterStruct* data)
{
	L2HAL_ADF4001_BytesToSendStruct toWrite;
	toWrite.most = 0x00;
	toWrite.middle = 0x00;
	toWrite.least = 0x00;

	/* Control bits for reference counter latch */
	toWrite.least |= L2HAL_ADF4001_CONTROLBITS_R;

	/* Selecting 6 least significant bits of R counter, shifting it left to 2 and writing into least byte */
	toWrite.least |= ((data->ReferenceCounter & 0b111111) << 2);

	/* Selecting 8 most significant bits of R counter, shifting it right to 6 and writing into middle byte */
	toWrite.middle |= ((data->ReferenceCounter & 0b11111111000000) >> 6);

	/* Anti-backlash */
	switch(data->AntiBacklashWidth)
	{
		case PulseWidth1_3:
			toWrite.most |= (1 << L2HAL_ADF4001_ANTIBACKLASH0);
		break;

		case PulseWidth6_0:
			toWrite.most |= (1 << L2HAL_ADF4001_ANTIBACKLASH1);
		break;
	}

	/* Lock detect precision */
	if (data->LockDetectPrecision == Cycles5)
	{
		toWrite.most |= (1 << L2HAL_ADF4001_LOCK_DETECT_PRECISION);
	}

	return toWrite.most;
}

