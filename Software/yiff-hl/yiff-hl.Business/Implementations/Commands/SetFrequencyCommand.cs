using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetFrequencyResponseDelegate(bool isSuccess);

    public class SetFrequencyCommand
    {
        private const int Min35Frequency = 3500000;
        private const int Max35Frequency = 3650000;

        private const int Min144Frequency = 144000000;
        private const int Max144Frequency = 146000000;

        private readonly IPacketsProcessor packetsProcessor;
        private OnSetFrequencyResponseDelegate onSetFrequencyResponse;

        public SetFrequencyCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetFrequencyResponse(OnSetFrequencyResponse);
        }

        public void SetResponseDelegate(OnSetFrequencyResponseDelegate onSetFrequencyResponse)
        {
            this.onSetFrequencyResponse = onSetFrequencyResponse;
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

            packetsProcessor.SendCommand(CommandType.SetFrequency, payload);
        }

        private void OnSetFrequencyResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetFrequencyResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetFrequencyResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
