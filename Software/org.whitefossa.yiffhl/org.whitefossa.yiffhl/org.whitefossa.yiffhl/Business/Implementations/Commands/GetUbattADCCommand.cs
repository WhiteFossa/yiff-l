using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetUbattADCCommand : IGetUbattADCCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetUBattADCResponseDelegate _onGetUBattADCResponse;

        public GetUbattADCCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetUBattADCResponse(OnGetUBattADCResponse);
        }

        public void SetResponseDelegate(OnGetUBattADCResponseDelegate onGetUBattADCResponse)
        {
            _onGetUBattADCResponse = onGetUBattADCResponse;
        }

        public void SendGetUbattADCCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetUBattADC, new List<byte>());
        }

        private void OnGetUBattADCResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetUBattADCResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetUBattADCResponse(level);
        }
    }
}
