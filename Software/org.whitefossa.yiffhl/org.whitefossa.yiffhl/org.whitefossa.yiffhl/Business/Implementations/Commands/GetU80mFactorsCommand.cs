using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetU80mFactorsCommand : IGetU80mFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetU80mFactorsResponseDelegate _onGetU80mFactorsResponse;

        public GetU80mFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetU80mFactorsResponse(OnGetU80mFactorsResponse);
        }

        public void SetResponseDelegate(OnGetU80mFactorsResponseDelegate onGetU80mFactorsResponse)
        {
            _onGetU80mFactorsResponse = onGetU80mFactorsResponse;
        }

        public void SendGetU80mFactorsCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetU80mADCToU80mVoltsFactors, new List<byte>());
        }

        private void OnGetU80mFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetU80mFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 8)
            {
                return;
            }

            var a = BitConverter.ToSingle(payload.ToArray(), 0);
            var b = BitConverter.ToSingle(payload.ToArray(), 4);

            _onGetU80mFactorsResponse(a, b);
        }
    }
}
