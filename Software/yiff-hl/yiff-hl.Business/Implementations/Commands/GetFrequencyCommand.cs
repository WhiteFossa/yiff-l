using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetFrequencyResponseDelegate(bool is144MHz, uint frequency);

    public class GetFrequencyCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetFrequencyResponseDelegate onGetFrequencyResponse;

        public GetFrequencyCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;
            packetsProcessor.SetOnGetFrequencyResponse(OnGetFrequencyResponse);
        }

        public void SetResponseDelegate(OnGetFrequencyResponseDelegate onGetFrequencyResponse)
        {
            this.onGetFrequencyResponse = onGetFrequencyResponse;
        }

        public void SendGetFrequencyCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetFrequency, new List<byte>());
        }

        private void OnGetFrequencyResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetFrequencyResponse == null)
            {
                return;
            }

            if (payload.Count != 5)
            {
                return;
            }

            var is144MHz = CommandsHelper.ToBool(payload.ElementAt(0));

            var frequencyBytes = payload
                .ToList()
                .GetRange(1, 4)
                .ToArray();

            var frequency = BitConverter.ToUInt32(frequencyBytes, 0);

            onGetFrequencyResponse(is144MHz, frequency);
        }
    }
}
