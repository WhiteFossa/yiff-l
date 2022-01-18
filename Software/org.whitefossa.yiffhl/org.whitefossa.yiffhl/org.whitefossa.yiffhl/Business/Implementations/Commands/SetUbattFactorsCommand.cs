using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetUbattFactorsCommand : ISetUbattFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetUbattFactorsResponseDelegate _onSetUbattFactorsResponse;

        public SetUbattFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetUBattFactorsResponse(OnSetUbattFactorsResponse);
        }

        public void SetResponseDelegate(OnSetUbattFactorsResponseDelegate onSetUbattFactorsResponse)
        {
            _onSetUbattFactorsResponse = onSetUbattFactorsResponse;
        }

        public void SendSetUbattFactors(bool resetToDefaults, float a, float b)
        {
            var payload = new List<byte>();

            payload.Add(CommandsHelper.FromBool(resetToDefaults));

            payload.AddRange(BitConverter.GetBytes(a));
            payload.AddRange(BitConverter.GetBytes(b));

            _packetsProcessor.SendCommand(CommandType.SetUbattADCToUbattVoltsFactors, payload);
        }

        private void OnSetUbattFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetUbattFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetUbattFactorsResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
