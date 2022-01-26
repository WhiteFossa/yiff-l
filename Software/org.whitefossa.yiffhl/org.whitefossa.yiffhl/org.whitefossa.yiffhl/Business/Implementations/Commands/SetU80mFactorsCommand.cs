using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetU80mFactorsCommand : ISetU80mFactorsCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetU80mFactorsResponseDelegate _onSetU80mFactorsResponse;

        public SetU80mFactorsCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetU80mFactorsResponse(OnSetU80mFactorsResponse);
        }

        public void SetResponseDelegate(OnSetU80mFactorsResponseDelegate onSetU80mFactorsResponse)
        {
            _onSetU80mFactorsResponse = onSetU80mFactorsResponse;
        }

        public void SendSetU80mFactors(bool resetToDefaults, float a, float b)
        {
            var payload = new List<byte>();

            payload.Add(CommandsHelper.FromBool(resetToDefaults));

            payload.AddRange(BitConverter.GetBytes(a));
            payload.AddRange(BitConverter.GetBytes(b));

            _packetsProcessor.SendCommand(CommandType.SetU80mADCToU80mVoltsFactors, payload);
        }

        private void OnSetU80mFactorsResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetU80mFactorsResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetU80mFactorsResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
