using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class ForceTxOnCommand : IForceTxOnCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnForceTxOnResponseDelegate _onForceTxOnResponse;

        public ForceTxOnCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnForceTxOnResponse(OnForceTxOnResponse);
        }

        public void SetResponseDelegate(OnForceTxOnResponseDelegate onForceTxOnResponse)
        {
            _onForceTxOnResponse = onForceTxOnResponse;
        }

        public void SendForceTxOnCommand()
        {
            _packetsProcessor.SendCommand(CommandType.ForceTxOn, new List<byte>());
        }

        private void OnForceTxOnResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onForceTxOnResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onForceTxOnResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
