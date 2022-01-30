using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetBattLevelFactorsCommand : IGetBattLevelFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetBattLevelFactorsResponseDelegate _onGetBattLevelFactorsResponse;

        public GetBattLevelFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetBattLevelFactorsResponse(OnGetBattLevelFactorsResponse);
        }

        public void SetResponseDelegate(OnGetBattLevelFactorsResponseDelegate onGetBattLevelFactorsResponse)
        {
            _onGetBattLevelFactorsResponse = onGetBattLevelFactorsResponse;
        }

        public void SendGetBattLevelFactorsCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetUbattVoltsToBattLevelFactors, new List<byte>());
        }

        private void OnGetBattLevelFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetBattLevelFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 8)
            {
                return;
            }

            var a = BitConverter.ToSingle(payload.ToArray(), 0);
            var b = BitConverter.ToSingle(payload.ToArray(), 4);

            _onGetBattLevelFactorsResponse(a, b);
        }
    }
}
