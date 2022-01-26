using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;
using OnResetLastErrorCodeDelegate = org.whitefossa.yiffhl.Abstractions.Interfaces.OnResetLastErrorCodeDelegate;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class ServiceCommandsManager : IServiceCommandsManager
    {
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
            ISetU80mFactorsCommand setU80mFactorsCommand)
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

        private void OnResetUbattFactorsResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
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

        private void OnSetUbattFactorsResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
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

        private void OnResetBattLevelFactorsResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
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

        private void OnSetBattLevelFactorsResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
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

        public async Task ResetU80mFactorsAsync(OnResetU80mFactorsDelegate onResetu80mFactors)
        {
            _onResetU80mFactors = onResetu80mFactors;

            _setU80mFactorsCommand.SetResponseDelegate(OnResetU80mFactorsResponse);
            _setU80mFactorsCommand.SendSetU80mFactors(true, 0, 0);
        }

        private void OnResetU80mFactorsResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
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

        private void OnSetU80mFactorsResponse(bool isSuccessfull)
        {
            if (!isSuccessfull)
            {
                throw new InvalidOperationException("Failed to set U80m(ADC) -> U80m(Volts) factors!");
            }

            _onSetU80mFactors();
        }

        #endregion
    }
}
