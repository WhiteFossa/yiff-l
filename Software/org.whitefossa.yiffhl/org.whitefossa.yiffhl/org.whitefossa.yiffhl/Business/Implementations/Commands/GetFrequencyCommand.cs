using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetFrequencyCommand : IGetFrequencyCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetFrequencyResponseDelegate _onGetFrequencyResponse;

        public GetFrequencyCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetFrequencyResponse(OnGetFrequencyResponse);
        }

        public void SetResponseDelegate(OnGetFrequencyResponseDelegate onGetFrequencyResponse)
        {
            _onGetFrequencyResponse = onGetFrequencyResponse;
        }

        public void SendGetFrequencyCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetFrequency, new List<byte>());
        }

        private void OnGetFrequencyResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetFrequencyResponse == null)
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

            _onGetFrequencyResponse(is144MHz, (int)frequency);
        }
    }
}
