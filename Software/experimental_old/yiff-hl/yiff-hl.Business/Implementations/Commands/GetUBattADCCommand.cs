using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetUBattADCResponseDelegate(float uBatt);

    public class GetUBattADCCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetUBattADCResponseDelegate onGetUBattADCResponse;

        public GetUBattADCCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetUBattADCResponse(OnGetUBattADCResponse);
        }

        public void SetResponseDelegate(OnGetUBattADCResponseDelegate onGetUBattADCResponse)
        {
            this.onGetUBattADCResponse = onGetUBattADCResponse;
        }

        public void SendGetUBattADCCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetUBattADC, new List<byte>());
        }

        private void OnGetUBattADCResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetUBattADCResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            onGetUBattADCResponse(level);
        }
    }
}
