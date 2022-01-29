using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetUantADCCommand : IGetUantADCCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetUantADCResponseDelegate _onGetUantADCResponse;

        public GetUantADCCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetUAntADCResponse(OnGetUantADCResponse);
        }

        public void SetResponseDelegate(OnGetUantADCResponseDelegate onGetUantADCResponse)
        {
            _onGetUantADCResponse = onGetUantADCResponse;
        }

        public void SendGetUantADCCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetUAntADC, new List<byte>());
        }

        private void OnGetUantADCResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetUantADCResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var uantADC = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetUantADCResponse(uantADC);
        }
    }
}
