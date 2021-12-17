using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class IsFoxArmedCommand : IIsFoxArmedCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnIsFoxArmedResponseDelegate _onIsFoxArmedResponse;

        public IsFoxArmedCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnIsFoxArmedResponse(OnIsFoxArmedResponse);
        }

        public void SetResponseDelegate(OnIsFoxArmedResponseDelegate onIsFoxArmedResponse)
        {
            _onIsFoxArmedResponse = onIsFoxArmedResponse;
        }

        public void SendIsFoxArmedCommand()
        {
            _packetsProcessor.SendCommand(CommandType.IsFoxArmed, new List<byte>());
        }

        private void OnIsFoxArmedResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onIsFoxArmedResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onIsFoxArmedResponse(CommandsHelper.ToBool(payload.ElementAt(0)));
        }
    }
}
