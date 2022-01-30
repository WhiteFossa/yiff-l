using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetUantFactorsCommand : IGetUantFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetUantFactorsResponseDelegate _onGetUantFactorsResponse;

        public GetUantFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetUantFactorsResponse(OnGetUantFactorsResponse);
        }

        public void SetResponseDelegate(OnGetUantFactorsResponseDelegate onGetUantFactorsResponse)
        {
            _onGetUantFactorsResponse = onGetUantFactorsResponse;
        }

        public void SendGetUantFactorsCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetUantADCToUantVoltsFactors, new List<byte>());
        }

        private void OnGetUantFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetUantFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 8)
            {
                return;
            }

            var a = BitConverter.ToSingle(payload.ToArray(), 0);
            var b = BitConverter.ToSingle(payload.ToArray(), 4);

            _onGetUantFactorsResponse(a, b);
        }
    }
}
