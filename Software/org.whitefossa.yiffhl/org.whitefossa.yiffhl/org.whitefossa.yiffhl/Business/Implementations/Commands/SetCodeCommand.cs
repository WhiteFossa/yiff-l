using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetCodeCommand : ISetCodeCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetCodeResponseDelegate _onSetCodeResponse;

        public SetCodeCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnSetCodeResponse(OnSetCodeResponse);
        }
        public void SetResponseDelegate(OnSetCodeResponseDelegate onSetCodeResponse)
        {
            _onSetCodeResponse = onSetCodeResponse;
        }

        public void SendSetCodeCommand(FoxCode code)
        {
            var payload = new List<byte>();

            payload.Add((byte)code);

            _packetsProcessor.SendCommand(CommandType.SetCode, payload);
        }

        private void OnSetCodeResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetCodeResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetCodeResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
