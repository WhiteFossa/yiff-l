using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetBattLevelFactorsCommand : ISetBattLevelFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetBattLevelFactorsResponseDelegate _onSetBattLevelFactorsResponse;

        public SetBattLevelFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetBattLevelFactorsResponse(OnSetBattLevelFactorsResponse);
        }

        public void SetResponseDelegate(OnSetBattLevelFactorsResponseDelegate onSetBattLevelFactorsResponse)
        {
            _onSetBattLevelFactorsResponse = onSetBattLevelFactorsResponse;
        }

        public void SendSetBattLevelFactors(bool resetToDefaults, float a, float b)
        {
            var payload = new List<byte>();

            payload.Add(CommandsHelper.FromBool(resetToDefaults));

            payload.AddRange(BitConverter.GetBytes(a));
            payload.AddRange(BitConverter.GetBytes(b));

            _packetsProcessor.SendCommand(CommandType.SetUbattVoltsToBattLevelFactors, payload);
        }

        private void OnSetBattLevelFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetBattLevelFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetBattLevelFactorsResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
