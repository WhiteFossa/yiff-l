using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetEndingToneDurationResponseDelegate(bool isSuccess);

    public class SetEndingToneDurationCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnSetEndingToneDurationResponseDelegate onSetEndingToneDurationResponse;

        public SetEndingToneDurationCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetEndingToneDurationResponse(OnSetEndingToneDurationResponse);
        }

        public void SetResponseDelegate(OnSetEndingToneDurationResponseDelegate onSetEndingToneDurationResponse)
        {
            this.onSetEndingToneDurationResponse = onSetEndingToneDurationResponse;
        }

        public void SendSetEndingToneResponseDurationCommand(TimeSpan endingToneDuration)
        {
            var payload = new List<byte>();

            // 2th (from 0th) byte - ending tone duration
            payload.Add((byte)endingToneDuration.TotalSeconds);

            packetsProcessor.SendCommand(CommandType.SetEndingToneDuration, payload);
        }

        private void OnSetEndingToneDurationResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetEndingToneDurationResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetEndingToneDurationResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
