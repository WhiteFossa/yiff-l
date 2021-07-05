using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetU80mVoltsResponseDelegate(float u80m);

    public class GetU80mVoltsCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetU80mVoltsResponseDelegate onGetU80mVoltsResponse;

        public GetU80mVoltsCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetU80mVoltsResponse(OnGetU80mVoltsResponse);
        }

        public void SetResponseDelegate(OnGetU80mVoltsResponseDelegate onGetU80mVoltsResponse)
        {
            this.onGetU80mVoltsResponse = onGetU80mVoltsResponse;
        }

        public void SendGetU80mVoltsCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetU80mVolts, new List<byte>());
        }

        private void OnGetU80mVoltsResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetU80mVoltsResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            onGetU80mVoltsResponse(level);
        }

    }
}
