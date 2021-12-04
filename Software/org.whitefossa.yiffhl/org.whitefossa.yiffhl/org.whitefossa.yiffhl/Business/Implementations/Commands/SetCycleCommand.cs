using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetCycleCommand : ISetCycleCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetCycleResponseDelegate _onSetCycleResponse;

        public SetCycleCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetCycleResponse(OnSetCycleResponse);
        }

        public void SetResponseDelegate(OnSetCycleResponseDelegate onSetCycleResponse)
        {
            _onSetCycleResponse = onSetCycleResponse;
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

            _packetsProcessor.SendCommand(CommandType.SetCycle, payload);
        }

        private void OnSetCycleResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetCycleResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetCycleResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
