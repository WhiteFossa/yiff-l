using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetCodeResponseDelegate(bool isSuccessfull);

    public class SetCodeCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnSetCodeResponseDelegate onSetCodeResponse;

        public SetCodeCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetCodeResponse(OnSetCodeResponse);
        }
        public void SetResponseDelegate(OnSetCodeResponseDelegate onSetCodeResponse)
        {
            this.onSetCodeResponse = onSetCodeResponse;
        }

        public void SendSetCodeCommand(FoxCode code)
        {
            var payload = new List<byte>();

            payload.Add((byte)code);

            packetsProcessor.SendCommand(CommandType.SetCode, payload);
        }

        private void OnSetCodeResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetCodeResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetCodeResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
