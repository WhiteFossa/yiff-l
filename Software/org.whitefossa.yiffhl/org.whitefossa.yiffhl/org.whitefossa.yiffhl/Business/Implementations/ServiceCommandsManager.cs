using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;
using OnResetLastErrorCodeDelegate = org.whitefossa.yiffhl.Abstractions.Interfaces.OnResetLastErrorCodeDelegate;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class ServiceCommandsManager : IServiceCommandsManager
    {
        private readonly IGetLastErrorCodeCommand _getLastErrorCodeCommand;
        private readonly IResetLastErrorCodeCommand _resetLastErrorCodeCommand;
        private readonly IUpdateSerialNumberCommand _updateSerialNumberCommand;
        private readonly IGetUbattADCCommand _getUbattADCCommand;
        private readonly IGetUbattVoltsCommand _getUbattVoltsCommand;
        private readonly IGetUbattFactorsCommand _getUbattFactorsCommand;

        private Abstractions.Interfaces.OnGetLastErrorCodeDelegate _onGetLastErrorCode;
        private Abstractions.Interfaces.OnResetLastErrorCodeDelegate _onResetLastErrorCode;
        private OnSerialNumberUpdateDelegate _onSerialNumberUpdate;
        private OnGetBatteryADCLevelDelegate _onGetBatteryADCLevel;
        private OnGetBatteryVoltageLevelDelegate _onGetBatteryVoltageLevel;
        private OnGetUBattFactorsDelegate _onGetUBattFactors;

        public ServiceCommandsManager(IGetLastErrorCodeCommand getLastErrorCodeCommand,
            IResetLastErrorCodeCommand resetLastErrorCodeCommand,
            IUpdateSerialNumberCommand updateSerialNumberCommand,
            IGetUbattADCCommand getUbattADCCommand,
            IGetUbattVoltsCommand getUbattVoltsCommand,
            IGetUbattFactorsCommand getUbattFactorsCommand)
        {
            _getLastErrorCodeCommand = getLastErrorCodeCommand;
            _resetLastErrorCodeCommand = resetLastErrorCodeCommand;
            _updateSerialNumberCommand = updateSerialNumberCommand;
            _getUbattADCCommand = getUbattADCCommand;
            _getUbattVoltsCommand = getUbattVoltsCommand;
            _getUbattFactorsCommand = getUbattFactorsCommand;
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

        public async Task GetUBattFactorsAsync(OnGetUBattFactorsDelegate onGetUBattFactors)
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
    }
}
