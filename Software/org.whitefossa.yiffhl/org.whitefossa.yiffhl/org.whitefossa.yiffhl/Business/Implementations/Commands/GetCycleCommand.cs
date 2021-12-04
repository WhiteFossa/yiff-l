using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetCycleCommand : IGetCycleCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetCycleResponseDelegate _onGetCycleResponse;

        public GetCycleCommand()
        {
            _packetsProcessor = App.Container.Resolve<IPacketsProcessor>();
            _packetsProcessor.SetOnGetCycleResponse(OnGetCycleResponse);
        }
        public void SetResponseDelegate(OnGetCycleResponseDelegate onGetCycleResponse)
        {
            _onGetCycleResponse = onGetCycleResponse;
        }

        public void SendGetCycleCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetCycle, new List<byte>());
        }

        private void OnGetCycleResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetCycleResponse == null)
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

            _onGetCycleResponse(isContinuous, new TimeSpan(0, 0, txTimeSeconds), new TimeSpan(0, 0, pauseTimeSeconds));
        }
    }
}
