using Acr.UserDialogs;
using org.whitefossa.yiffhl.Abstractions.DTOs.Bootloader;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using OnResetLastErrorCodeDelegate = org.whitefossa.yiffhl.Abstractions.Interfaces.OnResetLastErrorCodeDelegate;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class ServiceCommandsManager : IServiceCommandsManager
    {
        /// <summary>
        /// We read FLASH by this sized pages
        /// </summary>
        private const uint FlashReadPageSize = 32;

        #region Commands

        private readonly IGetLastErrorCodeCommand _getLastErrorCodeCommand;
        private readonly IResetLastErrorCodeCommand _resetLastErrorCodeCommand;

        private readonly IUpdateSerialNumberCommand _updateSerialNumberCommand;

        private readonly IGetUbattADCCommand _getUbattADCCommand;
        private readonly IGetUbattVoltsCommand _getUbattVoltsCommand;

        private readonly IGetUbattFactorsCommand _getUbattFactorsCommand;
        private readonly ISetUbattFactorsCommand _setUbattFactorsCommand;

        private readonly IGetBattLevelFactorsCommand _getBattLevelFactorsCommand;
        private readonly ISetBattLevelFactorsCommand _setBattLevelFactorsCommand;

        private readonly IGetU80mADCCommand _getU80mADCCommand;
        private readonly IGetU80mVoltsCommand _getU80mVoltsCommand;

        private readonly IGetU80mFactorsCommand _getU80mFactorsCommand;
        private readonly ISetU80mFactorsCommand _setU80mFactorsCommand;

        private readonly IGetP80mFactorsCommand _getP80mFactorsCommand;
        private readonly ISetP80mFactorsCommand _setP80mFactorsCommand;

        private readonly IGetUantADCCommand _getUantADCCommand;
        private readonly IGetUantVoltsCommand _getUantVoltsCommand;

        private readonly IGetUantFactorsCommand _getUantFactorsCommand;
        private readonly ISetUantFactorsCommand _setUantFactorsCommand;

        private readonly IForceTxOnCommand _forceTxOnCommand;
        private readonly IReturnToNormalTxCommand _returnToNormalTxCommand;

        private readonly IGetRTCCalibrationValueCommand _getRTCCalibrationValueCommand;
        private readonly ISetRTCCalibrationValueCommand _setRTCCalibrationValueCommand;

        private readonly IGetDisarmOnDischargeThresholdCommand _getDisarmOnDischargeThresholdCommand;
        private readonly ISetDisarmOnDischargeThresholdCommand _setDisarmOnDischargeThresholdCommand;

        private readonly IRebootToBootloaderCommand _rebootToBootloaderCommand;

        #endregion

        #region Bootloader commands

        private readonly Abstractions.Interfaces.Commands.Bootloader.IGetIdentificationDataCommand _bootloaderGetIdentificationDataCommand;
        private readonly Abstractions.Interfaces.Commands.Bootloader.IRebootToMainFirmwareCommand _rebootToMainFirmwareCommand;
        private readonly Abstractions.Interfaces.Commands.Bootloader.IReadFlashPageCommand _readFlashPageCommand;

        #endregion

        private Abstractions.Interfaces.OnGetLastErrorCodeDelegate _onGetLastErrorCode;
        private Abstractions.Interfaces.OnResetLastErrorCodeDelegate _onResetLastErrorCode;

        private OnSerialNumberUpdateDelegate _onSerialNumberUpdate;
        private OnGetBatteryADCLevelDelegate _onGetBatteryADCLevel;

        private OnGetBatteryVoltageLevelDelegate _onGetBatteryVoltageLevel;

        private OnGetUbattFactorsDelegate _onGetUBattFactors;
        private OnResetUbattFactorsDelegate _onResetUbattFactors;
        private OnSetUbattFactorsDelegate _onSetUbattFactors;

        private OnGetBattLevelFactorsDelegate _onGetBattLevelFactors;
        private OnResetBattLevelFactorsDelegate _onResetBattLevelFactors;
        private OnSetBattLevelFactorsDelegate _onSetBattLevelFactors;

        private OnGetU80mADCValueDelegate _onGetU80mADCValue;
        private OnGetU80mVoltageDelegate _onGetU80mVoltage;

        private OnGetU80mFactorsDelegate _onGetU80mFactors;
        private OnResetU80mFactorsDelegate _onResetU80mFactors;
        private OnSetU80mFactorsDelegate _onSetU80mFactors;

        private OnGetP80mFactorsDelegate _onGetP80mFactors;
        private OnResetP80mFactorsDelegate _onResetP80mFactors;
        private OnSetP80mFactorsDelegate _onSetP80mFactors;

        private OnGetUantADCValueDelegate _onGetUantADCValue;
        private OnGetUantVoltageDelegate _onGetUantVoltage;

        private OnGetUantFactorsDelegate _onGetUantFactors;
        private OnResetUantFactorsDelegate _onResetUantFactors;
        private OnSetUantFactorsDelegate _onSetUantFactors;

        private OnForceTxOnDelegate _onForceTxOn;
        private OnReturnToNormalTxDelegate _onReturnToNormalTx;

        private OnGetRTCCalibrationValueDelegate _onGetRTCCalibrationValue;
        private OnSetRTCCalibrationValueDelegate _onSetRTCCalibrationValue;

        private OnGetDisarmOnDischargeThresholdDelegate _onGetDisarmOnDischargeThreshold;
        private OnSetDisarmOnDischargeThresholdDelegate _onSetDisarmOnDischargeThreshold;

        private Abstractions.Interfaces.OnRebootToBootloaderDelegate _onRebootToBootloader;

        #region Bootloader delegates

        private OnGetBootloaderIdentificationDataDelegate _onGetBootloaderIdentificationData;

        private OnRebootToMainFirmwareDelegate _onRebootToMainFirmware;

        private OnReadMainFirmwareDelegate _onReadMainFirmware;

        #endregion

        /// <summary>
        /// Bootloader size in 32-bytes FLASH read pages
        /// </summary>
        private uint _bootloaderSizeInReadPages;

        /// <summary>
        /// Main firmware size in 32-bytes FLASH read pages
        /// </summary>
        private uint _mainFirmwareSizeInReadPages;

        /// <summary>
        /// Main firmware last page (in 32-bytes FLASH read pages)
        /// </summary>
        private uint _mainFirmwareLastReadPage;

        /// <summary>
        /// Current main firmware page (reading)
        /// </summary>
        private uint _currentMainFirmwareReadPage;

        /// <summary>
        /// Main firmware dump accumulates here
        /// </summary>
        private List<byte> _mainFirmwareDump = new List<byte>();

        /// <summary>
        /// To display operations progress
        /// </summary>
        private IProgressDialog _progressDialog;

        public ServiceCommandsManager(IGetLastErrorCodeCommand getLastErrorCodeCommand,
            IResetLastErrorCodeCommand resetLastErrorCodeCommand,
            IUpdateSerialNumberCommand updateSerialNumberCommand,
            IGetUbattADCCommand getUbattADCCommand,
            IGetUbattVoltsCommand getUbattVoltsCommand,
            IGetUbattFactorsCommand getUbattFactorsCommand,
            ISetUbattFactorsCommand setUbattFactorsCommand,
            IGetBattLevelFactorsCommand getBattLevelFactorsCommand,
            ISetBattLevelFactorsCommand setBattLevelFactorsCommand,
            IGetU80mADCCommand getU80mADCCommand,
            IGetU80mVoltsCommand getU80mVoltsCommand,
            IGetU80mFactorsCommand getU80mFactorsCommand,
            ISetU80mFactorsCommand setU80mFactorsCommand,
            IGetP80mFactorsCommand getP80mFactorsCommand,
            ISetP80mFactorsCommand setP80mFactorsCommand,
            IGetUantADCCommand getUantADCCommand,
            IGetUantVoltsCommand getUantVoltsCommand,
            IGetUantFactorsCommand getUantFactorsCommand,
            ISetUantFactorsCommand setUantFactorsCommand,
            IForceTxOnCommand forceTxOnCommand,
            IReturnToNormalTxCommand returnToNormalTxCommand,
            IGetRTCCalibrationValueCommand getRTCCalibrationValueCommand,
            ISetRTCCalibrationValueCommand setRTCCalibrationValueCommand,
            IGetDisarmOnDischargeThresholdCommand getDisarmOnDischargeThresholdCommand,
            ISetDisarmOnDischargeThresholdCommand setDisarmOnDischargeThresholdCommand,
            IRebootToBootloaderCommand rebootToBootloaderCommand,
            Abstractions.Interfaces.Commands.Bootloader.IGetIdentificationDataCommand bootloaderGetIdentificationDataCommand,
            Abstractions.Interfaces.Commands.Bootloader.IRebootToMainFirmwareCommand rebootToMainFirmwareCommand,
            Abstractions.Interfaces.Commands.Bootloader.IReadFlashPageCommand readFlashPageCommand)
        {
            _getLastErrorCodeCommand = getLastErrorCodeCommand;
            _resetLastErrorCodeCommand = resetLastErrorCodeCommand;
            _updateSerialNumberCommand = updateSerialNumberCommand;
            _getUbattADCCommand = getUbattADCCommand;
            _getUbattVoltsCommand = getUbattVoltsCommand;
            _getUbattFactorsCommand = getUbattFactorsCommand;
            _setUbattFactorsCommand = setUbattFactorsCommand;
            _getBattLevelFactorsCommand = getBattLevelFactorsCommand;
            _setBattLevelFactorsCommand = setBattLevelFactorsCommand;
            _getU80mADCCommand = getU80mADCCommand;
            _getU80mVoltsCommand = getU80mVoltsCommand;
            _getU80mFactorsCommand = getU80mFactorsCommand;
            _setU80mFactorsCommand = setU80mFactorsCommand;
            _getP80mFactorsCommand = getP80mFactorsCommand;
            _setP80mFactorsCommand = setP80mFactorsCommand;
            _getUantADCCommand = getUantADCCommand;
            _getUantVoltsCommand = getUantVoltsCommand;
            _getUantFactorsCommand = getUantFactorsCommand;
            _setUantFactorsCommand = setUantFactorsCommand;
            _forceTxOnCommand = forceTxOnCommand;
            _returnToNormalTxCommand = returnToNormalTxCommand;
            _getRTCCalibrationValueCommand = getRTCCalibrationValueCommand;
            _setRTCCalibrationValueCommand = setRTCCalibrationValueCommand;
            _getDisarmOnDischargeThresholdCommand = getDisarmOnDischargeThresholdCommand;
            _setDisarmOnDischargeThresholdCommand = setDisarmOnDischargeThresholdCommand;
            _rebootToBootloaderCommand = rebootToBootloaderCommand;
            _bootloaderGetIdentificationDataCommand = bootloaderGetIdentificationDataCommand;
            _rebootToMainFirmwareCommand = rebootToMainFirmwareCommand;
            _readFlashPageCommand = readFlashPageCommand;
        }

        #region Get last error code

        public async Task GetLastErrorCodeAsync(Abstractions.Interfaces.OnGetLastErrorCodeDelegate onGetLastErrorCode)
        {
            _onGetLastErrorCode = onGetLastErrorCode ?? throw new ArgumentNullException(nameof(onGetLastErrorCode));

            _getLastErrorCodeCommand.SetResponseDelegate(OnGetLastErrorCodeResponse);
            _getLastErrorCodeCommand.SendGetLastErrorCodeCommand();
        }

        private void OnGetLastErrorCodeResponse(uint code)
        {
            _onGetLastErrorCode(code);
        }

        #endregion

        #region Reset last error code

        public async Task ResetLastErrorCodeAsync(OnResetLastErrorCodeDelegate onResetLastErrorCode)
        {
            _onResetLastErrorCode = onResetLastErrorCode ?? throw new ArgumentNullException(nameof(onResetLastErrorCode));

            _resetLastErrorCodeCommand.SetResponseDelegate(OnResetLastErrorCodeResponse);
            _resetLastErrorCodeCommand.SendResetLastErrorCodeCommand();
        }

        private void OnResetLastErrorCodeResponse()
        {
            _onResetLastErrorCode();
        }

        #endregion

        #region Update serial number

        public async Task UpdateSerialNumberAsync(uint newSerialNumber, OnSerialNumberUpdateDelegate onSerialNumberUpdate)
        {
            _onSerialNumberUpdate = onSerialNumberUpdate ?? throw new ArgumentNullException(nameof(onSerialNumberUpdate));

            _updateSerialNumberCommand.SetResponseDelegate(OnSerialNumberUpdated);
            _updateSerialNumberCommand.SendUpdateSerialNumberCommand(newSerialNumber);
        }

        private void OnSerialNumberUpdated()
        {
            _onSerialNumberUpdate();
        }

        #endregion

        #region Get battery level (ADC)

        public async Task GetBatteryADCLevelAsync(OnGetBatteryADCLevelDelegate onGetBatteryADCLevel)
        {
            _onGetBatteryADCLevel = onGetBatteryADCLevel ?? throw new ArgumentNullException(nameof(onGetBatteryADCLevel));

            _getUbattADCCommand.SetResponseDelegate(OnGetBatteryADCLevelResponse);
            _getUbattADCCommand.SendGetUbattADCCommand();
        }

        private void OnGetBatteryADCLevelResponse(float averagedADCLevel)
        {
            _onGetBatteryADCLevel(averagedADCLevel);
        }

        #endregion

        #region Get battery level (voltage)

        public async Task GetBatteryVoltageLevelAsync(OnGetBatteryVoltageLevelDelegate onGetBatteryVoltageLevel)
        {
            _onGetBatteryVoltageLevel = onGetBatteryVoltageLevel ?? throw new ArgumentNullException(nameof(onGetBatteryVoltageLevel));

            _getUbattVoltsCommand.SetResponseDelegate(OnGetBatteryVoltageLevelResponse);
            _getUbattVoltsCommand.SendGetUbattVoltsCommand();
        }

        private void OnGetBatteryVoltageLevelResponse(float averagedVoltageLevel)
        {
            _onGetBatteryVoltageLevel(averagedVoltageLevel);
        }

        #endregion

        #region Get UBatt(ADC) -> UBatt(Volts) factors

        public async Task GetUbattFactorsAsync(OnGetUbattFactorsDelegate onGetUBattFactors)
        {
            _onGetUBattFactors = onGetUBattFactors ?? throw new ArgumentNullException(nameof(onGetUBattFactors));

            _getUbattFactorsCommand.SetResponseDelegate(OnGetUBattFactorsResponse);
            _getUbattFactorsCommand.SendGetUbattFactorsCommand();
        }

        private void OnGetUBattFactorsResponse(float a, float b)
        {
            _onGetUBattFactors(a, b);
        }

        #endregion

        #region Reset Ubatt(ADC) -> Ubatt(Volts) factors

        public async Task ResetUbattFactorsAsync(OnResetUbattFactorsDelegate onResetUbattFactors)
        {
            _onResetUbattFactors = onResetUbattFactors;

            _setUbattFactorsCommand.SetResponseDelegate(OnResetUbattFactorsResponse);
            _setUbattFactorsCommand.SendSetUbattFactors(true, 0, 0);
        }

        private void OnResetUbattFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to reset Ubatt(ADC) -> Ubatt(Volts) factors!");
            }

            _onResetUbattFactors();
        }

        #endregion

        #region Set Ubatt(ADC) -> Ubatt(Volts) factors

        public async Task SetUbattFactorsAsync(float a, float b, OnSetUbattFactorsDelegate onSetUbattFactors)
        {
            _onSetUbattFactors = onSetUbattFactors;

            _setUbattFactorsCommand.SetResponseDelegate(OnSetUbattFactorsResponse);
            _setUbattFactorsCommand.SendSetUbattFactors(false, a, b);
        }

        private void OnSetUbattFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to set Ubatt(ADC) -> Ubatt(Volts) factors!");
            }

            _onSetUbattFactors();
        }

        #endregion

        #region Get UBatt(Volts) -> Battery level factors

        public async Task GetBattLevelFactorsAsync(OnGetBattLevelFactorsDelegate onGetBattLevelFactors)
        {
            _onGetBattLevelFactors = onGetBattLevelFactors ?? throw new ArgumentNullException(nameof(onGetBattLevelFactors));

            _getBattLevelFactorsCommand.SetResponseDelegate(OnGetBattLevelFactorsResponse);
            _getBattLevelFactorsCommand.SendGetBattLevelFactorsCommand();
        }

        private void OnGetBattLevelFactorsResponse(float a, float b)
        {
            _onGetBattLevelFactors(a, b);
        }

        #endregion

        #region Reset UBatt(Volts) -> Battery level factors

        public async Task ResetBattLevelFactorsAsync(OnResetBattLevelFactorsDelegate onResetBattLevelFactors)
        {
            _onResetBattLevelFactors = onResetBattLevelFactors;

            _setBattLevelFactorsCommand.SetResponseDelegate(OnResetBattLevelFactorsResponse);
            _setBattLevelFactorsCommand.SendSetBattLevelFactors(true, 0, 0);
        }

        private void OnResetBattLevelFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to reset UBatt(Volts) -> Battery level factors!");
            }

            _onResetBattLevelFactors();
        }

        #endregion

        #region Set UBatt(Volts) -> Battery level factors

        public async Task SetBattLevelFactorsAsync(float a, float b, OnSetBattLevelFactorsDelegate onSetBattLevelFactors)
        {
            _onSetBattLevelFactors = onSetBattLevelFactors;

            _setBattLevelFactorsCommand.SetResponseDelegate(OnSetBattLevelFactorsResponse);
            _setBattLevelFactorsCommand.SendSetBattLevelFactors(false, a, b);
        }

        private void OnSetBattLevelFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to set UBatt(Volts) -> Battery level factors!");
            }

            _onSetBattLevelFactors();
        }

        #endregion

        #region Get U80m ADC value

        public async Task GetU80mADCValueAsync(OnGetU80mADCValueDelegate onGetU80mADCValue)
        {
            _onGetU80mADCValue = onGetU80mADCValue ?? throw new ArgumentNullException(nameof(onGetU80mADCValue));

            _getU80mADCCommand.SetResponseDelegate(OnGetU80mADCValueResponse);
            _getU80mADCCommand.SendGetU80mADCCommand();
        }

        private void OnGetU80mADCValueResponse(float averagedADCValue)
        {
            _onGetU80mADCValue(averagedADCValue);
        }

        #endregion

        #region Get U80m Voltage

        public async Task GetU80mVoltageAsync(OnGetU80mVoltageDelegate onGetU80mVoltage)
        {
            _onGetU80mVoltage = onGetU80mVoltage ?? throw new ArgumentNullException(nameof(onGetU80mVoltage));

            _getU80mVoltsCommand.SetResponseDelegate(OnGetU80mVoltageResponse);
            _getU80mVoltsCommand.SendGetU80mVoltsCommand();
        }

        private void OnGetU80mVoltageResponse(float averagedVoltage)
        {
            _onGetU80mVoltage(averagedVoltage);
        }

        #endregion

        #region Get U80m(ADC) -> U80m(Volts) factors

        public async Task GetU80mFactorsAsync(OnGetU80mFactorsDelegate onGetU80mFactors)
        {
            _onGetU80mFactors = onGetU80mFactors ?? throw new ArgumentNullException(nameof(onGetU80mFactors));

            _getU80mFactorsCommand.SetResponseDelegate(OnGetU80mFactorsResponse);
            _getU80mFactorsCommand.SendGetU80mFactorsCommand();
        }

        private void OnGetU80mFactorsResponse(float a, float b)
        {
            _onGetU80mFactors(a, b);
        }

        #endregion

        #region Reset U80m(ADC) -> U80m(Volts) factors

        public async Task ResetU80mFactorsAsync(OnResetU80mFactorsDelegate onResetU80mFactors)
        {
            _onResetU80mFactors = onResetU80mFactors;

            _setU80mFactorsCommand.SetResponseDelegate(OnResetU80mFactorsResponse);
            _setU80mFactorsCommand.SendSetU80mFactors(true, 0, 0);
        }

        private void OnResetU80mFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to reset U80m(ADC) -> U80m(Volts) factors!");
            }

            _onResetU80mFactors();
        }

        #endregion

        #region Set U80m(ADC) -> U80m(Volts) factors

        public async Task SetU80mFactorsAsync(float a, float b, OnSetU80mFactorsDelegate onSetU80mFactors)
        {
            _onSetU80mFactors = onSetU80mFactors;

            _setU80mFactorsCommand.SetResponseDelegate(OnSetU80mFactorsResponse);
            _setU80mFactorsCommand.SendSetU80mFactors(false, a, b);
        }

        private void OnSetU80mFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to set U80m(ADC) -> U80m(Volts) factors!");
            }

            _onSetU80mFactors();
        }

        #endregion

        #region Get P80m -> U80m factors

        public async Task GetP80mFactorsAsync(OnGetP80mFactorsDelegate onGetP80mFactors)
        {
            _onGetP80mFactors = onGetP80mFactors ?? throw new ArgumentNullException(nameof(onGetP80mFactors));

            _getP80mFactorsCommand.SetResponseDelegate(OnGetP80mFactorsResponse);
            _getP80mFactorsCommand.SendGetP80mFactorsCommand();
        }

        private void OnGetP80mFactorsResponse(float a, float b)
        {
            _onGetP80mFactors(a, b);
        }

        #endregion

        #region Reset P80m -> U80m factors

        public async Task ResetP80mFactorsAsync(OnResetP80mFactorsDelegate onResetP80mFactors)
        {
            _onResetP80mFactors = onResetP80mFactors ?? throw new ArgumentNullException(nameof(onResetP80mFactors));

            _setP80mFactorsCommand.SetResponseDelegate(OnResetP80mFactorsResponse);
            _setP80mFactorsCommand.SendSetP80mFactors(true, 0, 0);
        }

        private void OnResetP80mFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to reset P80m -> U80m factors!");
            }

            _onResetP80mFactors();
        }

        #endregion

        #region Set P80m -> U80m factors

        public async Task SetP80mFactorsAsync(float a, float b, OnSetP80mFactorsDelegate onSetP80mFactors)
        {
            _onSetP80mFactors = onSetP80mFactors ?? throw new ArgumentNullException(nameof(onSetP80mFactors));

            _setP80mFactorsCommand.SetResponseDelegate(OnSetP80mFactorsResponse);
            _setP80mFactorsCommand.SendSetP80mFactors(false, a, b);
        }

        private void OnSetP80mFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to set P80m -> U80m factors!");
            }

            _onSetP80mFactors();
        }

        #endregion

        #region Get Uant(ADC)

        public async Task GetUantADCValueAsync(OnGetUantADCValueDelegate onGetUantADCValue)
        {
            _onGetUantADCValue = onGetUantADCValue ?? throw new ArgumentNullException(nameof(onGetUantADCValue));

            _getUantADCCommand.SetResponseDelegate(OnGetUantADCValueResponse);
            _getUantADCCommand.SendGetUantADCCommand();
        }

        private void OnGetUantADCValueResponse(float averagedADCValue)
        {
            _onGetUantADCValue(averagedADCValue);
        }

        #endregion

        #region Get Uant(Volts)

        public async Task GetUantVoltageAsync(OnGetUantVoltageDelegate onGetUantVoltage)
        {
            _onGetUantVoltage = onGetUantVoltage ?? throw new ArgumentNullException(nameof(onGetUantVoltage));

            _getUantVoltsCommand.SetResponseDelegate(OnGetUantVoltageResponse);
            _getUantVoltsCommand.SendGetUantVoltsCommand();
        }

        private void OnGetUantVoltageResponse(float averagedVoltage)
        {
            _onGetUantVoltage(averagedVoltage);
        }

        #endregion

        #region Get Uant(ADC) -> Uant(Volts) factors

        public async Task GetUantFactorsAsync(OnGetUantFactorsDelegate onGetUantFactors)
        {
            _onGetUantFactors = onGetUantFactors ?? throw new ArgumentNullException(nameof(onGetUantFactors));

            _getUantFactorsCommand.SetResponseDelegate(OnGetUantFactorsResponse);
            _getUantFactorsCommand.SendGetUantFactorsCommand();
        }

        private void OnGetUantFactorsResponse(float a, float b)
        {
            _onGetUantFactors(a, b);
        }

        #endregion

        #region Reset Uant(ADC) -> Uant(Volts) factors

        public async Task ResetUantFactorsAsync(OnResetUantFactorsDelegate onResetUantFactors)
        {
            _onResetUantFactors = onResetUantFactors ?? throw new ArgumentNullException(nameof(onResetUantFactors));

            _setUantFactorsCommand.SetResponseDelegate(OnResetUantFactorsResponse);
            _setUantFactorsCommand.SendSetUantFactors(true, 0, 0);
        }

        private void OnResetUantFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to reset Uant(ADC) -> Uant(Volts) factors!");
            }

            _onResetUantFactors();
        }

        #endregion

        #region Set Uant(ADC) -> Uant(Volts) factors

        public async Task SetUantFactorsAsync(float a, float b, OnSetUantFactorsDelegate onSetUantFactors)
        {
            _onSetUantFactors = onSetUantFactors ?? throw new ArgumentNullException(nameof(onSetUantFactors));

            _setUantFactorsCommand.SetResponseDelegate(OnSetUantFactorsResponse);
            _setUantFactorsCommand.SendSetUantFactors(false, a, b);
        }

        private void OnSetUantFactorsResponse(bool isSuccessful)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException("Failed to set Uant(ADC) -> Uant(Volts) factors!");
            }

            _onSetUantFactors();
        }

        #endregion

        #region Force TX On

        public async Task ForceTxOnAsync(OnForceTxOnDelegate onForceTxOn)
        {
            _onForceTxOn = onForceTxOn ?? throw new ArgumentNullException(nameof(onForceTxOn));

            _forceTxOnCommand.SetResponseDelegate(OnForceTxOnResponse);
            _forceTxOnCommand.SendForceTxOnCommand();
        }

        private void OnForceTxOnResponse(bool isSuccessful)
        {
            _onForceTxOn(isSuccessful);
        }

        #endregion

        #region Return to normal TX

        public async Task ReturnToNormalTxAsync(OnReturnToNormalTxDelegate onReturnToNormalTx)
        {
            _onReturnToNormalTx = onReturnToNormalTx ?? throw new ArgumentNullException(nameof(onReturnToNormalTx));

            _returnToNormalTxCommand.SetResponseDelegate(OnReturnToNormalTxResponse);
            _returnToNormalTxCommand.SendReturnToNormalTxCommand();
        }

        private void OnReturnToNormalTxResponse(bool isSuccessful)
        {
            _onReturnToNormalTx(isSuccessful);
        }

        #endregion

        #region Get RTC calibration value

        public async Task GetRTCCalibrationValueAsync(OnGetRTCCalibrationValueDelegate onGetRTCCalibrationValue)
        {
            _onGetRTCCalibrationValue = onGetRTCCalibrationValue ?? throw new ArgumentNullException(nameof(onGetRTCCalibrationValue));

            _getRTCCalibrationValueCommand.SetResponseDelegate(OnGetRTCCalibrationValueResponse);
            _getRTCCalibrationValueCommand.SendGetRTCCalibrationValue();
        }

        private void OnGetRTCCalibrationValueResponse(uint value)
        {
            _onGetRTCCalibrationValue(value);
        }

        #endregion

        #region Set RTC calibration value

        public async Task SetRTCCalibrationValueAsync(uint newValue, OnSetRTCCalibrationValueDelegate onSetRTCCalibrationValue)
        {
            _onSetRTCCalibrationValue = onSetRTCCalibrationValue ?? throw new ArgumentNullException(nameof(onSetRTCCalibrationValue));

            _setRTCCalibrationValueCommand.SetResponseDelegate(OnSetRTCCalibrationValueResponse);
            _setRTCCalibrationValueCommand.SendSetRTCCalibrationValue(newValue);
        }

        private void OnSetRTCCalibrationValueResponse(bool isSuccessful)
        {
            _onSetRTCCalibrationValue(isSuccessful);
        }

        #endregion

        #region Get disarm on discharge threshold

        public async Task GetDisarmOnDischargeThresholdAsync(OnGetDisarmOnDischargeThresholdDelegate onGetDisarmOnDischargeThreshold)
        {
            _onGetDisarmOnDischargeThreshold = onGetDisarmOnDischargeThreshold ?? throw new ArgumentNullException();

            _getDisarmOnDischargeThresholdCommand.SetResponseDelegate(OnGetDisarmOnDischargeThresholdResponse);
            _getDisarmOnDischargeThresholdCommand.SendGetDisarmOnDischargeThreshold();
        }

        private void OnGetDisarmOnDischargeThresholdResponse(float threshold)
        {
            _onGetDisarmOnDischargeThreshold(threshold);
        }

        #endregion

        #region Set disarm on discharge threshold

        public async Task SetDisarmOnDischargeThresholdAsync(float newThreshold, OnSetDisarmOnDischargeThresholdDelegate onSetDisarmOnDischargeThreshold)
        {
            _onSetDisarmOnDischargeThreshold = onSetDisarmOnDischargeThreshold;

            _setDisarmOnDischargeThresholdCommand.SetResponseDelegate(OnSetDisarmOnDischargeThresholdResponse);
            _setDisarmOnDischargeThresholdCommand.SendSetDisarmOnDischargeThresholdCommand(newThreshold);
        }

        private void OnSetDisarmOnDischargeThresholdResponse(bool isSuccessful)
        {
            _onSetDisarmOnDischargeThreshold(isSuccessful);
        }

        #endregion

        #region Reboot to bootloader

        public async Task RebootToBootloaderAsync(Abstractions.Interfaces.OnRebootToBootloaderDelegate onRebootToBootloader)
        {
            _onRebootToBootloader = onRebootToBootloader;

            _rebootToBootloaderCommand.SetResponseDelegate(OnRebootToBootloaderResponse);
            _rebootToBootloaderCommand.SendRebootToBootloaderCommand();
        }

        private void OnRebootToBootloaderResponse(bool isSuccessful)
        {
            _onRebootToBootloader(isSuccessful);
        }

        #endregion

        #region Get bootloader identification data

        public async Task GetBootloaderIdentificationData(OnGetBootloaderIdentificationDataDelegate onGetBootloaderIdentificationData)
        {
            _onGetBootloaderIdentificationData = onGetBootloaderIdentificationData;

            _bootloaderGetIdentificationDataCommand.SetResponseDelegate(OnGetBootloaderIdentificationDataResponse);
            _bootloaderGetIdentificationDataCommand.SendGetBootloaderIdentificationDataCommand();
        }

        private void OnGetBootloaderIdentificationDataResponse
        (
            bool isFoxBootloader,
            UInt16 protocolVersion,
            UInt16 hardwareRevision,
            UInt16 softwareVersion,
            UInt32 flashStartAddress,
            UInt32 mainFirmwareStartAddress,
            UInt32 flashEndAddress
        )
        {
            _onGetBootloaderIdentificationData
            (
                isFoxBootloader,
                protocolVersion,
                hardwareRevision,
                softwareVersion,
                flashStartAddress,
                mainFirmwareStartAddress,
                flashEndAddress
            );
        }

        #endregion

        #region Reboot to main firmware

        public async Task RebootToMainFirmware(OnRebootToMainFirmwareDelegate onRebootToMainFirmware)
        {
            _onRebootToMainFirmware = onRebootToMainFirmware;

            _rebootToMainFirmwareCommand.SetResponseDelegate(OnRebootToMainFirmwareResponse);
            _rebootToMainFirmwareCommand.RebootToMainFirmware();
        }

        private void OnRebootToMainFirmwareResponse()
        {
            _onRebootToMainFirmware();
        }

        #endregion

        #region Read main firmware

        public async Task ReadMainFirmware(BootloaderIdentificationData bootloaderIdentificationData, OnReadMainFirmwareDelegate onReadMainFirmware)
        {
            // Calculating main firmware size
            _bootloaderSizeInReadPages
                = (bootloaderIdentificationData.MainFirmwareStartAddress - bootloaderIdentificationData.FlashStartAddress) / FlashReadPageSize;

            _mainFirmwareSizeInReadPages
                = (bootloaderIdentificationData.FlashEndAddress + 1 - bootloaderIdentificationData.MainFirmwareStartAddress) / FlashReadPageSize;

            _mainFirmwareLastReadPage = _bootloaderSizeInReadPages + _mainFirmwareSizeInReadPages;

            _onReadMainFirmware = onReadMainFirmware;

            _readFlashPageCommand.SetResponseDelegate(async(s, d) => await OnReadFlashPageFromMainFirmwareAsync(s, d));

            _progressDialog = UserDialogs.Instance.Progress("Reading FLASH", null, null, true, MaskType.Clear);

            _mainFirmwareDump.Clear();

            _currentMainFirmwareReadPage = _bootloaderSizeInReadPages; // First page of the main firmware
            _readFlashPageCommand.SendReadFlashPageCommand(_currentMainFirmwareReadPage);
        }

        private async Task OnReadFlashPageFromMainFirmwareAsync(bool isSuccessful, List<byte> data)
        {
            if (!isSuccessful)
            {
                throw new InvalidOperationException($"Failed to read FLASH page { _currentMainFirmwareReadPage }");
            }

            _mainFirmwareDump.AddRange(data);

            _progressDialog.PercentComplete = 
                (int)Math.Round(100 * (_currentMainFirmwareReadPage - _bootloaderSizeInReadPages) / (double)_mainFirmwareSizeInReadPages);

            _currentMainFirmwareReadPage ++;

            if (_currentMainFirmwareReadPage < _mainFirmwareLastReadPage)
            {
                // Continue to read
                _readFlashPageCommand.SendReadFlashPageCommand(_currentMainFirmwareReadPage);
                return;
            }

            // Completed
            _progressDialog.Dispose();
            _onReadMainFirmware(_mainFirmwareDump);
        }

        #endregion
    }
}
