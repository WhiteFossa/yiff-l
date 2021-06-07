using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetEndingToneDurationResponseDelegate(TimeSpan duration);

    public class GetEndingToneDurationCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetEndingToneDurationResponseDelegate onGetEndingToneDurationResponse;

        public GetEndingToneDurationCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;
            packetsProcessor.SetOnGetEndingToneDurationResponse(OnGetEndingToneDurationResponse);
        }

        public void SetResponseDelegate(OnGetEndingToneDurationResponseDelegate onGetEndingToneDurationResponse)
        {
            this.onGetEndingToneDurationResponse = onGetEndingToneDurationResponse;
        }

        public void SendGetEndingToneDurationCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetEndingToneDuration, new List<byte>());
        }

        private void OnGetEndingToneDurationResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetEndingToneDurationResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onGetEndingToneDurationResponse(new TimeSpan(0, 0, payload.ElementAt(0)));
        }
    }
}
