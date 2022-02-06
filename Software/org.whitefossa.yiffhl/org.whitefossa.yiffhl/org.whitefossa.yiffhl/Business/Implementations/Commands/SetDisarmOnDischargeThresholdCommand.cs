using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using org.whitefossa.yiffhl.Business.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class SetDisarmOnDischargeThresholdCommand : ISetDisarmOnDischargeThresholdCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnSetDisarmOnDischargeThresholdResponseDelegate _onSetDisarmOnDischargeThresholdResponse;

        public SetDisarmOnDischargeThresholdCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnSetDisarmOnDischargeThresholdResponse(OnSetDisarmOnDischargeThresholdResponse);
        }

        public void SetResponseDelegate(OnSetDisarmOnDischargeThresholdResponseDelegate onSetDisarmOnDischargeThresholdResponse)
        {
            _onSetDisarmOnDischargeThresholdResponse = onSetDisarmOnDischargeThresholdResponse;
        }

        public void SendSetDisarmOnDischargeThreshold(float newThreshold)
        {
            var payload = new List<byte>();

            payload.AddRange(BitConverter.GetBytes(newThreshold));

            _packetsProcessor.SendCommand(CommandType.SetDisarmOnDischargeThreshold, payload);
        }

        private void OnSetDisarmOnDischargeThresholdResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onSetDisarmOnDischargeThresholdResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            _onSetDisarmOnDischargeThresholdResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
