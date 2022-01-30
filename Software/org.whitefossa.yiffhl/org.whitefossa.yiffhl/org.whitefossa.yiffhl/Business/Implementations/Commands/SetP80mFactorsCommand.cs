using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetP80mFactorsCommand : ISetP80mFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetP80mFactorsResponseDelegate _onSetP80mFactorsResponse;

        public SetP80mFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetP80mFactorsResponse(OnSetP80mFactorsResponse);
        }

        public void SetResponseDelegate(OnSetP80mFactorsResponseDelegate onSetP80mFactorsResponse)
        {
            _onSetP80mFactorsResponse = onSetP80mFactorsResponse;
        }

        public void SendSetP80mFactors(bool resetToDefaults, float a, float b)
        {
            var payload = new List<byte>();

            payload.Add(CommandsHelper.FromBool(resetToDefaults));

            payload.AddRange(BitConverter.GetBytes(a));
            payload.AddRange(BitConverter.GetBytes(b));

            _packetsProcessor.SendCommand(CommandType.SetP80mToU80mFactors, payload);
        }

        private void OnSetP80mFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetP80mFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetP80mFactorsResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
