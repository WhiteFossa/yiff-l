using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetP80mFactorsCommand : IGetP80mFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetP80mFactorsResponseDelegate _onGetP80mFactorsResponse;

        public GetP80mFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetP80mFactorsResponse(OnGetP80mFactorsResponse);
        }

        public void SetResponseDelegate(OnGetP80mFactorsResponseDelegate onGetP80mFactorsResponse)
        {
            _onGetP80mFactorsResponse = onGetP80mFactorsResponse;
        }

        public void SendGetP80mFactorsCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetP80mToU80mFactors, new List<byte>());
        }

        private void OnGetP80mFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetP80mFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 8)
            {
                return;
            }

            var a = BitConverter.ToSingle(payload.ToArray(), 0);
            var b = BitConverter.ToSingle(payload.ToArray(), 4);

            _onGetP80mFactorsResponse(a, b);
        }
    }
}
