using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class ServiceCommandsManager : IServiceCommandsManager
    {
        private readonly IGetLastErrorCodeCommand _getLastErrorCodeCommand;

        private OnGetLastErrorCodeResultDelegate _onGetLastErrorCode;

        public ServiceCommandsManager(IGetLastErrorCodeCommand getLastErrorCodeCommand)
        {
            _getLastErrorCodeCommand = getLastErrorCodeCommand;
        }

        #region Get last error code

        public async Task GetLastErrorCode(OnGetLastErrorCodeResultDelegate onGetLastErrorCode)
        {
            _onGetLastErrorCode = onGetLastErrorCode ?? throw new ArgumentNullException(nameof(onGetLastErrorCode));

            _getLastErrorCodeCommand.SetResponseDelegate(OnGetLastErrorCode);
            _getLastErrorCodeCommand.SendGetLastErrorCodeCommand();
        }

        private void OnGetLastErrorCode(uint code)
        {
            _onGetLastErrorCode(code);
        }

        #endregion
    }
}
