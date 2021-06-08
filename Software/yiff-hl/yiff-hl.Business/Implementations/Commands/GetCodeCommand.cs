using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnGetCodeResponseDelegate(FoxCode code);

    public class GetCodeCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnGetCodeResponseDelegate onGetCodeResponse;

        public GetCodeCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnGetCodeResponse(OnGetCodeResponse);
        }

        public void SetResponseDelegate(OnGetCodeResponseDelegate onGetCodeResponse)
        {
            this.onGetCodeResponse = onGetCodeResponse;
        }

        public void SendGetCodeCommand()
        {
            packetsProcessor.SendCommand(CommandType.GetCode, new List<byte>());
        }

        private void OnGetCodeResponse(IReadOnlyCollection<byte> payload)
        {
            if (onGetCodeResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            var foxCode = (FoxCode)payload.ElementAt(0);

            onGetCodeResponse(foxCode);
        }
    }
}
