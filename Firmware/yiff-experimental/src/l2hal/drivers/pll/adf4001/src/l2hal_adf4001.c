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
		case ABWPulseWidth1_3:
			toWrite.Most |= (1 << L2HAL_ADF4001_ANTIBACKLASH0);
		break;

		case ABWPulseWidth6_0:
			toWrite.Most |= (1 << L2HAL_ADF4001_ANTIBACKLASH1);
		break;
	}

	/* Lock detect precision */
	if (data->LockDetectPrecision == LDPCycles5)
	{
		toWrite.Most |= (1 << L2HAL_ADF4001_LOCK_DETECT_PRECISION);
	}

	L2HAL_ADF4001_WriteToPll(toWrite);
}


void L2HAL_ADF4001_WriteNCounter(L2HAL_ADF4001_NCounterStruct* data)
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

	if (data->CPGain == CPGain2)
	{
		toWrite.Most |= (1 << L2HAL_ADF4001_CP_GAIN);
	}

	L2HAL_ADF4001_WriteToPll(toWrite);
}

void L2HAL_ADF4001_WriteFunctionLatch(L2HAL_ADF4001_FunctionStruct* data)
{
	L2HAL_ADF4001_BytesToSendStruct toWrite = L2HAL_ADF4001_PrepareFinctionAndInitializationData(data);

	toWrite.Least |= L2HAL_ADF4001_CONTROLBITS_FUNCTION;

	L2HAL_ADF4001_WriteToPll(toWrite);
}

void L2HAL_ADF4001_WriteInitializationLatch(L2HAL_ADF4001_FunctionStruct* data)
{
	L2HAL_ADF4001_BytesToSendStruct toWrite = L2HAL_ADF4001_PrepareFinctionAndInitializationData(data);

	toWrite.Least |= L2HAL_ADF4001_CONTROLBITS_INITIALIZATION;

	L2HAL_ADF4001_WriteToPll(toWrite);
}

L2HAL_ADF4001_BytesToSendStruct L2HAL_ADF4001_PrepareFinctionAndInitializationData(L2HAL_ADF4001_FunctionStruct* data)
{
	L2HAL_ADF4001_BytesToSendStruct toWrite;
	toWrite.Most = 0x00;
	toWrite.Middle = 0x00;
	toWrite.Least = 0x00;

	if (data->CountersReset == CRHoldInReset)
	{
		toWrite.Least |= (1 << L2HAL_ADF4001_HOLD_IN_RESET);
	}

	switch (data->PowerDownMode)
	{
		case PDNAsynchronousPowerDown:
			toWrite.Least |= (1 << L2HAL_ADF4001_POWER_DOWN1);
		break;

		case PDNSynchronousPowerDown:
			toWrite.Least |= (1 << L2HAL_ADF4001_POWER_DOWN1);
			toWrite.Most |= (1 << L2HAL_ADF4001_POWER_DOWN2);
		break;
	}

	switch (data->MuxoutControl)
	{
		case MCDigitalLockDetect:
			toWrite.Least |= (1 << L2HAL_ADF4001_MUXOUT1);
		break;
		case MCNDivider:
			toWrite.Least |= (1 << L2HAL_ADF4001_MUXOUT2);
		break;
		case MCAVDD:
			toWrite.Least |= (1 << L2HAL_ADF4001_MUXOUT2) | (1 << L2HAL_ADF4001_MUXOUT1);
		break;
		case MCRDivider:
			toWrite.Least |= (1 << L2HAL_ADF4001_MUXOUT3);
		break;
		case MCOpenDrainLockDetect:
			toWrite.Least |= (1 << L2HAL_ADF4001_MUXOUT3) | (1 << L2HAL_ADF4001_MUXOUT1);
		break;
		case MCSerialDataOutput:
			toWrite.Least |= (1 << L2HAL_ADF4001_MUXOUT3) | (1 << L2HAL_ADF4001_MUXOUT2);
		break;
		case MCDGND:
			toWrite.Least |= (1 << L2HAL_ADF4001_MUXOUT3) | (1 << L2HAL_ADF4001_MUXOUT2) | (1 << L2HAL_ADF4001_MUXOUT1);
		break;
	}

	if (data->PhaseDetectorPolarity == PDPPositive)
	{
		toWrite.Least |= (1 << L2HAL_ADF4001_PHASE_DETECTOR_POLARITY);
	}

	if (data->CPThreeState == CPThreeState)
	{
		toWrite.Middle |= (1 << L2HAL_ADF4001_CHARGE_PUMP_THREESTATE);
	}

	if (data->FastlockEnable == FLEnabled)
	{
		toWrite.Middle |= (1 << L2HAL_ADF4001_FASTLOCK_ENABLE);
	}

	if (data->FastlockMode == FLMode2)
	{
		toWrite.Middle |= (1 << L2HAL_ADF4001_FASTLOCK_MODE);
	}

	switch (data->TimerCounterControl)
	{
		case TCC_7:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC0);
		break;
		case TCC_11:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC1);
		break;
		case TCC_15:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC1) | (1 << L2HAL_ADF4001_TCC0);
		break;
		case TCC_19:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC2);
		break;
		case TCC_23:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC2) | (1 << L2HAL_ADF4001_TCC0);
		break;
		case TCC_27:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC2) | (1 << L2HAL_ADF4001_TCC1);
		break;
		case TCC_31:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC2) | (1 << L2HAL_ADF4001_TCC1) | (1 << L2HAL_ADF4001_TCC0);
		break;
		case TCC_35:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC0);
		break;
		case TCC_39:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC3) | (1 << L2HAL_ADF4001_TCC0);
		break;
		case TCC_43:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC3) | (1 << L2HAL_ADF4001_TCC1);
		break;
		case TCC_47:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC3) | (1 << L2HAL_ADF4001_TCC1) | (1 << L2HAL_ADF4001_TCC0);
		break;
		case TCC_51:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC3) | (1 << L2HAL_ADF4001_TCC2);
		break;
		case TCC_55:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC3) | (1 << L2HAL_ADF4001_TCC2) | (1 << L2HAL_ADF4001_TCC0);
		break;
		case TCC_59:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC3) | (1 << L2HAL_ADF4001_TCC2) | (1 << L2HAL_ADF4001_TCC1);
		break;
		case TCC_63:
			toWrite.Middle |= (1 << L2HAL_ADF4001_TCC3) | (1 << L2HAL_ADF4001_TCC2) | (1 << L2HAL_ADF4001_TCC1) | (1 << L2HAL_ADF4001_TCC0);
		break;
	}

	/* Current setting 1 */
	uint8_t currentSetting = L2HAL_ADF4001_GenerateCurrentSettingsByte(data->CurrentSetting1);
	toWrite.Middle |= ((currentSetting & 0b1) << 7);
	toWrite.Most |= ((currentSetting & 0b110) >> 1);

	/* Current setting 2 */
	currentSetting = L2HAL_ADF4001_GenerateCurrentSettingsByte(data->CurrentSetting2);
	toWrite.Most |= ((currentSetting & 0b111) << 2);

	return toWrite;
}

uint8_t L2HAL_ADF4001_GenerateCurrentSettingsByte(enum L2HAL_ADF4001_CurrentSetting current)
{
	uint8_t result = 0;

	switch(current)
	{
		case CS1_25:
			result |= (1 << L2HAL_ADF4001_CURRENT_SETTING0);
		break;
		case CS1_875:
			result |= (1 << L2HAL_ADF4001_CURRENT_SETTING1);
		break;
		case CS2_5:
			result |= (1 << L2HAL_ADF4001_CURRENT_SETTING1) | (1 << L2HAL_ADF4001_CURRENT_SETTING0);
		break;
		case CS3_125:
			result |= (1 << L2HAL_ADF4001_CURRENT_SETTING2);
		break;
		case CS3_75:
			result |= (1 << L2HAL_ADF4001_CURRENT_SETTING2) | (1 << L2HAL_ADF4001_CURRENT_SETTING0);
		break;
		case CS4_375:
			result |= (1 << L2HAL_ADF4001_CURRENT_SETTING2) | (1 << L2HAL_ADF4001_CURRENT_SETTING1);
		break;
		case CS5_0:
			result |= (1 << L2HAL_ADF4001_CURRENT_SETTING2) | (1 << L2HAL_ADF4001_CURRENT_SETTING1) | (1 << L2HAL_ADF4001_CURRENT_SETTING0);
		break;
	}

	return result;
}

