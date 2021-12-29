using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System.Collections.Generic;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class MarkMatchingAsSeenCommand : IMarkMatchingAsSeenCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnMarkMatchingAsSeenResponseDelegate _onMarkMatchingAsSeenResponse;

        public MarkMatchingAsSeenCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnMarkMatchingAsSeenResponse(OnMarkMatchingAsSeenResponse);
        }

        public void SetResponseDelegate(OnMarkMatchingAsSeenResponseDelegate onMarkMatchingAsSeenResponse)
        {
            _onMarkMatchingAsSeenResponse = onMarkMatchingAsSeenResponse;
        }

        public void SendMarkMatchingAsSeenCommand()
        {
            _packetsProcessor.SendCommand(CommandType.MarkMatchingAsSeen, new List<byte>());
        }

        private void OnMarkMatchingAsSeenResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onMarkMatchingAsSeenResponse == null)
            {
                return;
            }

            if (payload.Count != 0)
            {
                return;
            }

            _onMarkMatchingAsSeenResponse();
        }

    }
}
