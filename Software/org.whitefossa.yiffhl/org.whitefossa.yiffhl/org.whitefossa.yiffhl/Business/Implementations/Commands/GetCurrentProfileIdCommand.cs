using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetCurrentProfileIdCommand : IGetCurrentProfileIdCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetCurrentProfileIdResponseDelegate _onGetCurrentProfileIdResponse;

        public GetCurrentProfileIdCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetCurrentProfileIdResponse(OnGetCurrentProfileIdResponse);
        }

        public void SetResponseDelegate(OnGetCurrentProfileIdResponseDelegate onGetCurrentProfileIdResponse)
        {
            _onGetCurrentProfileIdResponse = onGetCurrentProfileIdResponse;
        }

        public void SendGetCurrentProfileIdCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetCurrentProfileId, new List<byte>());
        }

        private void OnGetCurrentProfileIdResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetCurrentProfileIdResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onGetCurrentProfileIdResponse(payload.ElementAt(0));
        }
    }
}
