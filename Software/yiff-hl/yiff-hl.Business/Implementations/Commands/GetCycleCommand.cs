using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetCycleResponseDelegate(bool isContinuous, TimeSpan txTime, TimeSpan pauseTime);

    public class GetCycleCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetCycleResponseDelegate onGetCycleResponse;

        public GetCycleCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor;
            packetsProcessor.SetOnGetCycleResponse(OnGetCycleResponse);
        }
        public void SetResponseDelegate(OnGetCycleResponseDelegate onGetCycleResponse)
        {
            this.onGetCycleResponse = onGetCycleResponse;
        }

        public void SendGetCycleCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetCycle, new List<byte>());
        }

        private void OnGetCycleResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetCycleResponse == null)
            {
                return;
            }

            if (payload.Count != 5)
            {
                return;
            }

            var isContinuous = CommandsHelper.ToBool(payload.ElementAt(0));

            var txTimeSecondsBytes = payload
                .ToList()
                .GetRange(1, 2)
                .ToArray();

            var txTimeSeconds = BitConverter.ToUInt16(txTimeSecondsBytes, 0);

            var pauseTimeSecondsBytes = payload
                .ToList()
                .GetRange(3, 2)
                .ToArray();

            var pauseTimeSeconds = BitConverter.ToUInt16(pauseTimeSecondsBytes, 0);

            onGetCycleResponse(isContinuous, new TimeSpan(0, 0, txTimeSeconds), new TimeSpan(0, 0, pauseTimeSeconds));
        }
    }
}
