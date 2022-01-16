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

        private Abstractions.Interfaces.OnGetLastErrorCodeDelegate _onGetLastErrorCode;
        private Abstractions.Interfaces.OnResetLastErrorCodeDelegate _onResetLastErrorCode;
        private OnSerialNumberUpdateDelegate _onSerialNumberUpdate;
        private OnGetBatteryADCLevelDelegate _onGetBatteryADCLevel;

        public ServiceCommandsManager(IGetLastErrorCodeCommand getLastErrorCodeCommand,
            IResetLastErrorCodeCommand resetLastErrorCodeCommand,
            IUpdateSerialNumberCommand updateSerialNumberCommand,
            IGetUbattADCCommand getUbattADCCommand)
        {
            _getLastErrorCodeCommand = getLastErrorCodeCommand;
            _resetLastErrorCodeCommand = resetLastErrorCodeCommand;
            _updateSerialNumberCommand = updateSerialNumberCommand;
            _getUbattADCCommand = getUbattADCCommand;
        }

        #region Get last error code

        public async Task GetLastErrorCode(Abstractions.Interfaces.OnGetLastErrorCodeDelegate onGetLastErrorCode)
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

        public async Task ResetLastErrorCode(OnResetLastErrorCodeDelegate onResetLastErrorCode)
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

        public async Task UpdateSerialNumber(uint newSerialNumber, OnSerialNumberUpdateDelegate onSerialNumberUpdate)
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

        public async Task GetBatteryADCLevel(OnGetBatteryADCLevelDelegate onGetBatteryADCLevel)
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
    }
}
