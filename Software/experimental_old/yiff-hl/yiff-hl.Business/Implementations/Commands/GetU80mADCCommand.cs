using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetU80mADCResponseDelegate(float u80m);

    public class GetU80mADCCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetU80mADCResponseDelegate onGetU80mADCResponse;

        public GetU80mADCCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetU80mADCResponse(OnGetU80mADCResponse);
        }

        public void SetResponseDelegate(OnGetU80mADCResponseDelegate onGetU80mADCResponse)
        {
            this.onGetU80mADCResponse = onGetU80mADCResponse;
        }

        public void SendGetU80mADCCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetU80mADC, new List<byte>());
        }

        private void OnGetU80mADCResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetU80mADCResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            onGetU80mADCResponse(level);
        }
    }
}
