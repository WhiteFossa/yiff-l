using System;
using System.Collections.Generic;
using System.Linq;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnDisarmFoxResponseDelegate(bool isSuccessful);

    public class DisarmFoxCommand
    {
        private readonly IPacketsProcessor packetsProcessor;
        private OnDisarmFoxResponseDelegate onDisarmFoxResponse;

        private DisarmFoxCommand()
        {

        }

        public DisarmFoxCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnDisarmFoxResponse(OnDisarmFoxResponse);
        }

        public void SetResponseDelegate(OnDisarmFoxResponseDelegate onDisarmFoxResponse)
        {
            this.onDisarmFoxResponse = onDisarmFoxResponse;
        }

        public void SendDisarmFoxCommand()
        {
            packetsProcessor.SendCommand(CommandType.DisarmFox, new List<byte>());
        }

        private void OnDisarmFoxResponse(IReadOnlyCollection<byte> payload)
        {
            if (onDisarmFoxResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onDisarmFoxResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}
