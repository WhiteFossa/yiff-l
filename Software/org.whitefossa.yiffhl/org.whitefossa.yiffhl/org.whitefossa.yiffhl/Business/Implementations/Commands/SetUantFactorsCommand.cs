using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetUantFactorsCommand : ISetUantFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetUantFactorsResponseDelegate _onSetUantFactorsResponse;

        public SetUantFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetUantFactorsResponse(OnSetUantFactorsResponse);
        }

        public void SetResponseDelegate(OnSetUantFactorsResponseDelegate onSetUantFactorsResponse)
        {
            _onSetUantFactorsResponse = onSetUantFactorsResponse;
        }

        public void SendSetUantFactors(bool resetToDefaults, float a, float b)
        {
            var payload = new List<byte>();

            payload.Add(CommandsHelper.FromBool(resetToDefaults));

            payload.AddRange(BitConverter.GetBytes(a));
            payload.AddRange(BitConverter.GetBytes(b));

            _packetsProcessor.SendCommand(CommandType.SetUantADCToUantVoltsFactors, payload);
        }

        private void OnSetUantFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetUantFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetUantFactorsResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
