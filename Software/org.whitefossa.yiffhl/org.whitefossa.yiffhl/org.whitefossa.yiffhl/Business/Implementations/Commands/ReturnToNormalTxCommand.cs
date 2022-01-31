using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class ReturnToNormalTxCommand : IReturnToNormalTxCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnReturnToNormalTxResponseDelegate _onReturnToNormalTxResponse;

        public ReturnToNormalTxCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnReturnToNormalTxResponse(OnReturnToNormalTxResponse);
        }

        public void SetResponseDelegate(OnReturnToNormalTxResponseDelegate onReturnToNormalTxResponse)
        {
            _onReturnToNormalTxResponse = onReturnToNormalTxResponse;
        }

        public void SendReturnToNormalTxCommand()
        {
            _packetsProcessor.SendCommand(CommandType.ReturnToNormalTx, new List<byte>());
        }

        private void OnReturnToNormalTxResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onReturnToNormalTxResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onReturnToNormalTxResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
