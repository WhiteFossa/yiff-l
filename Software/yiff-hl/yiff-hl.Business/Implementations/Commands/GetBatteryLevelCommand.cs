using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetBatteryLevelResponseDelegate(float level);

    public class GetBatteryLevelCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetBatteryLevelResponseDelegate onGetBatteryLevelResponse;

        private GetBatteryLevelCommand()
        {

        }

        public GetBatteryLevelCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetBatteryLevelResponse(OnGetBatteryLevelResponse);
        }

        public void SetResponseDelegate(OnGetBatteryLevelResponseDelegate onGetBatteryLevelResponse)
        {
            this.onGetBatteryLevelResponse = onGetBatteryLevelResponse;
        }

        public void SendGetBatteryLevelCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetBatteryLevel, new List<byte>());
        }

        private void OnGetBatteryLevelResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetBatteryLevelResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            onGetBatteryLevelResponse(level);
        }
    }
}
