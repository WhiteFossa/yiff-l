using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetLastFailureCodeResponseDelegate(uint code);

    public class GetLastFailureCodeCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetLastFailureCodeResponseDelegate onGetLastFailureCodeResponse;

        public GetLastFailureCodeCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetLastFailureCodeResponse(OnGetLastFailureCodeResponse);
        }

        public void SetResponseDelegate(OnGetLastFailureCodeResponseDelegate onGetLastFailureCodeResponse)
        {
            this.onGetLastFailureCodeResponse = onGetLastFailureCodeResponse;
        }

        public void SendGetLastFailureCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetLastFailureCode, new List<byte>());
        }

        private void OnGetLastFailureCodeResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetLastFailureCodeResponse == null)
            {
                return;
            }

            if (payload.Count != 4)
            {
                return;
            }

            var code = BitConverter.ToUInt32(payload.ToArray(), 0);

            onGetLastFailureCodeResponse(code);
        }
    }
}
