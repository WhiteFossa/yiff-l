using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetUbattVoltsCommand : IGetUbattVoltsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetUBattVoltsResponseDelegate _onGetUBattVoltsResponse;

        public GetUbattVoltsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetUBattVoltsResponse(OnGetUBattVoltsResponse);
        }

        public void SetResponseDelegate(OnGetUBattVoltsResponseDelegate onGetUBattVoltsResponse)
        {
            _onGetUBattVoltsResponse = onGetUBattVoltsResponse;
        }

        public void SendGetUbattVoltsCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetUBattVolts, new List<byte>());
        }

        private void OnGetUBattVoltsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetUBattVoltsResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetUBattVoltsResponse(level);
        }
    }
}
