using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetUAntADCResponseDelegate(float uAnt);

    public class GetUAntADCCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetUAntADCResponseDelegate onGetUAntADCResponse;

        public GetUAntADCCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetUAntADCResponse(OnGetUAntADCResponse);
        }
        public void SetResponseDelegate(OnGetUAntADCResponseDelegate onGetUAntADCResponse)
        {
            this.onGetUAntADCResponse = onGetUAntADCResponse;
        }

        public void SendGetUAntADCCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetUAntADC, new List<byte>());
        }

        private void OnGetUAntADCResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetUAntADCResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var level = BitConverter.ToSingle(payload.ToArray(), 0);

            onGetUAntADCResponse(level);
        }
    }
}
