using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace org.whitefossa.yiffhl.Business.Implementations.Commands
{
    public class GetBatteryLevelCommand : IGetBatteryLevelCommand
    {
        private readonly IPacketsProcessor _packetsProcessor;
        private OnGetBatteryLevelResponseDelegate _onGetBatteryLevelResponse;

        public GetBatteryLevelCommand(IPacketsProcessor packetsProcessor)
        {
            _packetsProcessor = packetsProcessor;
            _packetsProcessor.SetOnGetBatteryLevelResponse(OnGetBatteryLevelResponse);
        }

        public void SetResponseDelegate(OnGetBatteryLevelResponseDelegate onGetBatteryLevelResponse)
        {
            _onGetBatteryLevelResponse = onGetBatteryLevelResponse;
        }

        public void SendGetBatteryLevelCommand()
        {
            _packetsProcessor.SendCommand(CommandType.GetBatteryLevel, new List<byte>());
        }

        private void OnGetBatteryLevelResponse(IReadOnlyCollection<byte> payload)
        {
            if (_onGetBatteryLevelResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            _onGetBatteryLevelResponse(level);
        }
    }
}
