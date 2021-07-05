using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetUBattVoltsResponseDelegate(float uBatt);

    public class GetUBattVoltsCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetUBattVoltsResponseDelegate onGetUBattVoltsResponse;

        public GetUBattVoltsCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetUBattVoltsResponse(OnGetUBattVoltsResponse);
        }

        public void SetResponseDelegate(OnGetUBattVoltsResponseDelegate onGetUBattVoltsResponse)
        {
            this.onGetUBattVoltsResponse = onGetUBattVoltsResponse;
        }

        public void SendGetUBattVoltsCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetUBattVolts, new List<byte>());
        }

        private void OnGetUBattVoltsResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetUBattVoltsResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            onGetUBattVoltsResponse(level);
        }
    }
}
