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

        private Abstractions.Interfaces.OnGetLastErrorCodeDelegate _onGetLastErrorCode;
        private Abstractions.Interfaces.OnResetLastErrorCodeDelegate _onResetLastErrorCode;

        public ServiceCommandsManager(IGetLastErrorCodeCommand getLastErrorCodeCommand,
            IResetLastErrorCodeCommand resetLastErrorCodeCommand)
        {
            _getLastErrorCodeCommand = getLastErrorCodeCommand;
            _resetLastErrorCodeCommand = resetLastErrorCodeCommand;
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
    }
}
