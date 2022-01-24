using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetU80mVoltsCommand : IGetU80mVoltsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetU80mVoltsResponseDelegate _onGetU80mVoltsResponse;

        public GetU80mVoltsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetU80mVoltsResponse(OnGetU80mVoltsResponse);
        }

        public void SetResponseDelegate(OnGetU80mVoltsResponseDelegate onGetU80MVoltsResponse)
        {
            _onGetU80mVoltsResponse = onGetU80MVoltsResponse;
        }

        public void SendGetU80mVoltsCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetU80mVolts, new List<byte>());
        }

        private void OnGetU80mVoltsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetU80mVoltsResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var voltage = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetU80mVoltsResponse(voltage);
        }
    }
}
