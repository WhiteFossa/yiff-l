using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetFrequencyCommand : ISetFrequencyCommand
    {
        private const int Min35Frequency = 3500000;
        private const int Max35Frequency = 3650000;

        private const int Min144Frequency = 144000000;
        private const int Max144Frequency = 146000000;

        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetFrequencyResponseDelegate _onSetFrequencyResponse;

        public SetFrequencyCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetFrequencyResponse(OnSetFrequencyResponse);
        }

        public void SetResponseDelegate(OnSetFrequencyResponseDelegate onSetFrequencyResponse)
        {
            _onSetFrequencyResponse = onSetFrequencyResponse;
        }

        public void SendSetFrequencyCommand(bool is144MHz, int frequency)
        {
            if (is144MHz)
            {
                if (frequency < Min144Frequency || frequency > Max144Frequency)
                {
                    throw new ArgumentException("Wrong frequency", nameof(frequency));
                }
            }
            else
            {
                if (frequency < Min35Frequency || frequency > Max35Frequency)
                {
                    throw new ArgumentException("Wrong frequency", nameof(frequency));
                }
            }

            var payload = new List<byte>();

            // 2th (from 0th) byte - is 144MHz flag
            payload.Add(CommandsHelper.FromBool(is144MHz));

            // 3th - 6th bytes - frequency
            payload.AddRange(BitConverter.GetBytes((uint)frequency));

            _packetsProcessor.SendCommand(CommandType.SetFrequency, payload);
        }

        private void OnSetFrequencyResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetFrequencyResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetFrequencyResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
