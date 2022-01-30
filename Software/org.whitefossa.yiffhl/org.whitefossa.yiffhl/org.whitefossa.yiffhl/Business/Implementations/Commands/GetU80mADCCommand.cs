using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetU80mADCCommand : IGetU80mADCCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetU80mADCResponseDelegate _onGetU80mADCResponse;

        public GetU80mADCCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetU80mADCResponse(OnGetU80mADCResponse);
        }

        public void SetResponseDelegate(OnGetU80mADCResponseDelegate onGetU80mADCResponse)
        {
            _onGetU80mADCResponse = onGetU80mADCResponse;
        }

        public void SendGetU80mADCCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetU80mADC, new List<byte>());
        }

        private void OnGetU80mADCResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetU80mADCResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var u80mADC = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetU80mADCResponse(u80mADC);
        }
    }
}
