using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetUantVoltsCommand : IGetUantVoltsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetUantVoltsResponseDelegate _onGetUantVoltsResponse;

        public GetUantVoltsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetUAntVoltsResponse(OnGetUantVoltsResponse);
        }

        public void SetResponseDelegate(OnGetUantVoltsResponseDelegate onGetUantVoltsResponse)
        {
            _onGetUantVoltsResponse = onGetUantVoltsResponse;
        }

        public void SendGetUantVoltsCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetUAntVolts, new List<byte>());
        }

        private void OnGetUantVoltsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetUantVoltsResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var uantVolts = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetUantVoltsResponse(uantVolts);
        }
    }
}
