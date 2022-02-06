using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetDisarmOnDischargeThresholdCommand : IGetDisarmOnDischargeThresholdCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetDisarmOnDischargeThresholdResponseDelegate _onGetDisarmOnDischargeThresholdResponse;

        public GetDisarmOnDischargeThresholdCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetDisarmOnDischargeThresholdResponse(OnGetDisarmOnDischargeThresholdResponse);
        }

        public void SetResponseDelegate(OnGetDisarmOnDischargeThresholdResponseDelegate onGetDisarmOnDischargeThresholdResponse)
        {
            _onGetDisarmOnDischargeThresholdResponse = onGetDisarmOnDischargeThresholdResponse;
        }

        public void SendGetDisarmOnDischargeThreshold()
        {
            _packetsProcessor.SendCommand(CommandType.GetDisarmOnDischargeThreshold, new List<byte>());
        }

        private void OnGetDisarmOnDischargeThresholdResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetDisarmOnDischargeThresholdResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var threshold = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetDisarmOnDischargeThresholdResponse(threshold);
        }
    }
}
