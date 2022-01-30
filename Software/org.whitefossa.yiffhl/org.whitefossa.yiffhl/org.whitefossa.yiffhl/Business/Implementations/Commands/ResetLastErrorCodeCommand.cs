using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System.Collections.Generic;
using OnResetLastErrorCodeDelegate = org.whitefossa.yiffhl.Abstractions.Interfaces.Commands.OnResetLastErrorCodeDelegate;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class ResetLastErrorCodeCommand : IResetLastErrorCodeCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnResetLastErrorCodeDelegate _onResetLastErrorCode;

        public ResetLastErrorCodeCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnResetLastErrorCodeResponse(OnResetLastErrorCodeResponse);
        }

        public void SetResponseDelegate(OnResetLastErrorCodeDelegate onResetLastErrorCode)
        {
            _onResetLastErrorCode = onResetLastErrorCode;
        }

        public void SendResetLastErrorCodeCommand()
        {
            _packetsProcessor.SendCommand(CommandType.ResetLastFailureCode, new List<byte>());
        }

        private void OnResetLastErrorCodeResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onResetLastErrorCode == null)
            {
                return;
            }

            if (payload.Count != 0)
            {
                return;
            }

            _onResetLastErrorCode();
        }
    }
}
