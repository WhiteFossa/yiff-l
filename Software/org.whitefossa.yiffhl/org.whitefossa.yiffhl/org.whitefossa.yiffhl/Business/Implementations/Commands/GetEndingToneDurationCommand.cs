using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetEndingToneDurationCommand : IGetEndingToneDurationCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetEndingToneDurationResponseDelegate _onGetEndingToneDurationResponse;

        public GetEndingToneDurationCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            _packetsProcessor.SetOnGetEndingToneDurationResponse(OnGetEndingToneDurationResponse);
        }

        public void SetResponseDelegate(OnGetEndingToneDurationResponseDelegate onGetEndingToneDurationResponse)
        {
            _onGetEndingToneDurationResponse = onGetEndingToneDurationResponse;
        }

        public void SendGetEndingToneDurationCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetEndingToneDuration, new List<byte>());
        }

        private void OnGetEndingToneDurationResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetEndingToneDurationResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onGetEndingToneDurationResponse(new TimeSpan(0, 0, payload.ElementAt(0)));
        }
    }
}
