using System;
using System.Collections.Generic;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnNoOperationResponseDelegate();

    public class NoOperationCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnNoOperationResponseDelegate onNoOperationResponse;

        public NoOperationCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnNoOperationResponse(OnNoOperationResponse);
        }

        public void SetResponseDelegate(OnNoOperationResponseDelegate onNoOperationResponse)
        {
            this.onNoOperationResponse = onNoOperationResponse;
        }

        public void SendNoOperationCommand()
        {
            packetsProcessor.SendCommand(CommandType.NoOperation, new List<byte>());
        }

        private void OnNoOperationResponse(IReadOnlyCollection<byte> payload)
        {
            if (onNoOperationResponse == null)
            {
                return;
            }

            if (payload.Count != 0)
            {
                return;
            }

            onNoOperationResponse();
        }

    }
}
