using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetCurrentProfileIdResponseDelegate(int profileId);

    public class GetCurrentProfileIdCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetCurrentProfileIdResponseDelegate onGetCurrentProfileIdResponse;

        public GetCurrentProfileIdCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetCurrentProfileIdResponse(OnGetCurrentProfileIdResponse);
        }

        public void SetResponseDelegate(OnGetCurrentProfileIdResponseDelegate onGetCurrentProfileIdResponse)
        {
            this.onGetCurrentProfileIdResponse = onGetCurrentProfileIdResponse;
        }

        public void SendGetCurrentProfileIdCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetCurrentProfileId, new List<byte>());
        }

        private void OnGetCurrentProfileIdResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetCurrentProfileIdResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onGetCurrentProfileIdResponse(payload.ElementAt(0));
        }
    }
}
