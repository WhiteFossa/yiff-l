using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetUbattFactorsCommand : IGetUbattFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetUbattFactorsResponseDelegate _onGetUbattFactorsResponse;

        public GetUbattFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetUBattFactorsResponse(OnGetUBattFactorsResponse);
        }

        public void SetResponseDelegate(OnGetUbattFactorsResponseDelegate onGetUbattFactorsResponse)
        {
            _onGetUbattFactorsResponse = onGetUbattFactorsResponse;
        }

        public void SendGetUbattFactorsCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetUbattADCToUbattVoltsFactors, new List<byte>());
        }

        private void OnGetUBattFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetUbattFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 8)
            {
                return;
            }

            var a = BitConverter.ToSingle(payload.ToArray(), 0);
            var b = BitConverter.ToSingle(payload.ToArray(), 4);

            _onGetUbattFactorsResponse(a, b);
        }
    }
}
