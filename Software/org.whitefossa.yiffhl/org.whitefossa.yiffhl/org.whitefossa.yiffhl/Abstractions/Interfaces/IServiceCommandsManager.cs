using org.whitefossa.yiffhl.Abstractions.DTOs.Bootloader;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    public delegate void OnGetLastErrorCodeDelegate(uint lastErrorCode);

    public delegate void OnResetLastErrorCodeDelegate();

    public delegate void OnSerialNumberUpdateDelegate();

    public delegate void OnGetBatteryADCLevelDelegate(float averagedADCLevel);
    public delegate void OnGetBatteryVoltageLevelDelegate(float averagedVoltageLevel);

    public delegate void OnGetUbattFactorsDelegate(float a, float b);
    public delegate void OnResetUbattFactorsDelegate();
    public delegate void OnSetUbattFactorsDelegate();

    public delegate void OnGetBattLevelFactorsDelegate(float a, float b);
    public delegate void OnResetBattLevelFactorsDelegate();
    public delegate void OnSetBattLevelFactorsDelegate();

    public delegate void OnGetU80mADCValueDelegate(float averagedADCValue);
    public delegate void OnGetU80mVoltageDelegate(float averagedVoltage);

    public delegate void OnGetU80mFactorsDelegate(float a, float b);
    public delegate void OnResetU80mFactorsDelegate();
    public delegate void OnSetU80mFactorsDelegate();

    public delegate void OnGetP80mFactorsDelegate(float a, float b);
    public delegate void OnResetP80mFactorsDelegate();
    public delegate void OnSetP80mFactorsDelegate();

    public delegate void OnGetUantADCValueDelegate(float averagedADCValue);
    public delegate void OnGetUantVoltageDelegate(float averagedVoltage);

    public delegate void OnGetUantFactorsDelegate(float a, float b);
    public delegate void OnResetUantFactorsDelegate();
    public delegate void OnSetUantFactorsDelegate();

    public delegate void OnForceTxOnDelegate(bool isSuccessful);
    public delegate void OnReturnToNormalTxDelegate(bool isSuccessful);

    public delegate void OnGetRTCCalibrationValueDelegate(uint value);
    public delegate void OnSetRTCCalibrationValueDelegate(bool isSuccessful);

    public delegate void OnGetDisarmOnDischargeThresholdDelegate(float threshold);
    public delegate void OnSetDisarmOnDischargeThresholdDelegate(bool isSuccessful);

    public delegate void OnRebootToBootloaderDelegate(bool isSuccessful);

    public delegate void OnGetBootloaderIdentificationDataDelegate
    (
        bool isFoxBootloader,
        UInt16 protocolVersion,
        UInt16 hardwareRevision,
        UInt16 softwareVersion,
        UInt32 flashStartAddress,
        UInt32 mainFirmwareStartAddress,
        UInt32 flashEndAddress
    );

    public delegate void OnRebootToMainFirmwareDelegate();

    public delegate void OnReadMainFirmwareDelegate(List<byte> mainFirmwareDump);

    public interface IServiceCommandsManager
    {
        Task GetLastErrorCodeAsync(OnGetLastErrorCodeDelegate onGetLastErrorCode);

        Task ResetLastErrorCodeAsync(OnResetLastErrorCodeDelegate onResetLastErrorCode);

        Task UpdateSerialNumberAsync(uint newSerialNumber, OnSerialNumberUpdateDelegate onSerialNumberUpdate);

        Task GetBatteryADCLevelAsync(OnGetBatteryADCLevelDelegate onGetBatteryADCLevel);
        Task GetBatteryVoltageLevelAsync(OnGetBatteryVoltageLevelDelegate onGetBatteryVoltageLevel);

        Task GetUbattFactorsAsync(OnGetUbattFactorsDelegate onGetUBattFactors);
        Task ResetUbattFactorsAsync(OnResetUbattFactorsDelegate onResetUbattFactors);
        Task SetUbattFactorsAsync(float a, float b, OnSetUbattFactorsDelegate onSetUbattFactors);

        Task GetBattLevelFactorsAsync(OnGetBattLevelFactorsDelegate onGetBattLevelFactors);
        Task ResetBattLevelFactorsAsync(OnResetBattLevelFactorsDelegate onResetBattLevelFactors);
        Task SetBattLevelFactorsAsync(float a, float b, OnSetBattLevelFactorsDelegate onSetBattLevelFactors);

        Task GetU80mADCValueAsync(OnGetU80mADCValueDelegate onGetU80mADCValue);
        Task GetU80mVoltageAsync(OnGetU80mVoltageDelegate onGetU80mVoltage);

        Task GetU80mFactorsAsync(OnGetU80mFactorsDelegate onGetU80mFactors);
        Task ResetU80mFactorsAsync(OnResetU80mFactorsDelegate onResetU80mFactors);
        Task SetU80mFactorsAsync(float a, float b, OnSetU80mFactorsDelegate onSetU80mFactors);

        Task GetP80mFactorsAsync(OnGetP80mFactorsDelegate onGetP80mFactors);
        Task ResetP80mFactorsAsync(OnResetP80mFactorsDelegate onResetP80mFactors);
        Task SetP80mFactorsAsync(float a, float b, OnSetP80mFactorsDelegate onSetP80mFactors);

        Task GetUantADCValueAsync(OnGetUantADCValueDelegate onGetUantADCValue);
        Task GetUantVoltageAsync(OnGetUantVoltageDelegate onGetUantVoltage);

        Task GetUantFactorsAsync(OnGetUantFactorsDelegate onGetUantFactors);
        Task ResetUantFactorsAsync(OnResetUantFactorsDelegate onResetUantFactors);
        Task SetUantFactorsAsync(float a, float b, OnSetUantFactorsDelegate onSetUantFactors);

        Task ForceTxOnAsync(OnForceTxOnDelegate onForceTxOn);
        Task ReturnToNormalTxAsync(OnReturnToNormalTxDelegate onReturnToNormalTx);

        Task GetRTCCalibrationValueAsync(OnGetRTCCalibrationValueDelegate onGetRTCCalibrationValue);
        Task SetRTCCalibrationValueAsync(uint newValue, OnSetRTCCalibrationValueDelegate onSetRTCCalibrationValue);

        Task GetDisarmOnDischargeThresholdAsync(OnGetDisarmOnDischargeThresholdDelegate onGetDisarmOnDischargeThreshold);
        Task SetDisarmOnDischargeThresholdAsync(float newThreshold, OnSetDisarmOnDischargeThresholdDelegate onSetDisarmOnDischargeThreshold);

        Task RebootToBootloaderAsync(OnRebootToBootloaderDelegate onRebootToBootloader);

        Task GetBootloaderIdentificationData(OnGetBootloaderIdentificationDataDelegate onGetBootloaderIdentificationData);

        Task RebootToMainFirmware(OnRebootToMainFirmwareDelegate onRebootToMainFirmware);

        Task ReadMainFirmware(BootloaderIdentificationData bootloaderIdentificationData, OnReadMainFirmwareDelegate onReadMainFirmware);
    }
}
