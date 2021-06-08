using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetCycleResponseDelegate(bool isSuccess);

    public class SetCycleCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnSetCycleResponseDelegate onSetCycleResponse;

        public SetCycleCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetCycleResponse(OnSetCycleResponse);
        }

        public void SetResponseDelegate(OnSetCycleResponseDelegate onSetCycleResponse)
        {
            this.onSetCycleResponse = onSetCycleResponse;
        }

        public void SendSetCycleCommand(bool isContinuous, TimeSpan txTime, TimeSpan pauseTime)
        {
            var payload = new List<byte>();

            // 2th (from 0th) byte - is continuous flag
            payload.Add(CommandsHelper.FromBool(isContinuous));

            // 3th - 4th bytes - TX time
            ushort txTimeInSeconds = (ushort)txTime.TotalSeconds;
            payload.AddRange(BitConverter.GetBytes(txTimeInSeconds));

            // 5th - 6th bytes - pause time
            ushort pauseTimeInSeconds = (ushort)pauseTime.TotalSeconds;
            payload.AddRange(BitConverter.GetBytes(pauseTimeInSeconds));

            packetsProcessor.SendCommand(CommandType.SetCycle, payload);
        }

        private void OnSetCycleResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetCycleResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetCycleResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
