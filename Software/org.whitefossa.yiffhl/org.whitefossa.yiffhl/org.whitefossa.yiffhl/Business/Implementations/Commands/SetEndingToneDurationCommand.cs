using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetEndingToneDurationCommand : ISetEndingToneDurationCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetEndingToneDurationResponseDelegate _onSetEndingToneDurationResponse;

        public SetEndingToneDurationCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            packetsProcessor.SetOnSetEndingToneDurationResponse(OnSetEndingToneDurationResponse);
        }

        public void SetResponseDelegate(OnSetEndingToneDurationResponseDelegate onSetEndingToneDurationResponse)
        {
            _onSetEndingToneDurationResponse = onSetEndingToneDurationResponse;
        }

        public void SendSetEndingToneResponseDurationCommand(TimeSpan endingToneDuration)
        {
            var payload = new List<byte>();

            // 2th (from 0th) byte - ending tone duration
            payload.Add((byte)endingToneDuration.TotalSeconds);

            _packetsProcessor.SendCommand(CommandType.SetEndingToneDuration, payload);
        }

        private void OnSetEndingToneDurationResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetEndingToneDurationResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetEndingToneDurationResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
